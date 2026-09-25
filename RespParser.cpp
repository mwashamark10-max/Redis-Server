#include "RespParser.h"
ParseResult RespParser::parse(const std::string& input){
        std::vector<std::string> command;
        std::size_t position=0;//since LineEnd stores the position of the number of elements in the object and \r\n are after ti the string object starts from lineEnd+2
        std::size_t lineEnd = input.find("\r\n",position);//find the first instance of \r\n and return the position e.g 2 as for this instace the input string is *1\r\n 
        if(input.empty()){
            throw std::runtime_error("Empty RESP input");
        }
        if(input[0]!='*'){
            std::cout << "Error1";
            throw std::runtime_error("Expected RESP array");
        }
        if(lineEnd==std::string::npos){
            throw IncompleteData("Incomplete RESP array header");
        }
        std::string countString=input.substr(position+1,lineEnd-position-1);//substr(1,1) returns the substr thats is inclusive of that letter the number of character starting from it so one letter from 1 which is one
        int argumentCount=std::stoi(countString);//converted it to an integer one
        position=lineEnd+2;
        for(ssize_t i=0;i<argumentCount;i++){
        if(position>=input.size()||input[position]!='$'){//the string object starts from here and it starts with $ position should be at begiing of RESP string compared with size as imagine were told 3 arguments and isnt another byte available wed compare outside the string input[psotioni] would be an invalid memory address
            throw std::runtime_error("Expected RESP bulk string");
        }
        std::cout << "FIRST BYTES";
        for(int i=0;i<20&&i<input.size();i++){
            if(input[i]=='\r'){
                std::cout << "\\r";
            }else if(input[i]=='\n'){
                std::cout << "\\n";
            }else std::cout << input[i];
        }
        std::cout << "\n";
        std::size_t bulkHeaderEnd=input.find("\r\n",position);//this finds the instance of \r\n after position which is four and stores where the string object ends which is *1\r\n$4\r\n
        if(bulkHeaderEnd==std::string::npos){
            throw IncompleteData("Incomplete bulk string header");
        }//there should always be an r\n infront of the string as each ends with r/n if we recieved $3 instead of $3\r\n there was an error 
        std::string length=input.substr(position+1,bulkHeaderEnd-position-1);//its starts from the 5th character and is 6-4-1 which is one character
        int bulkLength=std::stoi(length);//the actual length of the first string
        std::size_t dataStart=bulkHeaderEnd+2;//where the actual data starts
        if(dataStart+bulkLength>input.size()){
            throw IncompleteData("Incomplete bulk string");//checking that weve recieved all the data supposed were told its 10 byts we recieved 5 dataStart+bulkLength would be greater than input.size() hence we dont have enough info for the bulk string
        }
        std::string value = input.substr(dataStart,bulkLength);
        command.push_back(value);
        position=dataStart+bulkLength;//datastary points to the start of each data value like S as its position+2 and bulklength is the length of the string so i should be r\n should be the next r\n
        if(position+2>input.size()){
            throw IncompleteData("Incomplete CRLF after bulk string");
        }
        if(input.substr(position,2)!="\r\n"){
            throw std::runtime_error("Missing CRLF after bulk string");
        }//making sure we have two bytes for r/n and are the next two bytes actually r\n
        position+=2;
    }
    //*3\r\n array header the parse fins $ find r\n reads the length jumps past r\n read length of bytses verifies another r\n and then moves to the next argument
    std::cout << "Argument count: "<<argumentCount << "\n";
    std:: cout << "Command\n";
    for(const auto&arg:command){
        std::cout << "[" <<arg << "]\n";
    }
    std:: cout << "Position: "<< position << "\n";
        return {
            command,
            position
        };
}