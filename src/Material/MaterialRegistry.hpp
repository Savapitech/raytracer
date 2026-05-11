#pragma once

#include "IMaterials.hpp"
#include "map"

class MaterialRegistry {
public:
  MaterialRegistry() = default;
  ~MaterialRegistry() = default;

  void uploadMaterial(const std::string &materialName,
                      const Material &material);
  Material getMaterial(const std::string &materialName);

private:
  std::map<std::string, Material> _materialRegistry;
};
