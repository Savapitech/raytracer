#include "PluginManager.hpp"
#include "LoadedPlugin.hpp"
#include <vector>
#include <dlfcn.h>
#include <stdexcept>


namespace Plugin {
    
    struct IPlugin
    {
        std::function<std::any()> create;
    };

    void PluginManager::PluginLoader(void)
    {
        std::string PluginName;
        PluginName = "IShape";
        PluginMap[PluginName] = LoadedPlugin(PluginName + ".so");
    }

    LoadedPlugin::LoadedPlugin(const std::string &path)
        : PluginName(path)
    {
        this->handle = dlopen(path.data(), RTLD_NOW);
        if (!handle) {
            std::cerr << "dlopen error: " << dlerror() << "\n";
            throw("Dlopen Error");
        }
        auto my_plugin = (IPlugin *(*)()) dlsym(handle, "getIPluginDescriptor");
        if (!my_plugin) {
            std::cerr << "dlsym error: " << dlerror() << "\n";
            throw("dlsym error:");
        }
        this->FuncPlugin = my_plugin();
    };

    LoadedPlugin::~LoadedPlugin(){
        dlclose(handle);
    }
}