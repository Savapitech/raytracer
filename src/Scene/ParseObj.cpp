#include <libconfig.h++>
#include <fstream>
#include <sstream>
#include <string>


#include "Scene.hpp"
#include "Triangle.hpp"
#include "Chrome.hpp"
#include "Default.hpp"

using namespace libconfig;

scene::Obj::Obj(std::string path, const libconfig::Setting &s)
{
    Vec3 pos = s.exists("pos") ? scene::readVec3(s["pos"]) : Vec3(0, 0, 0);
    Vec3 rot = s.exists("rot") ? scene::readVec3(s["rot"]) : Vec3(0, 0, 0);
    Vec3 scale = s.exists("scale") ? scene::readVec3(s["scale"]) : Vec3(1, 1, 1);

    float rx = DEG_TO_RAD(rot.x);
    float ry = DEG_TO_RAD(rot.y);
    float rz = DEG_TO_RAD(rot.z);

    std::ifstream file(path);
    if (!file.is_open()) {
        Log::Logger::warning("Error with the path obj:" + path);
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
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
        } else if (type == "f") {
            std::vector<int> faceIndex;
            std::string vertexStr;

            auto getIndex = [](const std::string& vStr) {
                size_t pos = vStr.find('/');
                if (pos != std::string::npos) {
                    return std::stoi(vStr.substr(0, pos)) - 1;
                }
                return std::stoi(vStr) - 1;
            };

            while (iss >> vertexStr) {
                try {
                    faceIndex.push_back(getIndex(vertexStr));
                } catch(const std::exception& e) {
                    Log::Logger::warning("stoi error with vertex");
                }
            }
            if (faceIndex.size() >= 3) {
                for (size_t i = 1; i < faceIndex.size() - 1; ++i) {
                    size_t i1 = faceIndex[0];
                    size_t i2 = faceIndex[i];
                    size_t i3 = faceIndex[i + 1];

                    if (i1 >= 0 && i2 >= 0 && i3 >= 0 &&
                        i1 < _vertices.size() && i2 < _vertices.size() && i3 < _vertices.size()) {

                        std::unique_ptr<IShape> shape = std::make_unique<Triangle>(_vertices[i1], _vertices[i3], _vertices[i2]);
                        std::unique_ptr<Material> material = std::make_unique<Default>();
                        shape->setColor({255, 0, 255});
                        std::unique_ptr<Object> obj = std::make_unique<Object>(std::move(shape), std::move(material));
                        
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

void scene::Scene::insertObjInObjects(const Setting &s, std::vector<std::unique_ptr<Object>> &objects)
{
    if (!s.exists("obj")){
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

            for (auto& obj_ptr : parsedObj.getObjects()) 
                objects.push_back(std::move(obj_ptr));
            } 
        else {
            throw std::runtime_error("no path set");
        }
    }
    Log::Logger::info("End of parsing Obj");
}