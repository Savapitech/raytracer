#include "ParserCmd.hpp"
#include "CmdConfig.hpp"
#include "logger.hpp"

namespace ParserCmd {

    void Parser::InitParser(void)
    {
        Log::Logger::info("start init parser");
        bindField(config.LogLvl, "log");
        bindField(config.scene, "scene");
        bindField(config.pathtracing, "pathtracing");
        Log::Logger::info("end init parser");
    }
}