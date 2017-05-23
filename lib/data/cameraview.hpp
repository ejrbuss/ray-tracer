#pragma once

#include "lib/data/camera.hpp"

using namespace std;

class CameraView {

    public:
        Vec from;
        Vec to;
        Vec vup;

    /* Builder Class for Camera */
    CameraView(const Vec & f, const Vec & t, const Vec & v) : from(f), to(t), vup(v) {}

    auto camera(const f32 fov, const f32 aspect) const -> Camera {
        return Camera(from, to, vup, fov, aspect);
    }
};