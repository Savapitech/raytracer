#include "render.hpp"
#include "RayTracer.hpp"

void Render::writePixel(int x, int y, sf::Color color) noexcept
{
    int acutalPixel = (y * scene.getCamera().width + x) * 4;

    RayBuffer[R(acutalPixel)] = color.r;
    RayBuffer[G(acutalPixel)] = color.g;
    RayBuffer[B(acutalPixel)] = color.b;
    RayBuffer[A(acutalPixel)] = color.a;
}

Vec3 speculaire(const Vec3& V, const Vec3& N, const Vec3& L, float alpha) noexcept
{
    Vec3 lightColor(255, 255, 255);
    Vec3 R = normalize( N * (2.0f * dot(N, L)) - L );
    float specAngle = std::max(dot(R, V), 0.0f);
    float spec = std::pow(specAngle, alpha);
    return lightColor * spec;
}

bool Render::ShadowRay(Vec3 &light, Hit &minHit, Vec3 &P, Vec3 &L) noexcept
{
    Ray shadowRay(P + minHit.normal * 0.001f, L);
    shadowRay.minHit = 0.001f;
    shadowRay.maxHit = norm(light - P) - 0.001f;

    if (this->bvh.intersect(shadowRay, minHit) == true)
        return true;
    return false;
}

sf::Color Render::shade(Ray &ray, Hit &minHit) noexcept
{
    /*Vec3 light(6.4, -2.2, -28.54);*/
    Vec3 light(10, 0, 10);
    Vec3 colorShape = minHit.object->shape->color;

    Vec3 P = ray.origin + ray.dir * minHit.t;
    Vec3 L = normalize(light - P);
    Vec3 N = normalize(P - minHit.object->shape->pos);
    //if (ShadowRay(light, minHit, P, L) == true)
    //    return sf::Color::Black;

    float diff = std::max(dot(N, L), 0.0f);

    Vec3 spec = speculaire(normalize(-ray.dir), N, L, 50.f);
    
    Vec3 finalColor = spec + (colorShape * diff);

    finalColor.x = std::clamp(finalColor.x, 0.0f, 255.0f);
    finalColor.y = std::clamp(finalColor.y, 0.0f, 255.0f);
    finalColor.z = std::clamp(finalColor.z, 0.0f, 255.0f);

    return sf::Color(finalColor.x, finalColor.y, finalColor.z, 255);
}

void Render::FindObject(int x, int y) noexcept
{
    Ray ray(scene.getCamera(), x, y);
    Hit minHit;

    this->bvh.intersect(ray, minHit);
    if (minHit.object != nullptr) {
        sf::Color color = this->shade(ray, minHit);
        writePixel(x, y, color);
    }
}

void Render::StartRender(void) noexcept
{
    const auto &cam = scene.getCamera();
    this->ImageRender = true;
    sf::Clock clock;

    int nbPixel = cam.width * cam.height;
    int count = 0;
    int percent = 0;

    for (int x = 0; x < cam.width; x++){
        if (load.pushLoad(window) == true)
            this->HandleWindow();
        if (window.isOpen() == false)
            return;
        if (percent != (count * 100) / nbPixel){
            percent = (count * 100) / nbPixel;
            this->load.pushPercent(window, percent);
            std::cout << percent << std::endl;
        }
        for (int y = 0; y < cam.height; y++){
            this->FindObject(x, y);
            count++;
        }
    }
    sf::Time RenderTime = clock.getElapsedTime();
    sf::Int32 RenderTimeMs = RenderTime.asMilliseconds();

    std::cout << CLR_BOLD_DEBUG << "Render Time:" << RenderTimeMs << CLR_RESET << std::endl;

    this->image.create(scene.getCamera().width, scene.getCamera().height, RayBuffer.data());
    this->texture.loadFromImage(image);
    this->sprite.setTexture(this->texture);
}
