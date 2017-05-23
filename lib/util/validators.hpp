#pragma once

using namespace std;

namespace valid {

    auto format(string & format) -> Validator {
        return [&](i32 n, const char** args) mutable -> i32 {
            format = string(args[n]);
            if(!equal(format, "bmp") && !equal(format, "ppm")) {
                fail(format + " is not a valid output foramt.");
            }
            return 1;
        };
    }

    auto out(string & out) -> Validator {
        return [&](i32 n, const char** args) mutable -> i32 {
            out = string(args[n]);
            return 1;
        };
    }

    auto shader(Shader & shader) -> Validator {
        return [&](i32 n, const char** args) mutable -> i32 {
            shader = shader::get(string(args[n]));
            return 1;
        };
    }

    auto scene(Scene & scene) -> Validator {
        return [&](i32 n, const char** args) mutable -> i32 {
            scene = scene::get(string(args[n]));
            return 1;
        };
    }

    auto aa(AA & aa) -> Validator {
        return [&](i32 n, const char** args) mutable -> i32 {
            aa = aa::get(string(args[n]));
            return 1;
        };
    }

    auto fov(f32 & fov) -> Validator {
        return [&](i32 n, const char** args) mutable -> i32 {
            fov = stof(string(args[n]));
            if(fov < 0 || fov >= 180) {
                fail(to_string(fov) + " is not a valid fov.");
            }
            return 1;
        };
    }

    auto camera(CameraView & camera) -> Validator {
        return [&](i32 n, const char** args) mutable -> i32 {
            const Vec from = stov(string(args[n + 0]));
            const Vec to   = stov(string(args[n + 1]));
            const Vec vup  = stov(string(args[n + 2]));

            camera = CameraView(from, to, vup);
            return 3;
        };
    }

    auto res(Resolution & res) -> Validator {
        return [&](i32 n, const char** args) mutable -> i32 {
            const string s = string(args[n]);
            regex rgx(R"((\d+)x(\d+))");
            smatch matches;

            if(!std::regex_search(s, matches, rgx)) {
                fail(s + " is not a valid resolution.");
            }
            res = Resolution(stoi(matches[1]), stoi(matches[2]));
            return 1;
        };
    }

}