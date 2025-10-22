#include <iostream>
#include <tuple>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <cmath>
#include <optional>
#include "Raytrace.h"

using namespace std;

#pragma region ========== GLOBALS ==========

static constexpr double BIAS = 0.1;

#pragma endregion

#pragma region ===== FUNCTIONS =====

// TODO Refacto and improve the way the function chain each other, where some part of the logic are
// TODO Create a RayIntersect Object (should it wrap or replace rayIntersectSphere ?)

tuple<const Sphere*, double> rayIntersectSpheres(const Ray& ray, const vector<Sphere>& spheres) {
    // Loop Initialisation
    double nearestDist = INFINITY;
    const Sphere* hitSphere = nullptr;

    // For this pixel try to see if there's an interesect with a sphere
    // If there's multiple intersections keep the closest hit that is > 0
    for (const Sphere& sphere : spheres) {
        double intersectionDist = - INFINITY;

        // Shorthand
        double radius = sphere.radius;
        Point center = sphere.center;
        Point origin = ray.origin;
        Direction direction = ray.direction;

        //Initialisation
        Direction oc = origin.DirectionTo(center);
        double r2 = radius * radius;

        // Setting the 3 terms of que equation system
        double a = direction.dot(direction);
        double b = -2 * direction.dot(oc);
        double c = oc.dot(oc) - r2;

        // Solving the system
        double delta = (b * b) - (4 * a * c);
        double t0 = (-b - sqrt(delta)) / (2 * a);
        double t1 = (-b + sqrt(delta)) / (2 * a);

        if (t0 >= 0) intersectionDist = t0;
        else if (t1 >= 0) intersectionDist = t1;

        // Should be -1 if no hit but it's best being cautious and test for <= 0
        if (intersectionDist > 0 && intersectionDist < nearestDist) {
            nearestDist = intersectionDist;
            hitSphere = &sphere;
        }
    }

    return make_tuple(hitSphere, nearestDist);
}

// TODO Actually not tracing two times as light -> point is more of a geometric computation, but still should see if I could fold that to point -> light tracing
LightPower lightsIntersectSpheres(const vector<Light>& lights, const Ray& ray, const Sphere& hitSphere, const vector<Sphere>& spheres, double intersectDistance) {

    // Initialisation
    Point intersectionPoint = ray.origin + (ray.direction * intersectDistance);
    NormalisedDirection normal = hitSphere.center.NormalisedDirectionTo(intersectionPoint);
    LightPower agglomeratedLight = LightPower(0, 0, 0);

    for (const Light& light : lights) {

        // ===== LIGHTS =====

        // Light loop initialisation (values for cosine and attenuation)
        NormalisedDirection dirToLight = intersectionPoint.NormalisedDirectionTo(light.position);
        double lightDistanceSquared = intersectionPoint.SquaredDistanceTo(light.position);

        // Compute cosine of angle between surface normal and light direction
        double lightAngle = normal.dot(dirToLight);

        // Compute attenuation (inverse-square law) and final light intensity
        LightPower attenuation = light.power / lightDistanceSquared;
        LightPower lightIntensity = attenuation * lightAngle;

        // ===== SHADOWS =====

        // Shadow loop initialisation
        Ray shadowRay{ intersectionPoint + BIAS * dirToLight, dirToLight };

        // If a sphere was in between the light and the intersected sphere doesn't add it to the aglomerated light
        auto [throwAway, intersectDist] = rayIntersectSpheres(shadowRay, spheres);
        if (intersectDist * intersectDist < lightDistanceSquared)
            continue;

        // Add the light to the total light
        agglomeratedLight +=  lightIntensity;
    }

    return agglomeratedLight;
}

// REFACTO : Divide in multiple functions ? (cleaner but potentially more overhead OR I use static inline for the "helpers" functions ?)
vector<Color> computeSpheresIntersect(const vector<Light>& lights, const vector<Sphere>& spheres, double cameraOpening, int WIDTH, int HEIGHT, Color backgroundColor) {
    
    // Initialisation
    // TODO Better way to manage the camera offset ?
    vector<Color> colVec(WIDTH * HEIGHT);
    double verticalOffset = WIDTH / 2;
    double horizontalOffset = HEIGHT / 2;

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            // Loop initialisation
            Color colorValue = backgroundColor;

            // Compute the ray direction for this pixel in camera space
            Point pNearPlane = Point(x, y, 0);
            Point pNearPlanePrime = Point(x - verticalOffset, y - horizontalOffset, 0);
            Point pFarPlane = Point((x - verticalOffset) * cameraOpening, (y - horizontalOffset) * cameraOpening, 1);
            NormalisedDirection planeDistance = pNearPlanePrime.NormalisedDirectionTo(pFarPlane);
            Ray ray{ pNearPlane, planeDistance };
            
            // Sphere Intersection with Camera
            auto [hitSphere, dist] = rayIntersectSpheres(ray, spheres);

            // If a sphere is hit set the color based on material and light intensity
            // Else set the color to background/missing texture
            if (hitSphere) {
                LightPower lightIntensity = lightsIntersectSpheres(lights, ray, *hitSphere, spheres, dist);
                colorValue = hitSphere->material.displayedColor(lightIntensity);
                colVec[y * WIDTH + x] = colorValue;
            }
            else
                colVec[y * WIDTH + x] = backgroundColor;
        
        }
    }

    return colVec;
}

int writeImage(const string& filename, int width, int height, const vector<Color>& vec) {

    // Creating file
    ofstream out(filename);

    if (!out) {
        throw ios_base::failure("Was unable to create output file : " + filename);
    }

    // Writing the PPM Header (Mode, size, color range)
    out << "P3\n" << width << " " << height << "\n255\n";


    // Writing the pixels to the file
    for (int i = 0; i < height; i++) {

        for (int j = 0; j < width; j++) {

            int index = i * width + j;
            Color color = vec[index];
            out << color.getRed() << " " << color.getGreen() << " " << color.getBlue() << " ";

        }

        // Go to the next line
        out << "\n";
    }

    // Close file and return success
    out.close();
    return 0;
}

#pragma endregion