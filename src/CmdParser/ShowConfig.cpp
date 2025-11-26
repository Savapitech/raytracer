#include "ParserCmd.hpp"
#include "CmdConfig.hpp"
#include "logger.hpp"

namespace ParserCmd {

    void Parser::ShowConfig(void)
    {
        Log::Logger::logVar(config.LogLvl);
    }
}