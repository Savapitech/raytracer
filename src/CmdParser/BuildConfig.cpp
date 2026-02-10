#include "ParserCmd.hpp"
#include "CmdConfig.hpp"
#include "logger.hpp"

#include <map>

namespace ParserCmd 
{

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

    void Parser::initParser(void)
    {
        Log::Logger::info("start init parser");
        bindField(config.LogLvl, "log");
        bindField(config.scene, "scene");
        bindField(config.pathtracing, "pathtracing");
        Log::Logger::info("end init parser");
    }

    void Parser::buildConfig(void)
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

    const CmdConfig::config_t &Parser::getConfig() const
    {
        return this->config;
    }
}