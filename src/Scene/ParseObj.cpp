#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <libconfig.h++>
#include <sstream>
#include <string>

#include "Chrome.hpp"
#include "Default.hpp"
#include "Scene.hpp"
#include "Triangle.hpp"

using namespace libconfig;

namespace object {}
namespace shape {}
namespace space {}
namespace material {}
namespace light {}
namespace scene {}
namespace render {}
namespace graphical {}

using namespace object;
using namespace space;
using namespace light;
using namespace material;
using namespace shape;

void scene::Obj::parseMtl(const std::string &path) {
  std::filesystem::path objPath(path);
  std::filesystem::path mtlPath = objPath;
  mtlPath.replace_extension(".mtl");

  Log::Logger::info("Trying to load MTL: " + mtlPath.string());

  std::ifstream file(mtlPath);
  if (!file.is_open()) {
    Log::Logger::warning("No MTL found or cannot open: " + mtlPath.string());
    return;
  }

  std::string line;
  std::string currentMaterial = "";
  Material currentMatObj;

  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string type;
    iss >> type;

    if (type == "newmtl") {
      if (!currentMaterial.empty()) {
        _materialRegistry.uploadMaterial(currentMaterial, currentMatObj);
      }

      iss >> currentMaterial;
      currentMatObj = Material();
      currentMatObj.type = currentMaterial;
      currentMatObj.color = {255, 0, 255};
      currentMatObj.metallic = 0.0f;
      currentMatObj.roughness = 0.5f;
      currentMatObj.ior = 1.5f;
      currentMatObj.transmission = 0.0f;
      currentMatObj.textureType = TextureType::NONE;
      currentMatObj.textureIndex = -1;
    } else if (type == "Kd" && !currentMaterial.empty()) {
      float r, g, b;
      iss >> r >> g >> b;
      currentMatObj.color = {r * 255.0f, g * 255.0f, b * 255.0f};
    } else if (type == "Ns" && !currentMaterial.empty()) {
      float ns;
      iss >> ns;
      currentMatObj.roughness = std::sqrt(2.0f / (ns + 2.0f));
    } else if (type == "d" && !currentMaterial.empty()) {
      float d;
      iss >> d;
      currentMatObj.transmission = 1.0f - d;
    } else if (type == "Ka" && !currentMaterial.empty()) {
      float r, g, b;
      iss >> r >> g >> b;
      if (r > 0.5f || g > 0.5f || b > 0.5f) {
        currentMatObj.color = currentMatObj.color * 5.0f;
      }
    } else if (type == "map_Kd" && !currentMaterial.empty()) {
      std::string texPath;
      std::getline(iss >> std::ws, texPath);
      if (!texPath.empty() && texPath.back() == '\r') {
        texPath.pop_back();
      }

      std::replace(texPath.begin(), texPath.end(), '\\', '/');

      std::filesystem::path rawTexPath(texPath);
      std::filesystem::path fileName = rawTexPath.filename();
      std::filesystem::path parentDir = objPath.parent_path();

      std::filesystem::path finalTexPath = parentDir / fileName;

      if (!std::filesystem::exists(finalTexPath)) {
        std::filesystem::path tryTextures = parentDir / "textures" / fileName;
        if (std::filesystem::exists(tryTextures)) {
          finalTexPath = tryTextures;
        } else {
          std::filesystem::path tryTexturesCap =
              parentDir / "Textures" / fileName;
          if (std::filesystem::exists(tryTexturesCap)) {
            finalTexPath = tryTexturesCap;
          }
        }
      }
      int texIndex =
          Material::textureManager.uploadTexture(finalTexPath.string());
      if (texIndex != -1) {
        currentMatObj.textureType = TextureType::LOAD_IMAGE;
        currentMatObj.textureIndex = texIndex;
        Log::Logger::debug("Loaded texture for " + currentMaterial + ": " +
                           finalTexPath.string());
      } else {
        Log::Logger::warning("Failed to load texture: " +
                             finalTexPath.string());
      }
    }
  }
  if (!currentMaterial.empty()) {
    _materialRegistry.uploadMaterial(currentMaterial, currentMatObj);
  }
  Log::Logger::info("MTL Parsing done.");
}

scene::Obj::Obj(const std::string &path, const libconfig::Setting &s) {
  Vec3 pos = s.exists("pos") ? scene::readVec3(s["pos"]) : Vec3(0, 0, 0);
  Vec3 rot = s.exists("rot") ? scene::readVec3(s["rot"]) : Vec3(0, 0, 0);
  Vec3 scale = s.exists("scale") ? scene::readVec3(s["scale"]) : Vec3(1, 1, 1);

  float rx = DEG_TO_RAD(rot.x);
  float ry = DEG_TO_RAD(rot.y);
  float rz = DEG_TO_RAD(rot.z);

  parseMtl(path);

  std::ifstream file(path);
  if (!file.is_open()) {
    Log::Logger::warning("Error with the path obj:" + path);
    return;
  }
  std::string line;
  std::string currentMaterialName = "";

  // Struct pour lire proprement v/vt/vn
  struct VertexIndex {
    int v = -1;
    int vt = -1;
    int vn = -1;
  };

  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string type;
    iss >> type;
    if (type == "usemtl") {
      iss >> currentMaterialName;
    } else if (type == "v") {
      float x, y, z;
      iss >> x >> y >> z;
      Vec3 v(x, y, z);

      v.x *= scale.x;
      v.y *= scale.y;
      v.z *= scale.z;

      float tempX = v.x * cos(ry) + v.z * sin(ry);
      float tempZ = -v.x * sin(ry) + v.z * cos(ry);
      v.x = tempX;
      v.z = tempZ;

      float tempY = v.y * cos(rx) - v.z * sin(rx);
      tempZ = v.y * sin(rx) + v.z * cos(rx);
      v.y = tempY;
      v.z = tempZ;

      tempX = v.x * cos(rz) - v.y * sin(rz);
      tempY = v.x * sin(rz) + v.y * cos(rz);
      v.x = tempX;
      v.y = tempY;

      v = v + pos;
      _vertices.push_back(v);
    } else if (type == "vt") {
      float u = 0.0f, v = 0.0f;
      iss >> u >> v;
      _uvs.push_back({u, v});
    } else if (type == "f") {
      std::vector<VertexIndex> faceIndex;
      std::string vertexStr;

      // Décodage du face token `v/vt/vn`
      auto getIndex = [](const std::string &vStr) -> VertexIndex {
        VertexIndex vi;
        std::istringstream stream(vStr);
        std::string val;
        int part = 0;
        while (std::getline(stream, val, '/')) {
          if (!val.empty()) {
            try {
              if (part == 0) vi.v = std::stoi(val) - 1;
              else if (part == 1) vi.vt = std::stoi(val) - 1;
              else if (part == 2) vi.vn = std::stoi(val) - 1;
            } catch (...) {}
          }
          part++;
        }
        return vi;
      };

      while (iss >> vertexStr) {
        faceIndex.push_back(getIndex(vertexStr));
      }

      if (faceIndex.size() >= 3) {
        for (size_t i = 1; i < faceIndex.size() - 1; ++i) {
          VertexIndex i1 = faceIndex[0];
          VertexIndex i2 = faceIndex[i];
          VertexIndex i3 = faceIndex[i + 1];

          if (i1.v >= 0 && i2.v >= 0 && i3.v >= 0 &&
              i1.v < (int)_vertices.size() && i2.v < (int)_vertices.size() && i3.v < (int)_vertices.size()) {

            Vec2 uv1 = (i1.vt >= 0 && i1.vt < (int)_uvs.size()) ? _uvs[i1.vt] : Vec2{0.0f, 0.0f};
            Vec2 uv2 = (i2.vt >= 0 && i2.vt < (int)_uvs.size()) ? _uvs[i2.vt] : Vec2{0.0f, 0.0f};
            Vec2 uv3 = (i3.vt >= 0 && i3.vt < (int)_uvs.size()) ? _uvs[i3.vt] : Vec2{0.0f, 0.0f};

            // ATTENTION: Toujours passer les UVs dans le même ordre que les sommets (qui sont inversés i3/i2 par l'auteur)
            std::unique_ptr<IShape> shape = std::make_unique<Triangle>(
                _vertices[i1.v], _vertices[i3.v], _vertices[i2.v],
                uv1, uv3, uv2);

            std::unique_ptr<Material> material = std::make_unique<Material>(
                _materialRegistry.getMaterial(currentMaterialName));

            std::unique_ptr<Object> obj =
                std::make_unique<Object>(std::move(shape), std::move(material));

            _objects.push_back(std::move(obj));

          } else {
            Log::Logger::warning("Invalid face: index out of range");
          }
        }
      } else {
        Log::Logger::warning("Face skip");
      }
    }
  }
}

void scene::Scene::insertObjInObjects(
    const Setting &s, std::vector<std::unique_ptr<Object>> &objects) {
  if (!s.exists("obj")) {
    Log::Logger::warning("No Obj");
    return;
  }

  Log::Logger::info("Parse Obj");
  const Setting &objList = s["obj"];
  if (!objList.isList() && !objList.isArray()) {
    throw std::runtime_error("'obj' must be a list or an array");
  }

  int count = objList.getLength();
  Log::Logger::debug("Obj files to load: " + std::to_string(count));

  for (int i = 0; i < count; i++) {
    const Setting &currentObjNode = objList[i];
    std::string path;

    if (currentObjNode.lookupValue("path", path)) {
      Log::Logger::info("Loading obj: " + path);
      scene::Obj parsedObj(path, currentObjNode);

      for (auto &obj_ptr : parsedObj.getObjects())
        objects.push_back(std::move(obj_ptr));
    } else {
      throw std::runtime_error("no path set");
    }
  }
  Log::Logger::info("End of parsing Obj");
}