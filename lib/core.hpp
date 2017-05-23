#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>
#include <regex>

#include <cstdlib>
#include <cfloat>
#include <cstdio>
#include <ctime>

#include "../glm/glm.hpp"

#define debug \
    if (!DEBUG) {} \
    else std::cerr

using namespace std;

/// Signed Types
typedef char      i8;
typedef short     i16;
typedef int       i32;
typedef long long i64;
typedef int       isize;

/// Unsigned Types
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef size_t             usize;

/// Floating Point Types
typedef float  f32;
typedef double f64;

/// Vector types
typedef glm::vec3 Vec;
typedef glm::vec3 Point;
typedef glm::mat3 Mat;

bool DEBUG = false;

namespace std {

    const f64 PI  =3.141592653589793238463;

    auto radians(const f32 angle) -> f32 {
        return angle * PI / 180;
    }

    auto tolower(const string & s) -> string {
        string out;
        transform(s.begin(), s.end(), back_inserter(out),
            (int (*)(int))std::toupper
        );
        return out;
    }

    auto toupper(const string & s) -> string {
        string out;
        transform(s.begin(), s.end(), back_inserter(out),
            (int (*)(int))std::toupper
        );
        return out;
    }

    /// to_bytes
    template<typename T>
    auto as_bytes(const T & x) -> i8* {
        return (i8*)(&x);
    }

    template<typename T>
    auto equal(const string & a, const T & b) -> bool {
        return !a.compare(b);
    }

    auto frand() -> f32 {
        return (f32)(rand()) / (f32)(RAND_MAX);
    }

    template <typename T>
    auto to_string(const T & x) -> string {
        ostringstream stream;
        stream << x ;
        return stream.str();
    }

    auto stoi(const string & s) -> i32 {
        stringstream stream(s);
        i32 n;
        stream << s;
        stream >> n;
        return n;
    }

    auto stof(const string & s) -> f32 {
        stringstream stream(s);
        f32 n;
        stream << s;
        stream >> n;
        return n;
    }

    auto stov(const string & s) -> Vec {
        stringstream stream(s);
        char tmp;
        f32 a, b, c;
        stream << s;
        stream >> a;
        stream >> tmp;
        stream >> b;
        stream >> tmp;
        stream >> c;
        return Vec(a, b, c);
    }

    auto success() -> void {
        exit(0);
    }
    auto success(const string & s) -> void {
        cout << s << endl;
        success();
    }
    auto fail() -> void {
        exit(1);
    }
    auto fail(const string & s) -> void {
        cerr << s << endl;
        fail();
    }
}

namespace vec {

    const Vec zero(0, 0, 0);

    auto str(const Vec & v) -> string {
        stringstream stream;
        stream << "[" << v.x << ", " << v.y << ", " << v.z << "]";
        return stream.str();
    }

    auto rand() -> const Vec {
        Vec point;
        do {
            point = f32(2.0) * Vec(frand(), frand(), frand()) - Vec(1.0, 1.0, 1.0);
        } while(glm::length(point) >= 1.0);
        return point;
    }

    auto cclamp(const Vec & v) -> Vec {
        return glm::clamp(v, Vec(0, 0, 0), Vec(1, 1, 1));
    }

    auto dither() -> Vec {
        const f32 off = (frand() - 0.5) / (255 * 3);
        return Vec(off, off, off);
    }

    auto reflect(const Vec & v, const Vec & normal) -> const Vec {
        return v - f32(2.0) * glm::dot(v, normal) * normal;
    }

    auto refract(const Vec & v, const Vec & normal, const f32 index, Vec & refracted) -> bool {
        Vec uv = glm::normalize(v);
        f32 dt = glm::dot(uv, normal);
        f32 d  = f32(1.0) - index * index * (f32(1.0) - dt * dt);
        if(d > 0) {
            refracted = index * (uv - normal * dt) - normal * sqrt(d);
            return true;
        }
        return false;
    }

    auto schlick(const f32 cosine, const f32 refIndex) -> f32 {
        f32 r = (1.0 - refIndex) / (1.0 + refIndex);
        r = r * r;
        return r + (1 - r) * pow((1.0 - cosine), 5);
    }

}