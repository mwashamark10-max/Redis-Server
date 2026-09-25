#include "Server.h"
#include "RedisStore.h"
#include <iostream>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
RespParser parser;
Server::Server(int portno):handler(store){
    this->port = portno;
    this->serverSocket = -1;
}
void Server::sendAll(int clientSocket,const std::string& response){
    std::size_t totalSent=0;
    while(totalSent<response.size()){
        ssize_t bytesSent=send(clientSocket,response.c_str()+totalSent,response.size()-totalSent,0);
        if(bytesSent<0){
            throw std::runtime_error("Send Failed");
        }
        totalSent+=bytesSent;
    }
}
void Server::start(){
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    store.set("name","Mark");
    if(serverSocket==-1){
        std::cerr << "Socket creation failed" << strerror(errno) << std::endl;
        return;
    }else{
        std::cout << "Socket created successfully " << " FD is " << serverSocket << std::endl;
    }
    int opt = 1;
    if(setsockopt(serverSocket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt))==-1){
            perror("setsockopt");
            close(serverSocket);
            serverSocket = -1;
            return;
    }
    sockaddr_in serverAddress{};//creating the struct for the address or the generic addresss
    serverAddress.sin_family = AF_INET;//    //the bind function has to include the sin_family in order to allow linux to know what type of address strcture the address strcture in this one is ipv4
    serverAddress.sin_port = htons(port);//converting/setting the port number to be in big endian
    serverAddress.sin_addr.s_addr = INADDR_ANY;//listen to any sort of connection or allowed to receive data from any sort of connection
    //htons convert the port to begendian notation
    //inaddr_any allows us to recieve data from any sort of connection whether its wifi or ethernet
    //bind needs a pointer to sockaddr as it is a generic address rather than a specific one every specfiic one like ipv4 or 6 or bluetooth has to be converted to a generic one
    //reiterpret case doesnt change the memory but changes how the compirer interpts the pointer
    //im telling the compirer that they are compatible
    bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
    if(listen(serverSocket,10)==-1){
        perror("Unable to listen to connections or unable to connect to clients");
        close(serverSocket);
        return;
    }
    std::cout << "Server listening on port " << port << std::endl;
    bool ClientConnected=true;
    while(true){
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if(clientSocket==-1){
            perror("accept");
            continue;
        }
        std::cout << "Client connected" << std::endl;
        std::string receivedBuffer;
        while(ClientConnected){
        char buffer[4096];
        ssize_t bytesReceived=recv(clientSocket,buffer,sizeof(buffer),0);
        std::cout << '\n';
        std::vector<std::string>command={};
        std::cout  << "Client connection established"<< std::endl;
        switch(bytesReceived){
        case -1:
        perror("recv");
        break;
        case 0:
        std::cout <<"Client connection closed " << std::endl;
        close(serverSocket);
        ClientConnected=false;
        break;
        default:
        receivedBuffer.append(buffer,bytesReceived);
        std::cout << "Buffer currently contains " << receivedBuffer << "\n";
        while(!receivedBuffer.empty()){
            try{
                ParseResult result=parser.parse(receivedBuffer);
                std::cout << "Server: "<< result.command.size() << "arguments\n";
                for(const auto& arg: result.command){
                    std::cout << "SEVER" << arg << "]\n";
                }
                std::string response= handler.execute(result.command);
                receivedBuffer.erase(0,result.bytesConsumed);
                 sendAll(clientSocket,response);
            }catch(const IncompleteData&){
                break;
            }catch(const std::exception& error){
                std::cerr << "Protocal error: "<<error.what() << "\n";
                receivedBuffer.clear();
                break;
            }
            }
        }
    }
        close(clientSocket);
    }
}
//the deconstrctor will eventually release the socket if one is created
Server::~Server(){

}