#pragma once

class LightFactory
{
    public:
        LightFactory();
        std::unique_ptr<ILight> getLight(const libconfig::Setting &s);
};
