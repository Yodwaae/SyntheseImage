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
class Material {

    private:
        Color _color;
        Albedo _albedo;

    public :

        #pragma region === CONSTRUCTORS ===

        // Default
        Material() : _color(255, 255, 255), _albedo(1, 1, 1){};

        // Explicit
        Material(const Color& color, const Albedo& albedo) : _color(color), _albedo(albedo) {};

        #pragma endregion

        #pragma region === FUNCTIONS ===

        Color displayedColor(const LightPower& lightIntensity) const {

            // Apply the gamma correction and the albedo to the material color // TODO This calcul might be wrong as I might apply gamma correction too early (should be applied after
            Color res = (_color * lightIntensity.GammaCorrection() ) * _albedo ;

            return res;
        }

        #pragma endregion

};

struct Ray {

    public:
        Point origin;
        NormalisedDirection direction;

};

struct Sphere {

    public:
        Point center;
        double radius;
        Material material;

};

struct Light {

    public:
        Point position;
        LightPower power;
};

struct Object {

    public :
        Sphere sphere;
};

#pragma endregion


#pragma region ===== FUNCTIONS =====

#pragma region === INTERSECTIONS ===

tuple<const Sphere*, double> rayIntersectSpheres(const Ray& ray, const vector<Sphere>& spheres);
LightPower lightsIntersectSpheres(const vector<Light>& lights, const Ray& ray, const Sphere& sphere, const vector<Sphere>& spheres, double intersectDistance);
vector<Color> computeSpheresIntersect(const vector<Light>& lights, const vector<Sphere>& spheres, double cameraOpening, int WIDTH, int HEIGHT, Color backgroundColor = { 255, 0, 220 });

#pragma endregion

int writeImage(const string& filename, int width, int height, const vector<Color>& vec);

#pragma endregion