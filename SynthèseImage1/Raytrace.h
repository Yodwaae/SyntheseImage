#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "SyntheseImage.h"

using namespace std;


// TODO : Wrap Sphere into a struct Object
// TODO : Add the behavior to the material (metal, glass, ...) + I don't really like that I added a function to the material struct but don't think there's another way (or make material a func)
// Although good to note that color is no longer necessary as the three channel reflection will create the color, need to think about that


#pragma region ===== STRUCTURES =====


struct Material {

    public:
        Color color;
        Albedo albedo;

        Color displayedColor(const LightPower& lightIntensity) const {

            // Apply the gamma correction and the albedo to the material color
            Color res = (color * lightIntensity.GammaCorrection() ) * albedo ;

            return res;
        }
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




#pragma endregion


#pragma region ===== FUNCTIONS =====

#pragma region === INTERSECTIONS ===

tuple<const Sphere*, double> rayIntersectSpheres(const Ray& ray, const vector<Sphere>& spheres);
LightPower lightsIntersectSpheres(const vector<Light>& lights, const Ray& ray, const Sphere& sphere, const vector<Sphere>& spheres, double intersectDistance);
vector<Color> computeSpheresIntersect(const vector<Light>& lights, const vector<Sphere>& spheres, double cameraOpening, int WIDTH, int HEIGHT, Color backgroundColor = { 255, 0, 220 });

#pragma endregion

int writeImage(const string& filename, int width, int height, const vector<Color>& vec);

#pragma endregion