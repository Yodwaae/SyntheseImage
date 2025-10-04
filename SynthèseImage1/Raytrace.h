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

#pragma endregion


#pragma region ===== FUNCTIONS =====

double rayIntersectSphere(Ray ray, Sphere sphere);
vector<Color> computeSphereIntersect(Sphere sphere, size_t WIDTH, size_t HEIGHT);
int writeImage(const string& filename, int width, int height, const vector<Color>& vec);

#pragma endregion