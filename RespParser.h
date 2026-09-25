#ifndef RESP_PARSER_H
#define RESP_PARSER_H
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <stdexcept>
class IncompleteData: public std::runtime_error{
    public:
    using std::runtime_error::runtime_error;
};
struct ParseResult{
    std::vector<std::string> command;
    std::size_t bytesConsumed;
};
class RespParser{
    public:
    ParseResult parse(const std::string& input);
};
#endif