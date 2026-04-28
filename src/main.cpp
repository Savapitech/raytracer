/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** raytracer
*/
#include "logger.hpp"
#include "ParserCmd.hpp"
#include "Ray.hpp"
#include "Object.hpp"

int main(int ac, char **av)
{
    Log::Logger::SetLogLvl(Log::Logger::DEBUG);
    Log::Logger::info("start...");

    /*====Prep the Parser for the main command from argv====*/
    ParserCmd::Parser parser(ac, av);

    try {
        /*===Parse the command from argv===*/
        parser.initParser();
        parser.buildConfig();

        if (parser.getConfig().gui == true)
            gui();

        /*===Init the main class who init the scene and the render===*/
        RayTracer::RayTracer raytracer(parser.getConfig());

        /*===Run the ray tracing render===*/
        raytracer.run();
    }
    catch(const std::exception& e) {
        Log::Logger::error(e.what());
    }    
}