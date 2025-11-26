#pragma once

#include <iostream>
#include <map>
#include <functional>
#include <any>
#include "LoadedPlugin.hpp"

namespace Plugin {

    
    template <typename FuncType, typename ReturnType>
    struct CastPair {
        using func = FuncType;
        using ret  = ReturnType;
    };

    using ObjetC = CastPair<char(*)(void), int>;
    
    class PluginManager
    {
        void PluginLoader(void);
        
        template<typename TCast>
        auto acces(std::string str)
        {
            using T  = typename TCast::func;
            using RT = typename TCast::ret;

            auto func = create_any<T>(str);
            return func();
        }
    private:        
        std::map<std::string, LoadedPlugin> PluginMap;
        
        template<typename T>
        T create_any(const std::string& key)
        {
            return std::any_cast<T>(PluginMap[key].create());
        }
    };
}