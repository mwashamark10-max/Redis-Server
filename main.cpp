#include <iostream>
#include <vector>
#include "Server.h"
#include "RespParser.h"
#include "RedisStore.h"
#include "CommandHandler.h"
int main(){
    Server server(6379);
    server.start();
    return 0;
}