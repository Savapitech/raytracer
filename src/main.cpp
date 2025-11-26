/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** raytracer
*/
#include <iostream>
#include "logger.hpp"
#include "ParserCmd.hpp"

int main(int ac, char **av)
{
    std::cout << "start..." << std::endl;
    Log::Logger::SetLogLvl(Log::Logger::DEBUG);

    try
    {
        ParserCmd::Parser parser(ac, av);

        parser.InitParser();
        parser.BuildConfig();
    }
    catch(const std::exception& e)
    {
        Log::Logger::error(e.what());
        Log::Logger::info("init parser failed.");
    }
}