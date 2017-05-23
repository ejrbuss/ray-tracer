#pragma once

class Color;

namespace color {

    // Color constants
    const f32 GAMMA = 1.0 / 2.0;
    const f32 RANGE = 255.99;

    /*
     * Convert a floating point color representation to an integer between 0
     * and range with gamma adjustment.
     */
    auto convert(const f32 f) -> i32 { return RANGE * pow(f, GAMMA); }
}

class Color {
    public:
        u8 r;
        u8 g;
        u8 b;

        Color() {}

        Color(const Vec & v) :
            r(color::convert(v.r)),
            g(color::convert(v.g)),
            b(color::convert(v.b))
        {}

        Color(const f32 r, const f32 g, const f32 b) : Color(Vec(r, g, b)) {}

        Color(const i32 r, const i32 g, const i32 b) : r(r), g(g), b(b) {}

        auto str() const -> string {
            return to_string((i32)(r)) + " " + to_string((i32)(g)) + " " + to_string((i32)(b));
        }

        /* Get LittleEndian data rep. */
        auto bytes(i8* data) const -> const i8* {
            data[0] = (i8)(b);
            data[1] = (i8)(g);
            data[2] = (i8)(r);
            return data;
        }
};

namespace color {
     /// Color Presets
    const Color BLACK(0,   0,   0  );
    const Color WHITE(255, 255, 255);
    const Color RED  (255, 0,   0  );
    const Color GREEN(0,   255, 0  );
    const Color BLUE (0,   0,   255);
}
