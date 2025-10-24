#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "SyntheseImage.h"

using namespace std;

// TODO : Wrap Sphere into a struct Object
#pragma region ===== STRUCTURES =====

// TODO Add the behavior (metal, glass, ...)
// TODO Color might be a bit redundant but my refacto attempt prooved unsuccessful (If I succesfully remove color should I keep Material as a wrapper of albedo ? Or should I just use Albedo directly/rename it ?)

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

// TODO Should rename the class albedo to material and get rid of this material class
class Material {

private:
    Albedo _albedo;

public:

#pragma region === CONSTRUCTORS ===

    // Default
    Material() : _albedo(1, 1, 1) {};

    // Explicit
    Material(const Albedo& albedo) : _albedo(albedo) {};

#pragma endregion

#pragma region === FUNCTIONS ===

    Color displayedColor(const Light& light, const double intensity) const {

        // Apply the gamma correction and the albedo to the material color // TODO Re add the gamma correction
        Color res = light.color * _albedo * intensity;

        return res;
    }

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