#include "RedisStore.h"
#include <iostream>
#include <sstream>
#include <vector>
RedisStore::RedisStore():persistanceFile("redislog.txt",std::ios::in|std::ios::out|std::ios::app){
    std::string command,key,value;
    std::string fullcommand;
    while(getline(persistanceFile,fullcommand)){
        std::stringstream fullcommandss(fullcommand);
        fullcommandss>>command>>key>>value;
        if(command=="DEL"){
            if(del(key)){
                std::cout << "able to delete " << key <<std::endl;
            };
        }
    };        
}
void RedisStore::append(const std::string& command){
    persistanceFile.clear();
    if(persistanceFile.is_open()){
        std::cout << "File is open" <<std::endl;
    }else{
        std::cout << "Unable to open the file" <<std::endl;
    }
    persistanceFile << command << std::endl;
    std::cout << "Append end" << std::endl;
}
void RedisStore::set(const std::string& key, const std::string& value){
     data[key]=value;
}

bool RedisStore::get(const std::string key,std::string& value){
    auto it=data.find(key);
    if(it==data.end()){
        return false;
    }
    value=it->second;
    return true;
}
bool RedisStore::del(const std::string& key){
    return data.erase(key)>0;  
}
bool RedisStore::exists(const std::string& key){
    return data.find(key)!=data.end();
}

