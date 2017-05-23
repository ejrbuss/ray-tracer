#pragma once

#include "lib/core.hpp"

using namespace std;

class Resolution {

    public:
        u32 width;
        u32 height;
        f32 aspect;

        Resolution(const u32 w, const u32 h) : width(w), height(h), aspect(w/h) {}
};