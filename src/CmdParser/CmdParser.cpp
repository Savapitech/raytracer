#include "ParserCmd.hpp"
#include "CmdConfig.hpp"
#include "logger.hpp"

namespace ParserCmd {

    const CmdConfig::config_t &Parser::getConfig() const
    {
        return this->config;
    }
}