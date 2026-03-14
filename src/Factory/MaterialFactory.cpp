#include <libconfig.h++>
#include <memory>
#include <map>
#include <functional>

#include "Scene.hpp"

using MaterialCtor = std::function<std::unique_ptr<AMaterial>(const libconfig::Setting &s)>;

std::map<std::string, MaterialCtor> materialRegistry;

MaterialFactory::MaterialFactory()
{
    materialRegistry["mirror"] = [](const libconfig::Setting& s) {
        return std::make_unique<Mirror>(s);
    };
    materialRegistry["chrome"] = [](const libconfig::Setting& s) {
        return std::make_unique<Chrome>(s);
    };
    materialRegistry["default"] = [](const libconfig::Setting& s) {
        return std::make_unique<Default>(s);
    };
}

#define MATERIAL    "material"
#define TEXTURE     "texture"

TextureManager AMaterial::textureManager;

std::unique_ptr<AMaterial> MaterialFactory::GetMaterial(const libconfig::Setting &s)
    {
        
        int textureIndex = -1;
        std::string type;
        std::unique_ptr<AMaterial> aMaterial;

        if (s.exists(MATERIAL)) {
            const libconfig::Setting &s1 = s[MATERIAL];
            Log::Logger::debug("Material FOUND");
            if (!s1.exists("type"))
                throw std::invalid_argument("Need type to implement Type");
            Log::Logger::debug("Type FOUND");
            type = (std::string)s1["type"];
            Log::Logger::debug("Type load");
            if (s1.exists(TEXTURE))
                textureIndex = this->textureManager.uploadTexture(s1[TEXTURE]);
        }
        else
            type = "default";
        
        auto fcntMaterial = materialRegistry.at(type);

        if (fcntMaterial == nullptr)
            throw std::invalid_argument("Type: " + type + " didn't exist.");

        if (type == "default")
            aMaterial = fcntMaterial(s);
        else
            aMaterial = fcntMaterial(s[MATERIAL]);
        aMaterial->textureIndex = textureIndex;
        Log::Logger::info("IndexTexture: " + std::to_string(textureIndex));
        if (textureIndex == -1)
            return aMaterial;
        AMaterial::textureManager = this->textureManager;
        return aMaterial;
    }