#pragma once

class Light {
    public:
        Vec point;
        Vec intensity;

        Light(const Vec & p, const Vec & i) : point(p), intensity(i) {}

        Light(const Vec & p, const f32 i) : point(p), intensity(Vec(i,i,i)) {}

};

namespace light {

    vector<Light> lights;

    auto area(const Light & light, const u32 count, const f32 spread) -> vector<Light> {

        const u32 max = sqrt(count);
        const f32 off = 2 * spread / max;
        const Vec i   = light.intensity * f32(1) / f32(max * max);
        const f32 x   = light.point.x;
        const f32 z   = light.point.z;

        for(u32 ix = 0; ix < max; ix++) {
        for(u32 iz = 0; iz < max; iz++) {
            lights.push_back(Light(Vec(x + ix * off, light.point.y, z + iz * off), i));
        }}
        return lights;
    }

}