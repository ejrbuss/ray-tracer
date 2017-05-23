#pragma once

#include "lib/data/material.hpp"

class Intersection {

    public:
        f32 t;
        Vec point;
        Vec normal;
        Material material;

    Intersection() {}

    Intersection(const f32 t, const Vec & p, const Vec & n, const Material & m) :
        t(t),
        point(p),
        normal(glm::normalize(n)),
        material(m)
    {}

    auto str() const -> string {
        return "ray(" + to_string(t) + ") = " + vec::str(point);
    }

};