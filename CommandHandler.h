#ifndef  Command_Handler
#define Command_Handler
#include <string>
#include <vector>
#include "RedisStore.h"

class CommandHandler{
    private:
    RedisStore& store;
    public:
      CommandHandler(RedisStore& store);
      std::string execute(const std::vector<std::string>& command);
};
#endif