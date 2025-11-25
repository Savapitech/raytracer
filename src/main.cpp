/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** raytracer
*/
#include <iostream>
#include "logger.hpp"

int main(void)
{
    std::cout << "start..." << std::endl;

    Log::Logger::SetLogLvl(Log::Logger::INFO);

    Log::Logger::info("Test de l'info test");
}