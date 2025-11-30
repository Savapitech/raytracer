#include "ParserCmd.hpp"
#include "CmdConfig.hpp"
#include "logger.hpp"

#include <map>

namespace ParserCmd {

    void Parser::BuildConfig(void)
    {
        std::string Arg;
        std::string Value;
        if (CmdArg.size() % 2 != 0)
            throw std::invalid_argument("CmdArg odd size");
        for (size_t i = 0; i < CmdArg.size(); i += 2){
            Arg = CmdArg[i];
            Value = CmdArg[i + 1];
            if (bind.count(Arg)){
                Log::Logger::debug("Bind Find Arg: " + Arg + " With Value: " + Value);
                bind[Arg](Value);
            }
            else {
                Log::Logger::debug("Didn't find Bind: " + Arg + " With Value: " + Value);
            }
        }
        Log::Logger::SetLogLvlStr(config.LogLvl);
    }
}