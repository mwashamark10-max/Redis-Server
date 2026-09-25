#include "CommandHandler.h"
#include <iostream>
#include <string>
#include <algorithm>
#include "RedisStore.h"
CommandHandler::CommandHandler(RedisStore& store):store(store){

}
std::string CommandHandler::execute(const std::vector<std::string>& command){//were passing a vector by reference so we dont need to modify it
    if(command.empty()){
        return "-Err empty command\r\n";
    }
    std::cout << "Handler recieved " << command.size() << "arguments:\n";
    for(const auto& arg:command){
        std::cout << "[" <<arg << "]\n";
    }
    std::string requestName=command[0];
    std::string fullcommand="";      
    store.append(fullcommand);
    std::transform(requestName.begin(),requestName.end(),requestName.begin(),[](unsigned char c){
        return std::toupper(c);
    });
    if(requestName=="PING"){
        return "+PONG\r\n";
    }
    if(requestName=="SET"){
        if(command.size()!=3){
            return "-ERR wrong number of arguments for 'set' command\r\n";
        }
         for(std::string word:command){
        fullcommand+=word;
        fullcommand+=" ";
    }   store.append(fullcommand);
     std::cout << "Were in the Command Handler file the fullcommand is " << fullcommand << "\n";
        store.set(command[1],command[2]);
        return "+OK\r\n";
    }
    if(requestName=="GET"){
        if(command.size()!=2){
            return "-ERR wrong number of arguments for 'get' command\r\n";
        }
        std::string value;
        if(!store.get(command[1],value)){//store ! returns a bool true if it does and not if it doesnt
            return "$-1\r\n";
        }
        return "$"+std::to_string(value.size())+"\r\n"+value+"\r\n";
    }
    if(requestName=="EXISTS"){
        if(command.size()!=2){
           return "-ERR wrong number of arguments for 'exists' command\r\n"; 
        }
        if(store.exists(command[1])){
            return ":1\r\n";
        }
        return "0\r\n";

    }
    if(requestName=="DELETE"){
        if(command.size()!=2){
            return "-ERR wrong number of arguments for DEL\r\n";
        }
        for(std::string word:command){
        fullcommand+=word;
        fullcommand+=" ";
    }  
         std::cout << "Were in the Command Handler file the fullcommand is " << fullcommand << "\n"; 
        store.append(fullcommand);
        if(store.del(command[1])){
            return ":1\r\n";
        }
        return ":0\r\n";
    }
    return "-ERR unknown command\r\n";
}

