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
    ParserCmd::Parser parser(ac, av);

    parser.InitParser();
    parser.BuildConfig();
}