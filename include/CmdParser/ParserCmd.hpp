#pragma once

#include <iostream>
#include <map>
#include <functional>

#include "CmdConfig.hpp"

namespace ParserCmd {

    class Parser
    {
        public:
            Parser(int ac, char **av);
            void buildConfig(void);
            void initParser(void);
            const CmdConfig::config_t &getConfig() const;

        private:
            std::vector<std::string> CmdArg;

            std::map<std::string, std::function<void(std::string)>> bind;

            template<typename T>
            void bindField(T& target, const std::string& name);
            CmdConfig::config_t config;
    };
}

#include "BindField.tpp"