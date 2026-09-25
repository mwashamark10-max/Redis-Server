#ifndef Redis_Store
#define Redis_Store
#include <string>
#include <unordered_map>
#include <fstream>
class RedisStore{
    private:
    std::fstream persistanceFile;
    std::unordered_map<std::string,std::string> data;
    public:
    RedisStore();
    void set(const std::string& key, const std::string& value);
    bool get(const std::string key, std::string& value);
    bool del(const std::string& key);
    bool exists(const std::string& key);
    void append(const std::string& command);
};
#endif