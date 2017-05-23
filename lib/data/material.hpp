#pragma once

class Material {

    public:
        Vec amb;
        Vec diff;
        Vec spec;
        Vec refl;

        f32 specpow;
        f32 fuzz;

        Material() {}

        Material(const Vec & a, const Vec & d, const Vec & s, const Vec & r, const u32 p, const f32 f) :
            amb(a), diff(d), spec(s), refl(r), specpow(p), fuzz(f)
        {}

        auto str() const -> string {
            return vec::str(amb);
        }
};

namespace material {

    const f32 AMBIENT = 0.1;

    auto scatterLambertian(const Vec & albedo) -> Material {
        return Material(albedo / f32(4), albedo, Vec(0.1, 0.1, 0.1), vec::zero, 2, 0.2);
    }

    auto scatterMetal(const Vec & albedo, f32 fuzz) -> Material {
        return Material(albedo / f32(4), vec::zero, vec::zero, albedo, 0, fuzz);
    }

    const Material cornellLight(
        Vec(1, 1, 1),
        Vec(1, 1, 1),
        Vec(1, 1, 1),
        Vec(1, 1, 1),
        35, 0.1
    );

    const Material triforce(
        Vec(0.3, 0.25, 0.07),
        Vec(1.0, 0.9,  0.3),
        Vec(0.5, 0.4,  0.1),
        Vec(0.3, 0.2,  0),
        32, 0.1
    );

    const Material ebrass(
        Vec(0.1, 0.1, 0.1),
        Vec(0.3, 0.3, 0.3),
        Vec(0.1, 0.1, 0.1),
        Vec(0,   0,   0),
        2, 0
    );

    const Material cbrass(
        Vec(0.1,  0.1,  0.1),
        Vec(0.3,  0.3,  0.3),
        Vec(0.9,  0.9,  0.9),
        Vec(0,    0,    0),
        35, 0
    );

    const Material fbrass(
        Vec(0.1,  0.1,  0.1),
        Vec(0.3,  0.3,  0.3),
        Vec(0.9,  0.9,  0.9),
        Vec(0.05,  0.05,  0.05),
        35, 0
    );

     const Material mirror(
         Vec(0.2, 0.2, 0.2),
         Vec(0.6, 0.6, 0.6),
         Vec(0.9, 0.9, 0.9),
         Vec(0.8, 0.8, 0.8),
         35, 0.01
     );
}