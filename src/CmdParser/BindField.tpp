#pragma once

#include "ParserCmd.hpp"
#include "CmdConfig.hpp"
#include "logger.hpp"

namespace ParserCmd {

    template<typename T>
    void Parser::bindField(T& target, const std::string& name)
    {
        if constexpr (std::is_same_v<T, int>){
            Log::Logger::debug("bind int: " + name);
            _bind[name] = [&](const std::string& value){
                target = std::stoi(value);
            };
        }
        else if constexpr (std::is_same_v<T, bool>){
            Log::Logger::debug("bind bool: " + name);
            _bind[name] = [&](const std::string& value){
                if (value != "true" && value != "false")
                    throw std::invalid_argument("Error: Bool");
                target = (value == "true");
            };
        }
        else if constexpr (std::is_same_v<T, std::string>){
            Log::Logger::debug("bind string: " + name);
            _bind[name] = [&](const std::string& value){
                target = value;
            };
        }
        else {
            Log::Logger::error("Bind Field: Error Type");
        }
    }
}
