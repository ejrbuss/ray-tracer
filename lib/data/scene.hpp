# pragma once

#include "lib/render/light.hpp"
#include "lib/render/body.hpp"

using namespace std;

class Scene;

namespace scene { vector<Scene*> scenes; }

class Scene {

    public:
        vector<Body>  bodies;
        vector<Light> lights;
        Body          world;
        string        name;

        Scene(const string & n, const vector<Body> & b, const vector<Light> & l) :
            name(n),
            bodies(b),
            lights(l),
            world(body::aggregate(bodies))
        {
            scene::scenes.push_back(this);
        }
};

namespace scene {

    auto get(const string & name) -> const Scene {
        for(const Scene* scene: scenes) {
            if(equal(name, (*scene).name)) {
                return *scene;
            }
        }
        fail(name + " is not a valid scene name.");
    }

    const Scene scene1("scene1", vector<Body> {
        body::plane(Vec(0,-0.5,0), Vec(0,1,0), material::fbrass),  // bottom
        body::sphere(Vec(0,0,-1), 0.5,         material::fbrass),
        body::sphere(Vec(1,0,-5), 0.5,         material::fbrass),
    }, vector<Light> {
        Light(Vec(-0.5, 2, 0), 0.2),
        Light(Vec( 0.5, 2, 0), 0.2)
    });

    const Scene scene2("scene2", vector<Body> {
        body::sphere(Vec( 0, -100.5, -1), 100, material::scatterLambertian(Vec(0.8, 0.8, 0))),
        body::sphere(Vec( 0, 0, -1),      0.5, material::scatterLambertian(Vec(0.8, 0.3, 0.3))),
        body::sphere(Vec(-1, 0, -1),      0.5, material::scatterMetal(Vec(0.8, 0.8, 0.8), 0.1)),
        body::sphere(Vec( 1, 0, -1),      0.5, material::scatterMetal(Vec(0.8, 0.6, 0.2), 0.4))
    }, vector<Light> {
        Light(Vec(-0.5, 0, 0), 0.2)
    });

    const f32 R = cos(PI / 4.0);
    const Scene scene3("scene3", vector<Body> {
        body::sphere(Vec(-R, 0, -1), R, material::scatterLambertian(Vec(0, 0, 1))),
        body::sphere(Vec( R, 0, -1), R, material::scatterLambertian(Vec(1, 0, 0)))
    }, vector<Light> {
        Light(Vec(-0.5, 0.5, 0), 0.2)
    });

    // Triforce
    const Scene triforce("triforce", vector<Body> {
        body::triangle(Vec(-0.5, 0, -1), Vec(-1,   -1, -1), Vec( 0,   -1, -1), material::triforce),
        body::triangle(Vec( 0.5, 0, -1), Vec( 0,   -1, -1), Vec( 1,   -1, -1), material::triforce),
        body::triangle(Vec( 0,   1, -1), Vec(-0.5,  0, -1), Vec( 0.5,  0, -1), material::triforce)
    }, vector<Light> {
        Light(Vec(0, 1, 0), 0.5)
    });

    // Spheres and Planes

    const Scene spheresAndPlanes("spheres-and-planes", vector<Body> {
        body::plane(Vec(0,-2,0), Vec(0,1,0), material::fbrass),
        body::sphere(Vec(-1, -1, -2), 0.4, material::scatterLambertian(Vec(1.0, 0.5, 0.0))),
        body::sphere(Vec( 0, -1, -2), 0.4, material::scatterLambertian(Vec(0.5, 1.0, 0.0))),
        body::sphere(Vec( 1, -1, -2), 0.4, material::scatterLambertian(Vec(  0, 0.5, 1.0))),
        body::sphere(Vec(-1,  0, -2), 0.4, material::scatterLambertian(Vec(1.0, 0.5, 0.0))),
        body::sphere(Vec( 0,  0, -2), 0.4, material::fbrass),
        body::sphere(Vec( 1,  0, -2), 0.4, material::scatterLambertian(Vec(  0, 0.5, 1.0))),
        body::sphere(Vec(-1,  1, -2), 0.4, material::scatterLambertian(Vec(1.0, 0.5, 0.0))),
        body::sphere(Vec( 0,  1, -2), 0.4, material::scatterLambertian(Vec(0.5, 1.0, 0.0))),
        body::sphere(Vec( 1,  1, -2), 0.4, material::scatterLambertian(Vec(  0, 0.5, 1.0))),
    }, vector<Light> {
        Light(Vec(0, 1, 0), 0.3)
    });

    // Box Scene

    const Vec box1_center = Vec(0.15, 0.15, 0);
    const Vec box2_center = Vec(0.15, 0.3,  0);
    const f32 box1_angle = radians(10);
    const f32 box2_angle = radians(60);
    const Mat box1_rotate(
         cos(box1_angle), 0, sin(box1_angle),
         0,               1, 0,
        -sin(box1_angle), 0, cos(box1_angle)
    );
    const Mat box2_rotate(
         cos(box2_angle), 0, sin(box2_angle),
         0,               1, 0,
        -sin(box2_angle), 0, cos(box2_angle)
    );

    auto box1_transform(Vec v) -> Vec {
        v -= box1_center;
        v =  box1_rotate * v;
        v += box1_center + Vec(0, -0.5, -0.7);
        return v;
    }

    auto box2_transform(Vec v) -> Vec {
        v -= box2_center;
        v =  box2_rotate * v;
        v += box2_center + Vec(-0.35, -0.5, -0.8);
        return v;
    }

    const Vec b1f1v1 = box1_transform(Vec(0,   0,    0.15));
    const Vec b1f1v2 = box1_transform(Vec(0,   0.3,  0.15));
    const Vec b1f1v3 = box1_transform(Vec(0.3, 0,    0.15));

    const Vec b1f2v1 = box1_transform(Vec(0,   0.3,  0.15));
    const Vec b1f2v2 = box1_transform(Vec(0,   0.3, -0.15));
    const Vec b1f2v3 = box1_transform(Vec(0.3, 0.3,  0.15));

    const Vec b1f3v1 = box1_transform(Vec(0.3, 0,    0.15));
    const Vec b1f3v2 = box1_transform(Vec(0.3, 0.3,  0.15));
    const Vec b1f3v3 = box1_transform(Vec(0.3, 0,   -0.15));

    const Vec b2f1v1 = box2_transform(Vec(0,   0,    0.15));
    const Vec b2f1v2 = box2_transform(Vec(0,   0.6,  0.15));
    const Vec b2f1v3 = box2_transform(Vec(0.3, 0,    0.15));

    const Vec b2f2v1 = box2_transform(Vec(0,   0.6,  0.15));
    const Vec b2f2v2 = box2_transform(Vec(0,   0.6, -0.15));
    const Vec b2f2v3 = box2_transform(Vec(0.3, 0.6,  0.15));

    const Vec b2f3v1 = box2_transform(Vec(0.3, 0,    0.15));
    const Vec b2f3v2 = box2_transform(Vec(0.3, 0.6,  0.15));
    const Vec b2f3v3 = box2_transform(Vec(0.3, 0,   -0.15));

    const Scene boxScene("box-scene", vector<Body> {

        // Walls
        body::plane(Vec( 0,   0,  -1), Vec( 0,  0, 1), material::scatterLambertian(Vec(0.9, 0.9, 0.9))), // Back wall
        body::plane(Vec(-0.5, 0,   0), Vec( 1,  0, 0), material::scatterLambertian(Vec(1, 0, 0))), // Left wall
        body::plane(Vec( 0.5, 0,   0), Vec(-1,  0, 0), material::scatterLambertian(Vec(0, 1, 0))), // Right wall
        body::plane(Vec( 0,  -0.5, 0), Vec( 0,  1, 0), material::scatterLambertian(Vec(0.9, 0.9, 0.9))), // Floor
        body::plane(Vec( 0,   0.5, 0), Vec( 0, -1, 0), material::scatterLambertian(Vec(0.9, 0.9, 0.9))), // Cieling

        // Box1
        body::quad(b1f1v1, b1f1v2, b1f1v3, material::scatterLambertian(Vec(1, 1, 1))),
        body::quad(b1f2v1, b1f2v2, b1f2v3, material::scatterLambertian(Vec(1, 1, 1))),
        body::quad(b1f3v1, b1f3v2, b1f3v3, material::scatterLambertian(Vec(1, 1, 1))),

        // Box2
        body::quad(b2f1v1, b2f1v2, b2f1v3, material::scatterLambertian(Vec(1, 1, 1))),
        body::quad(b2f2v1, b2f2v2, b2f2v3, material::scatterLambertian(Vec(1, 1, 1))),
        body::quad(b2f3v1, b2f3v2, b2f3v3, material::scatterLambertian(Vec(1, 1, 1))),

        // Cieling "light"
        body::quad(Vec(-0.1, 0.499, -0.64), Vec(-0.1, 0.499, -0.60), Vec( 0.1, 0.499, -0.64), material::cornellLight)

    }, light::area(Light(Vec(0, 0.40, -0.62), Vec(0.3, 0.25, 0.15)), 64, 0.02));
}