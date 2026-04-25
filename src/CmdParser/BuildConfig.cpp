#include "ParserCmd.hpp"
#include "CmdConfig.hpp"
#include "logger.hpp"

#include <map>

namespace ParserCmd 
{

    Parser::Parser(int argc, char **argv)
    {
        std::string cmd;
        std::string value;
        size_t pos;

        for (int i = 1; i < argc; i++) { 
            cmd = argv[i];
            if (cmd.rfind("--", 0) == 0)
                cmd.erase(0, 2);
            else if (cmd.rfind("-", 0) == 0)
                cmd.erase(0, 1);
            else {
                _cmdArg.push_back(cmd);
                continue;
            }
            pos = cmd.find('=');
            if (pos == std::string::npos) {
                _cmdArg.push_back(cmd);
                continue;
            }
            value = cmd.substr(pos + 1);
            cmd.erase(pos);
            _cmdArg.push_back(cmd);
            _cmdArg.push_back(value);       
        }
        for (std::string str : _cmdArg){
            Log::Logger::debug("List Arg: " + str);
        }
    }

    void Parser::initParser(void)
    {
        Log::Logger::info("start init parser");
        bindField(_config.LogLvl, "log");
        bindField(_config.scene, "scene");
        bindField(_config.pathtracing, "pathTracing");
        bindField(_config.sample, "sample");
        Log::Logger::info("end init parser");
    }

    void Parser::buildConfig(void)
    {
        std::string arg;
        std::string value;

        if (_cmdArg.size() % 2 != 0)
            throw std::invalid_argument("_cmdArg odd size");
        for (size_t i = 0; i < _cmdArg.size(); i += 2){
            arg = _cmdArg[i];
            value = _cmdArg[i + 1];
            if (_bind.count(arg)){
                Log::Logger::debug("Bind Find Arg: " + arg + " With Value: " + value);
                _bind[arg](value);
            }
            else {
                Log::Logger::debug("Didn't find Bind: " + arg + " With Value: " + value);
            }
        }
        Log::Logger::SetLogLvlStr(_config.LogLvl);
    }

    const CmdConfig::config_t &Parser::getConfig() const
    {
        return _config;
    }
}