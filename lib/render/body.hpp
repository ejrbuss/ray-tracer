#pragma once

#include "lib/data/ray.hpp"
#include "lib/data/intersection.hpp"
#include "lib/data/material.hpp"

using namespace std;

typedef function<bool(const Ray &, f32, f32, Intersection &)> IntersectionFn;

class Body {
    public:
        string         type;
        IntersectionFn intersects;

        Body(const string & t, const IntersectionFn & i) : type(t), intersects(i) {}
};

namespace body {

    const f32 EPSILON   = 0.001;

    auto aggregate(const vector<Body> & bodies) -> Body {
        return Body("aggregate", [&](const Ray & ray, f32 min, f32 max, Intersection & i) {

            Intersection tmp;
            f32 closest      = max;
            bool intersected = false;

            for(Body const & body : bodies) {
                if(body.intersects(ray, min, closest, tmp)) {
                    intersected = true;
                    closest     = tmp.t;
                    i           = tmp;
                }
            }
            return intersected;
        });
    }

    auto sphere(const Vec & center, const f32 radius, const Material & material) -> Body {
        return Body("sphere", [=](const Ray & ray, f32 min, f32 max, Intersection & i) {

            const Vec oc = ray.origin - center;
            const f32 a  = glm::dot(ray.direction, ray.direction);
            const f32 b  = glm::dot(ray.direction, oc) * 2.0;
            const f32 c  = glm::dot(oc, oc) - radius * radius;
            const f32 d  = b * b - 4 * a * c;

            f32 t;

            if(d > 0) {
                t = (-b - sqrt(d)) / (2.0 * a);
                if(t > min && t < max) {
                    i = Intersection(t, ray.at(t), ray.at(t) - center, material);
                    return true;
                }
                t = (-b + sqrt(d)) / (2.0 * a);
                if(t > min && t < max) {
                    i = Intersection(t, ray.at(t), ray.at(t) - center, material);
                    return true;
                }
            }
            return false;
        });
    }

    auto plane(const Vec & point, const Vec & n, const Material & material) -> Body {

        const Vec normal = glm::normalize(n);

        return Body("plane", [=](const Ray & ray, f32 min, f32 max, Intersection & i) {

            const f32 t =
                glm::dot(point - ray.origin, normal) /
                glm::dot(normal, ray.direction);

            if(t > min && t < max) {
                i = Intersection(t, ray.at(t), normal, material);
                return true;
            }
            return false;
        });
    }

    auto triangle(const Vec & v1, const Vec & v2, const Vec & v3, const Material & material) -> Body {

        const Vec edge1  = v2 - v1;
        const Vec edge2  = v3 - v1;
        const Vec normal = glm::cross(edge1, edge2);

        return Body("triangle", [=](const Ray & ray, f32 min, f32 max, Intersection & i) {

            const Vec a = glm::cross(ray.direction, edge2);
            const Vec b = ray.origin - v1;
            const Vec c = glm::cross(b, edge1);

            const f32 d = glm::dot(edge1, a);
            const f32 u = glm::dot(b, a) / d;
            const f32 v = glm::dot(ray.direction, c) / d;

            if(fabs(d) > EPSILON && u > 0 && u < 1 && v > 0 && u + v < 1) {
                const f32 t = glm::dot(edge2, c) / d;
                if(t > min && t < max) {
                    i = Intersection(t, ray.at(t), normal, material);
                    return true;
                }
            }
            return false;
        });

    }

    auto quad(const Vec & v1, const Vec & v2, const Vec & v3, const Material & material) -> Body {

        const Vec s1 = v2 - v1;
        const Vec s2 = v3 - v1;
        const Vec normal = -glm::cross(s1, s2);

        const f32 sqrs1 = glm::dot(s1, s1);
        const f32 sqrs2 = glm::dot(s2, s2);

        return Body("quad", [=](const Ray & ray, f32 min, f32 max, Intersection & i) {

            const f32 t =
                glm::dot(v1 - ray.origin, normal) /
                glm::dot(normal, ray.direction);

            if(t > min && t < max) {
                i = Intersection(t, ray.at(t), normal, material);
                const Vec s3 = ray.at(t) - v1;
                const f32 u = glm::dot(s3, s1);
                const f32 v = glm::dot(s3, s2);
                return u >= 0 && u <= sqrs1 && v >= 0 && v <= sqrs2;
            }
            return false;
        });
    }

}