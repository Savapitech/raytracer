#include "RayTracer.hpp"


namespace scene{
    void Camera::setupCam() {
        Vec3 worldUp(0,1,0);

        forward = normalize(dir);
        if (fabs(dot(forward, worldUp)) > 0.999f) {
            worldUp = Vec3(1,0,0);
        }
        right   = normalize(cross(worldUp, forward));
        up      = normalize(cross(forward, right));
    }   


    Camera readcam(const libconfig::Setting &s)
    {
        Camera camera;
        
        camera.fov = s.exists("fov") ? (float)s["fov"] : 90.0f;
        Log::Logger::info("Camera Fov set");
        if (!s.exists("dir"))
            throw std::runtime_error("Missing 'dir' field in scene");
        camera.dir = readVec3(s["dir"]);
        camera.dir = normalize(camera.dir);
        Log::Logger::info("Camera Dir set");
        if (!s.exists("pos"))
            throw std::runtime_error("Missing 'pos' field in scene");
        camera.pos = readVec3(s["pos"]);
        Log::Logger::info("Camera Pos set");
        camera.setupCam();
        return camera;
    }
}