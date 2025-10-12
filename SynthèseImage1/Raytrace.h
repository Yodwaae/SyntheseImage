#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "SyntheseImage.h"

using namespace std;


// TODO : Wrap Sphere into a struct Object
// Object will handle the color and surfaceAbsorbtion/Reflection (maybe collapse that in a Material)
// Although good to note that color is no longer necessary as the three channel reflection will create the color, need to think about that

#pragma region ===== STRUCTURES =====

struct Ray {

    public:
        Point origin;
        NormalisedDirection direction;

};

struct Sphere {

    public:
        Point center;
        double radius;
        Color color;

};

struct Light {

    public:
        Point position;
        double power; // TODO To see if I do a proper tonemap, if Light has a color or just a vec3 for power (but that would need a new class, etc ...)
};

#pragma endregion


#pragma region ===== FUNCTIONS =====

#pragma region === INTERSECTIONS ===

double rayIntersectSphere(const Ray& ray, const Sphere& sphere);
double lightIntersectSphere(const Light& light, const Ray& ray, const Sphere& sphere, double intersectDistance);
vector<Color> computeSpheresIntersect(const Light& light, const vector<Sphere>& spheres, size_t WIDTH, size_t HEIGHT);

#pragma endregion

int writeImage(const string& filename, int width, int height, const vector<Color>& vec);

#pragma endregion