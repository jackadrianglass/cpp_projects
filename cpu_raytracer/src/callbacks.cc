#include "callbacks.h"

#include <vector>
#include <algorithm>
#include <execution>

#include <glm/glm.hpp>
#include <glm/gtx/vector_query.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "scene.h"
#include "ray_trace.h"
#include "lighting.h"
#include "image_buffer.h"

auto hasIntersection(const Scene& scene, Ray ray, int skipId) -> int
{
    for(auto& shape : scene.shapesInScene) {
        auto tmp = shape->getIntersection(ray);
        if(shape->id != skipId
            && tmp.type != Intersection::Type::None
            && glm::distance(tmp.point, ray.origin) > 0.00001
            && glm::distance(tmp.point, ray.origin) < glm::distance(ray.origin, scene.lightPos) - 0.01
        )
        {
            tmp.id;
        }
    }
    return -1;
}

auto getClosestIntersection(const Scene& scene, Ray ray, int skipId) -> Intersection
{
    Intersection closest{
        -1,
        Intersection::Type::None,
        {},
        {},
        {}
    };
    auto min = std::numeric_limits<float>::max();

    for(auto& shape : scene.shapesInScene) {
        if(skipId == shape->id) {
            continue;
        }
        auto p = shape->getIntersection(ray);
        auto distance = glm::distance(p.point, ray.origin);

        if(p.type == Intersection::Type::Object && distance < min && distance > 0.0001) {
            min = distance;
            closest = p;
        }
    }

    if(glm::distance(scene.lightPos, ray.origin) < min){
        // Figure out if they hit a light source
        static constexpr auto epsilon = 0.00001f;
        const auto get_t = [](auto l, auto o, auto d){return (l - o) / d;};
        const auto tx = get_t(scene.lightPos.x, ray.origin.x, ray.dir.x);
        const auto ty = get_t(scene.lightPos.y, ray.origin.y, ray.dir.y);
        const auto tz = get_t(scene.lightPos.z, ray.origin.z, ray.dir.z);

        if(std::abs(tx - ty) < epsilon && std::abs(tx - tz) < epsilon) {
            closest.id = -1;
            closest.point = scene.lightPos;
            closest.type = Intersection::Type::LightSource;
        }
    }

    return closest;
}

auto raytraceSingleRay(const Scene& scene, const Ray& ray, int level, int source_id) -> glm::vec3
{
    if(level < 0){
        return glm::vec3(0); //background colour
    }
    const auto result = getClosestIntersection(scene, ray, source_id);

    if(result.type == Intersection::Type::None) {
        return glm::vec3(0); //background colour
    } else if(result.type == Intersection::Type::LightSource){
        return scene.lightCol;
    } else {
        auto phong = PhongReflection{result, result.material, ray, scene};

        const auto l = -1.0f * ray.dir;
        const auto r_dir = glm::normalize(2 * glm::dot(result.normal, l) * result.normal - l);
        const auto r_ray = Ray{result.point, r_dir};
        const auto reflection = result.material.reflectionStrength * raytraceSingleRay(scene, r_ray, level - 1, result.id);

        // in shadow
        const auto light_dir = glm::normalize(scene.lightPos - result.point);
        const auto shadow_collision = getClosestIntersection(scene, {result.point, light_dir}, result.id);

        auto diffuse = glm::vec3(0);
        if(shadow_collision.type == Intersection::Type::LightSource){
            diffuse = phong.Id();
        }

        return diffuse + reflection + phong.Ia();
    }
}

struct RayAndPixel {
    Ray ray;
    glm::ivec2 pos;
};

auto getRaysForViewpoint(ImageBuffer& image, glm::vec3 viewPoint) -> std::vector<RayAndPixel>
{
    auto pos = glm::ivec2{};
    auto rays = std::vector<RayAndPixel>{};

    for(auto i = -1.0f; pos.x < image.width(); pos.x++) {
        pos.y = 0;
        for(auto j = -1.0f; pos.y < image.height(); pos.y++){
            const auto dir = glm::normalize(glm::vec3{i, j, -2.0f} - viewPoint);
            const auto ray = Ray{viewPoint, dir};
            rays.push_back({ray, pos});
            j += 2.0f / image.height();
        }
        i += 2.0f / image.width();
    }
    return rays;
}

auto raytraceImage(const Scene& scene, ImageBuffer& image, glm::vec3 viewPoint) -> void {
    image.init();

    const auto rays = getRaysForViewpoint(image, viewPoint);

    // std::for_each(std::execution::par, std::begin(rays), std::end(rays), [&](const auto& ray){
    for(const auto& ray : rays) { //todo parallelize?
        const auto colour = raytraceSingleRay(scene, ray.ray, 2, -1);
        image.setPixel(ray.pos, colour);

    };
    // });
}

Callbacks::Callbacks(glm::vec2 dimensions)
    : m_screen{dimensions}
    , m_viewPoint{0, 0, 0}
    , m_scene(initScene1())
{
    raytraceImage(m_scene, m_outImage, m_viewPoint);
}

auto Callbacks::keyCallback(int key, [[maybe_unused]]int scancode, int action, [[maybe_unused]]int mods) -> void 
{
    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_1){
            spdlog::info("Rendering scene 1");
            m_scene = initScene1();
            raytraceImage(m_scene, m_outImage, m_viewPoint);
        }
        else if(key == GLFW_KEY_2){
            spdlog::info("Rendering scene 2");
            m_scene = initScene2();
            raytraceImage(m_scene, m_outImage, m_viewPoint);
        }
    }
}

auto Callbacks::mouseButtonCallback(int, int, int) -> void {}
// auto Callbacks::mouseButtonCallback(int button, int action, int mods) -> void {}

auto Callbacks::cursorPosCallback(glm::vec2) -> void {}
// auto Callbacks::cursorPosCallback(glm::vec2 pos) -> void {}

auto Callbacks::scrollCallback(glm::vec2) -> void {}
// auto Callbacks::scrollCallback(glm::vec2 size) -> void {}

auto Callbacks::windowSizeCallback(glm::ivec2 size) -> void 
{
    m_screen = size;
    glViewport(0, 0, static_cast<GLsizei>(m_screen.x), static_cast<GLsizei>(m_screen.y));
}

auto Callbacks::update() -> void 
{
    
}

auto Callbacks::draw() -> void 
{
    m_outImage.render();
}

auto Callbacks::imgui_draw() -> void 
{
    
}