#pragma once

#include <pthread.h>
#include "lib/core.hpp"
#include "lib/data/color.hpp"

#define poolSize 32

using namespace std;

class Buffer;

typedef function<Color(const Color &, u32, u32, const Buffer &)> BufferMapper;
typedef function<void (const Color &, u32, u32, const Buffer &)> BufferIter;

class Buffer {

    public:
        u32 width;
        u32 height;
        vector<Color> data;

        Buffer() {}

        Buffer(const u32 w, const u32 h) :
            width(w),
            height(h),
            data(w * h, color::BLACK)
        {}

        auto get(const u32 x, const u32 y) const -> const Color & {
            return data[y * width + x];
        }

        auto set(const u32 x, const u32 y, const Color & c) -> const Color & {
            return (data[y * width + x] = c);
        }

        auto each(const BufferIter & fn, const i32 xstep = 1, const i32 ystep = 1) const -> void {

            const i32 ystart = ystep > 0 ? 0 : height - 1;
            const i32 xstart = xstep > 0 ? 0 : width - 1;

            for(i32 y = ystart; y >= 0 && y < height; y += ystep) {
            for(i32 x = xstart; x >= 0 && x < width;  x += xstep) {
                fn(get(x, y), x, y, *this);
            }}
        }

        auto map(const BufferMapper & fn, const i32 xstep = 1, const i32 ystep = 1) -> void {
            const i32 ystart = ystep > 0 ? 0 : height - 1;
            const i32 xstart = xstep > 0 ? 0 : width - 1;

            for(i32 y = ystart; y >= 0 && y < height; y += ystep) {
            for(i32 x = xstart; x >= 0 && x < width;  x += xstep) {
                set(x, y, fn(get(x, y), x, y, *this));
            }}
        }

        auto out(const string & format, const string & path) const -> void {
            equal(format, "bmp")
                ? bmp(path)
                : ppm(path);
        }

        auto ppm(const string & path) const -> void {

            ofstream ppm;
            ofstream* ppm_ptr = &ppm;

            ppm.open(path);
            ppm << "P3" << endl << width << " " << height << endl << 255 << endl;

            each([&](const Color c, u32 x, u32 y, const Buffer & b) {
                (*ppm_ptr) << c.str() << endl;
            }, 1, -1);

            ppm.close();
        }

        auto bmp(const string & path) const -> void {

            ofstream fs;
            ofstream* bmp = &fs;
            i8 data[3];

            const u32 fileSize        = 54 + 3 * width * height;
            const i8 fileHeader[]     = {0,0, 0,0, 54,0,0,0};
            const i8 imageHeader1[]   = {40,0,0,0};
            const i8 imageHeader2[28] = {1,0, 24,0};
            const i8 padding[]        = {0, 0, 0, 0};

            bmp->open(path, ios::binary | ios::out);
            bmp->write("BM", 2);
            bmp->write(as_bytes(fileSize), 4);
            bmp->write(fileHeader, 8);
            bmp->write(imageHeader1, 4);
            bmp->write(as_bytes(width), 4);
            bmp->write(as_bytes(height), 4);
            bmp->write(imageHeader2, 28);

            each([&](const Color & c, u32 x, u32 y, const Buffer & b) {

                bmp->write(c.bytes(data), 3);
                if(x == width - 1) {
                    bmp->write(padding, (4 - (width * 3) % 4) % 4);
                }
            });
            bmp->close();
        }

};