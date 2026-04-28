#pragma once

#include <map>
#include <functional>

#include "CmdConfig.hpp"

namespace ParserCmd {

    class Parser // Classe Parser, sert à parser les arguments de la ligne de commande.
    {
        public:
            Parser(int ac, char **av);
            void buildConfig(void);
            void initParser(void);
            const CmdConfig::config_t &getConfig() const;

        private:
            std::vector<std::string> _cmdArg; // std::vector de string CmdArg, contient les arguments des commandes.

            std::map<std::string, std::function<void(std::string)>> _bind; // std::map qui bind une string (instruction) à une fonction.

            template<typename T>
            void bindField(T& target, const std::string& name); // méthode bindField, qui fill la std::map ci-dessus.
            CmdConfig::config_t _config;
    };
}
void gui();
#include "BindField.tpp"