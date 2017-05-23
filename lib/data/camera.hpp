#pragma once

class Camera {

    public:
        Vec origin;
        Vec llc;
        Vec hrz;
        Vec vrt;

        /**
         * Camera largely adopted from
         * "Ray Tracing in One Weekend" by Peter Shirley
         */
        Camera(const Vec & o, const Vec & to, const Vec & vup, const f32 fov, const f32 aspect) : origin(o) {

            // Calculate screen width and height
            const f32 height = tan(radians(fov) / 2) * 2;
            const f32 width  = aspect * height;

            // Calculate orthanormal basis
            const Vec w = glm::normalize(origin - to);
            const Vec u = glm::normalize(glm::cross(vup, w));
            const Vec v = glm::normalize(glm::cross(w, u));

            // Set attirbutes
            hrz = width  * u;
            vrt = height * v;
            llc = origin - hrz / f32(2) - vrt / f32(2) - w;
        }
};