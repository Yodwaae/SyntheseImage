#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "SyntheseImage.h"

using namespace std;

#pragma region ===== STRUCTURES =====

struct Ray {

    public:
        Point origin;
        Direction direction;

};

struct Sphere {

    public:
        Point center;
        double radius;

};

struct Light {

    public:
        Point position;
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