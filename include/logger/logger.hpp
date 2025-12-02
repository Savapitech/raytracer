#pragma once

#include <iostream>

namespace Log {

    #define CLR_BOLD_INFO "\033[1;34m"
    #define CLR_BOLD_WARNING "\033[1;35m"
    #define CLR_BOLD_ERROR "\033[1;31m"
    #define CLR_BOLD_DEBUG "\033[1;32m"
    #define CLR_INFO "\033[0;34m"
    #define CLR_WARNING "\033[0;35m"
    #define CLR_ERROR "\033[0;31m"
    #define CLR_DEBUG "\033[0;32m"
    #define CLR_RESET "\033[0m"

    class Logger
    {
        public:
            enum LogLvl {
                NONE = 0,
                INFO = 1,
                WARNING = 2,
                ERROR = 3,
                DEBUG = 4
            };
            static void info(const std::string &str) noexcept;
            static void warning(const std::string &str) noexcept;
            static void error(const std::string &str) noexcept;
            static void debug(const std::string &str) noexcept;
            static void SetLogLvl(const LogLvl lvl) noexcept;
            static const LogLvl &GetLogLvl(void) noexcept {return LoggerLvl;}
            static void SetLogLvlStr(const std::string &lvl) noexcept;
            template<typename T>
            static void logVar(const T& var) noexcept {
                Logger::debug(std::to_string(var));
            }

        private:
            static LogLvl LoggerLvl;
            static void log_info(LogLvl lvl, const std::string &msg) noexcept;
    };
}