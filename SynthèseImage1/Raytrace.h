#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "SyntheseImage.h"

using namespace std;

// TODO : Wrap Sphere into a struct Object
#pragma region ===== STRUCTURES =====

enum Behavior {
    Mirror,
    Diffuse,
    Glass,
};


struct Ray {

    public:
        Point origin;
        NormalisedDirection direction;

};

struct Light {

    public:
        Point position;
        Color color;
        double power;
};

// TODO Add the behavior (metal, glass, ...)
// TODO Should rename the class albedo to material and get rid of this material class (Well not sure about that actually as the material also take into account the behavior)
class Material {

private:
    Albedo _albedo;
    Behavior _behavior;

public:

#pragma region === CONSTRUCTORS ===
    
    // TODO : Do I add a constructor with only albedo that default behavior to diffuse ? Idem for only behavior ? (The first might be useful in some cases but the second one seems uselesss honestly)

    // Default
    Material() : _albedo(1, 1, 1), _behavior(Diffuse) {};

    // Explicit
    Material(const Albedo& albedo, Behavior behavior) : _albedo(albedo), _behavior(behavior) {};

#pragma endregion

#pragma region === FUNCTIONS ===

    #pragma region GETTERS

    inline Behavior getBehavior() const { return _behavior; }

    inline Albedo getAlbedo() const { return _albedo; }

    #pragma endregion

#pragma endregion

};


struct Sphere {

    public:
        Point center;
        double radius;
        Material material;

};


struct Object {

    public :
        Sphere sphere;
};

#pragma endregion


#pragma region ===== FUNCTIONS =====

#pragma region === INTERSECTIONS ===

tuple<const Sphere*, double> rayIntersectSpheres(const Ray& ray, const vector<Sphere>& spheres);
Color lightsIntersectSpheres(const vector<Light>& lights, const Ray& ray, const Sphere& hitSphere, const vector<Sphere>& spheres, double intersectDistance);
vector<Color> computeSpheresIntersect(const vector<Light>& lights, const vector<Sphere>& spheres, double cameraOpening, int WIDTH, int HEIGHT, Color backgroundColor = { 255, 0, 220 });

#pragma endregion

int writeImage(const string& filename, int width, int height, const vector<Color>& vec);

#pragma endregion