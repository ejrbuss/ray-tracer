#include <iostream>
#include <fstream>

using namespace std;

typedef __int8 i8;
typedef __int16 i16;
typedef __int32 i32;
typedef __int64 i64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

auto main() -> int {
    i32 nx = 200;
    i32 ny = 100;
    cout << "P3" << endl << nx << " " << ny << endl << 255 << endl;
    for(i32 j = ny - 1; j >= 0; j--) {
        for(i32 i = 0; i < nx; i++) {
            float r = float(i) / float(nx);
            float g = float(j) / float(ny);
            float b = 0.2;
            i32 ir = int(255.99 * r);
            i32 ig = int(255.99 * g);
            i32 ib = int(255.99 * b);
            cout << ir << " " << ig << " " << ib << endl;
        }
    }
}

auto bmp(const u32 width, const u32 height, const ostream& stream = cout) -> void {

    // File header

    u16 bf_type     = "BM";
    u32 bf_size     = -1; // TODO
    u32 bf_reserved = 0;
    u32 bf_offset   = -1; // TODO

    // Image header

    u32 bi_size        = 40;
    u32 bi_width       = width;
    u32 bi_height      = height;
    u16 bi_planes      = 1;
    u16 bi_count       = 24;
    u64 bi_compression = 0;
    u32 bi_xppm        = 0;
    u32 bi_yppm        = 0;
    u32 bi_clrused     = 0;
    u32 bi_clrimp      = 0;

    //

}