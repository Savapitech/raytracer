#pragma once

#include <iostream>
#include <map>
#include <functional>
#include <any>




namespace Plugin {

    struct IPlugin
    {
        std::function<std::any()> create;
    };

    class LoadedPlugin
    {
        public:
        
            LoadedPlugin(const std::string &path);
            void *handle;
            IPlugin *FuncPlugin;
            std::string PluginName;
            ~LoadedPlugin();
    };
};