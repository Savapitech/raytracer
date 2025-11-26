#include "ParserCmd.hpp"
#include "CmdConfig.hpp"
#include "logger.hpp"

namespace ParserCmd {

    Parser::Parser(int argc, char **argv)
    {
        std::string Cmd;
        std::string Value;
        size_t pos;

        for (int i = 1; i < argc; i++) { 
            Cmd = argv[i];
            if (Cmd.rfind("--", 0) == 0)
                Cmd.erase(0, 2);
            else if (Cmd.rfind("-", 0) == 0)
                Cmd.erase(0, 1);
            else {
                CmdArg.push_back(Cmd);
                continue;
            }
            pos = Cmd.find('=');
            if (pos == std::string::npos) {
                this->CmdArg.push_back(Cmd);
                continue;
            }
            Value = Cmd.substr(pos + 1);   
            Cmd.erase(pos);
            this->CmdArg.push_back(Cmd);
            this->CmdArg.push_back(Value);       
        }
        for (std::string str : CmdArg){
            Log::Logger::debug("List Arg: " + str);
        }
    }
}