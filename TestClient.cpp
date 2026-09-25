#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <chrono>
int main(){
int testClientSocket=socket(AF_INET, SOCK_STREAM, 0);
if(testClientSocket==-1){
    perror("Unable to create socket");
    return 0;
}
sockaddr_in ServerAddress{};
ServerAddress.sin_family=AF_INET;
ServerAddress.sin_port=htons(6379);
int conversion=inet_pton(AF_INET,"127.0.0.1", &ServerAddress.sin_addr);
if(conversion<1){
    perror("Unable to convert address to bytes format");
    return 0;
}
int connection=connect(testClientSocket,reinterpret_cast<sockaddr*>(&ServerAddress),sizeof(ServerAddress));
if(connection==-1){
    perror("Unable to establish connection");
    return 0;
}
std::cout << "Connection successfull\n";
std::string response="*3\r\n$3\r\nSE";
ssize_t bytesSent=send(testClientSocket,response.c_str(),response.size(),0);
std::this_thread::sleep_for(std::chrono::seconds(2));
std::string secondpart ="T\r\n$4\r\nName\r\n$4\r\nMark\r\n";
bytesSent=send(testClientSocket,secondpart.c_str(),secondpart.size(),0);

}