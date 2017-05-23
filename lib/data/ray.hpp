#pragma once

class Ray {

    public:
        Vec origin;
        Vec direction;

        Ray() {}

        Ray(const Vec & o, const Vec & d) :
            origin(o),
            direction(glm::normalize(d))
        {}

        Ray(const Camera & c, const f32 u, const f32 v) :
            origin(c.origin),
            direction((c.llc + u * c.hrz + v * c.vrt) - c.origin)
        {}

        auto at(const f32 t) const -> const Vec {
            return origin + t * direction;
        }

};