#pragma once

using namespace std;

typedef function<Color(
    const Camera &,
    const Scene &,
    const Shader &,
    u32 x, u32 y,
    const Buffer &
)> AliasFn;

class AA;

namespace aa { vector<AA*> aliases; }

class AA {

    public:
        string name;
        AliasFn alias;

        AA(const string & n, const AliasFn & s) : name(n), alias(s) {
            aa::aliases.push_back(this);
        }

        auto sample(
            const Camera & c,
            const Scene & s,
            const Shader & h
        ) const -> BufferMapper {
            return [&](const Color & color, u32 x, u32 y, const Buffer & b) -> Color {
                return alias(c, s, h, x, y, b);
            };
        }
};

namespace aa {

    auto get(const string & name) -> const AA {
        for(const AA* aa: aliases) {
            if(equal(name, (*aa).name)) {
                return *aa;
            }
        }
        fail(name + " is not a valid AA algorithm.");
    }

    auto progress(const u32 x, const u32 y, const u32 height) -> void {
        if(x == 0) {

            const f32 p = f32(y) / f32(height - 1);
            const u32 w = 60;
            const u32 i = p * w;

            debug << "[";
            for (u32 j = 0; j < w; j++) {
                debug << (j <= i ? "|" : " ");
            }
            debug << "] " << u32(p * 100.0) << "%\r";
            debug.flush();
        }
    }

    const AA none("none", [](
        const Camera & camera,
        const Scene & scene,
        const Shader & shade,
        u32 x, u32 y,
        const Buffer & b
    ) -> Color {
        progress(x, y, b.height);
        const f32 u = f32(x) / f32(b.width);
        const f32 v = f32(y) / f32(b.height);
        return Color(shade(Ray(camera, u, v), scene, 1));
    });

    const AA centered("centered", [](
        const Camera & camera,
        const Scene & scene,
        const Shader & shade,
        u32 x, u32 y,
        const Buffer & b
    ) -> Color {
        progress(x, y, b.height);
        const f32 u = f32(x + 0.5) / f32(b.width);
        const f32 v = f32(y + 0.5) / f32(b.height);
        return Color(shade(Ray(camera, u, v), scene, 1));
    });

    auto SSAA(u32 times) -> const AA {

        const f32 step = 1.0 / times;
        const f32 K    = 1.0 / (times * times);
        const f32 f_1  = step * 0.1;
        const f32 f_3  = step * 0.3;
        const f32 f_7  = step * 0.7;
        const f32 f_9  = step * 0.9;

        return AA(to_string(times) + "xSSAA", [=](
            const Camera & camera,
            const Scene & scene,
            const Shader & shade,
            u32 x, u32 y,
            const Buffer & b
        ) -> Color {
            progress(x, y, b.height);
            Vec s(0,0,0);
            f32 u;
            f32 v;

            for(f32 yoff = 0; yoff < 1; yoff += step) {
            for(f32 xoff = 0; xoff < 1; xoff += step) {

                u = f32(x + xoff + f_7) / f32(b.width);
                v = f32(y + yoff + f_1) / f32(b.height);
                const Vec s1 = shade(Ray(camera, u, v), scene, 1);

                u = f32(x + xoff + f_1) / f32(b.width);
                v = f32(y + yoff + f_3) / f32(b.height);
                const Vec s2 = shade(Ray(camera, u, v), scene, 1);

                u = f32(x + xoff + f_9) / f32(b.width);
                v = f32(y + yoff + f_7) / f32(b.height);
                const Vec s3 = shade(Ray(camera, u, v), scene, 1);

                u = f32(x + xoff + f_3) / f32(b.width);
                v = f32(y + yoff + f_9) / f32(b.height);
                const Vec s4 = shade(Ray(camera, u, v), scene, 1);

                s += (s1 + s2 + s3 + s4) * f32(1.0 / 4.0);
            }}
            return Color(s * K);
        });
    }

    const AA SSAAx4  = SSAA(1);
    const AA SSAAx8  = SSAA(2);
    const AA SSAAx16 = SSAA(4);
    const AA SSAAx32 = SSAA(8);
    const AA SSAAx64 = SSAA(16);

}