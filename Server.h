#pragma once
#include "RedisStore.h"
#include "CommandHandler.h"
#include "RespParser.h"
class Server
{
private:
    int port;         // in order for clients to knwo where to connect
    int serverSocket; // the description that belongs to the socket
    RedisStore store;
    RespParser parser;
    CommandHandler handler;
    void sendAll(int clientSocket, const std::string& response);
public:
    Server(int portno);
    ~Server();
    void start();
};