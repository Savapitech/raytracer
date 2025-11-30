#include "logger.hpp"

namespace Log{

    
    void Logger::log_info(Logger::LogLvl lvl, const std::string &msg) noexcept
    {
        if (lvl == Logger::NONE)
            return;
        if (lvl == Logger::INFO)
            std::cout << CLR_BOLD_INFO    << "Info: "    << CLR_INFO << msg << CLR_RESET << std::endl;
        if (lvl == Logger::WARNING)
            std::cout << CLR_BOLD_WARNING << "Warning: " << CLR_WARNING << msg << CLR_RESET << std::endl;
        if (lvl == Logger::ERROR)
            std::cout << CLR_BOLD_ERROR   << "Error: "   << CLR_ERROR << msg << CLR_RESET << std::endl;
        if (lvl == Logger::DEBUG)
            std::cout << CLR_BOLD_DEBUG   << "Debug: "   << CLR_DEBUG << msg << CLR_RESET << std::endl;
    }

    void Logger::info(const std::string &str) noexcept
    {
        if (Logger::LoggerLvl < Logger::INFO)
            return;
        log_info(LogLvl::INFO, str);
    }

    void Logger::warning(const std::string &str) noexcept
    {
        if (Logger::LoggerLvl < Logger::WARNING)
            return;
        log_info(LogLvl::WARNING, str);
    }

    void Logger::error(const std::string &str) noexcept
    {
        if (Logger::LoggerLvl < Logger::ERROR)
            return;
        log_info(LogLvl::ERROR, str);
    }

    void Logger::debug(const std::string &str) noexcept
    {
        if (Logger::LoggerLvl < Logger::DEBUG)
            return;
        log_info(LogLvl::DEBUG, str);
    }

    Log::Logger::LogLvl Logger::LoggerLvl = Logger::NONE;

    void Logger::SetLogLvl(const LogLvl lvl) noexcept
    {
        Logger::LoggerLvl = lvl;
    }

    void Logger::SetLogLvlStr(const std::string &lvl) noexcept
    {
        if (lvl == "NONE")
            Logger::LoggerLvl = LogLvl::NONE;
        else if (lvl == "INFO")
            Logger::LoggerLvl = LogLvl::INFO;
        else if (lvl == "WARNING")
            Logger::LoggerLvl = LogLvl::WARNING;
        else  if (lvl == "ERROR")
            Logger::LoggerLvl = LogLvl::ERROR;
        else if (lvl == "DEBUG")
            Logger::LoggerLvl= LogLvl::DEBUG;
        else 
            Logger::LoggerLvl= LogLvl::NONE;
    }
}