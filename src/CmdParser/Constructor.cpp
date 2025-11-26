#include "ParserCmd.hpp"
#include "CmdConfig.hpp"
#include "logger.hpp"

namespace ParserCmd {

    /*Fill Vector CmdArg with parameter*/
    /*
    **If we got -- or - there will be remove
    **If we find = in the str we will cut the str in two at the frist =
    **
    */
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
                CmdArg.push_back(Cmd);
                continue;
            }
            Value = Cmd.substr(pos + 1);   
            Cmd.erase(pos);
            CmdArg.push_back(Cmd);
            CmdArg.push_back(Value);       
        }
        for (std::string str : CmdArg){
            Log::Logger::debug("List Arg: " + str);
        }
    }
}