#include "ParserCmd.hpp"
#include "CmdConfig.hpp"
#include "logger.hpp"

namespace ParserCmd {

    void Parser::InitParser(void)
    {
        Log::Logger::info("start init parser");
        bindField(config.LogLvl, "log");
        Log::Logger::info("end init parser");
    }
}