#include "lib/core.hpp"
#include "lib/data/buffer.hpp"
#include "lib/data/resolution.hpp"
#include "lib/data/cameraview.hpp"
#include "lib/data/scene.hpp"
#include "lib/render/shader.hpp"
#include "lib/render/aa.hpp"
#include "lib/util/argparser.hpp"
#include "lib/util/validators.hpp"

using namespace std;

auto render(
    const Resolution & res,
    const Camera & camera,
    const Scene & scene,
    const Shader & shader,
    const AA & aa
) -> const Buffer {
    Buffer buffer(res.width, res.height);
    buffer.map(aa.sample(camera, scene, shader));
    return buffer;
}

auto main(const i32 argc, const i8 * argv[]) -> i32 {

    Buffer buffer;

    /// Default Values
    string format      = "bmp";
    string out         = "render.bmp";
    Shader shader      = shader::normal;
    Scene scene        = scene::scene1;
    AA aa              = aa::none;
    f32 fov            = 90;
    CameraView camView = CameraView(Vec(0,0,0), Vec(0,0,-1), Vec(0,1,0));
    Resolution res     = Resolution(1000, 500);

    bool preview = false;

    /// Command Line Arguments
    ArgParser parser("rayn", R"(
  __________    _____ _____.___._______
  \______   \  /  _  \\__  |   |\      \
   |       _/ /  /_\  \/   |   |/   |   \
   |    |   \/    |    \____   /    |    \
   |____|_  /\____|__  / ______\____|__  /
          \/         \/\/              \/
         A ray tracer built by @ejrbuss
    )");

    parser.arg(valid::format(format),  "--format",     "-f", "output format (bmp or ppm)");
    parser.arg(valid::out(out),        "--out",        "-o", "output file path");
    parser.arg(valid::shader(shader),  "--shader",     "-s", "select shader (normal, scatter, phong)");
    parser.arg(valid::scene(scene),    "--scene",      "-S", "select scene");
    parser.arg(valid::aa(aa),          "--aa",         "-a", "select anti aliasing method (none, centered, SSAA)");
    parser.arg(valid::fov(fov),        "--fov",        "-v", "set the vertical FOV in degrees");
    parser.arg(valid::camera(camView), "--camera",     "-c", "set camera position, angle, up");
    parser.arg(valid::res(res),        "--resolution", "-r", "set resolution widthxheight");
    parser.opt(preview,                "--preview",    "-p", "enable preview images");
    parser.opt(DEBUG,                  "--debug",      "-d", "enable debug messages");
    parser.parse(argc, argv);

    debug << "Running ray tracer in debug mode..." << endl
        << endl << "[Configuration]"
        << endl << " FORMAT:   " << format
        << endl << " OUTPUT:   " << out
        << endl << " SHADER:   " << shader.name
        << endl << " SCENE:    " << scene.name
        << endl << " AA:       " << aa.name
        << endl << " FOV:      " << fov
        << endl << " POSITION: " << vec::str(camView.from)
        << endl << " TOWARDS:  " << vec::str(camView.to)
        << endl << " VUP:      " << vec::str(camView.vup)
        << endl << " RES:      " << res.width << "x" << res.height
        << endl;

    Camera camera = camView.camera(fov, res.aspect);

    if(preview) {
        debug << endl << "[Previewing]" << endl;
        render(Resolution(res.aspect * 100, 100), camera, scene, shader, aa::none).out(format, "preview." + format);
        debug << endl;
    }

    debug << endl << "[Rendering]" << endl;
    render(res, camera, scene, shader, aa).out(format, out);
    debug << endl;
}