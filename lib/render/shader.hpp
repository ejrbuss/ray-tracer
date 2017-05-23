#pragma once

using namespace std;

typedef function<const Vec(const Ray &, const Scene &, const u32)> ShaderFn;

class Shader;
namespace shader { vector<Shader*> shaders; }

class Shader {

    public:
        string name;
        ShaderFn shade;

        Shader(const string & n, const ShaderFn & s) : name(n), shade(s) {
            shader::shaders.push_back(this);
        }

        auto operator()(
            const Ray & ray,
            const Scene & scene,
            const u32 depth
        ) const -> const Vec {
            return shade(ray, scene, depth);
        }
};

namespace shader {

    const u32 MAX_DEPTH = 16;


    auto get(const string & name) -> const Shader {
        for(const Shader* shader: shaders) {
            if(equal(name, (*shader).name)) {
                return *shader;
            }
        }
        fail(name + " is not a valid shader name.");
    }

    auto background(const Ray & ray) -> const Vec {
        f32 t = 0.5 * ray.direction.y + 1;
        return (f32)(1.0 - t) * Vec(1.0, 1.0, 1.0) + t * Vec(0.5, 0.7, 1.0);
    }

}

namespace shader {

    const ShaderFn normalShader = [](const Ray & ray, const Scene & scene, const u32 depth) -> const Vec {

        Intersection i;
        if(scene.world.intersects(ray, body::EPSILON, FLT_MAX, i)) {
            return (f32)(0.5) * Vec(
                i.normal.x + 1,
                i.normal.y + 1,
                i.normal.z + 1
            );
        }
        return background(ray);
    };
    const Shader normal("normal", normalShader);

    const ShaderFn scatterShader = [](const Ray & ray, const Scene & scene, const u32 depth) -> const Vec {
            Intersection i;

            if(scene.world.intersects(ray, body::EPSILON, FLT_MAX, i)) {
                if(depth > MAX_DEPTH) {
                    return vec::zero;
                }

                const Vec off     = vec::rand();
                const Vec target  = i.point + i.normal + off;
                const Vec reflect = vec::reflect(ray.direction, i.normal);

                Vec color = vec::zero;

                // Diffuse color
                if(glm::length(i.material.diff) > body::EPSILON) {
                    color = color + i.material.diff * scatterShader(Ray(i.point, target - i.point), scene, depth + 1);
                }
                // Reflection
                if(glm::length(i.material.refl) > body::EPSILON) {
                    color = color + i.material.refl * scatterShader(Ray(i.point, reflect + i.material.fuzz * off), scene, depth + 1);
                }
                return color;
            }
            return background(ray);
    };
    const Shader scatter("scatter", scatterShader);

    auto ambient(const Intersection & i) -> Vec {
        return material::AMBIENT * i.material.amb;
    }

    auto diffuse(const Intersection & i, const Light & light, const Vec & l, const Vec & f) -> Vec {
        return vec::cclamp(light.intensity * i.material.diff * glm::dot(f, l));
    }

    auto specular(const Ray & ray, const Intersection & i, const Light & light, const Vec & l) -> Vec {
        Vec h = glm::normalize(l - ray.direction);
        return vec::cclamp(light.intensity * i.material.spec * pow(glm::dot(i.normal, h), i.material.specpow));
    }

    const ShaderFn phongShader = [](const Ray & ray, const Scene & scene, const u32 depth) -> const Vec {

            Intersection i, tmp;

            Vec color(0, 0, 0);

            if(scene.world.intersects(ray, body::EPSILON, FLT_MAX, i)) {
                if(depth > MAX_DEPTH) {
                    return color;
                }
                const Vec fuzz = glm::normalize(i.normal + i.material.fuzz * vec::rand());
                color = color + ambient(i);

                for(const Light & light : scene.lights) {
                    Vec l   = light.point - i.point;
                    f32 max = glm::length(l);
                    l       = glm::normalize(l);
                    if(!scene.world.intersects(Ray(i.point, l), body::EPSILON, max, tmp)) {
                        color = color
                            + diffuse(i, light, l, fuzz)
                            + specular(ray, i, light, l);
                    }
                }
                if(glm::length(i.material.refl) > body::EPSILON) {
                    color += i.material.refl * phongShader(Ray(i.point, vec::reflect(ray.direction, fuzz)), scene, depth + 1);
                }
                return color;
            }
            return background(ray);
    };
    const Shader phong("phong", phongShader);
}