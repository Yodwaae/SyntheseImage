#include <iostream>
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

double rayIntersectSphere(const Ray& ray, const Sphere& sphere) {

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

    /* The return logic is :
        - delta is < 0, there's no solution to the system return -1 to signify it (no intersection)
        - else it means there at least one solution to the system, in that case :
            * return the smallest t that is > 0 (t0 will always be smaller than t1 so we start by that one)
            * else return 0 (intersection was behind the camera)
    */
    if (delta < 0)
        return -1;
    else if (t0 >= 0)
        return t0;
    else if (t1 >= 0)
        return t1;
    else
        return 0;

}

LightPower lightIntersectSphere(const Light& light, const Ray& ray, const Sphere& hitSphere, const vector<Sphere>& spheres, double intersectDistance) {

    // Initialisation (values for cosine and attenuation)
    Point intersectionPoint = ray.origin + (ray.direction * intersectDistance);
    NormalisedDirection dirToLight = intersectionPoint.NormalisedDirectionTo(light.position);
    NormalisedDirection normal = hitSphere.center.NormalisedDirectionTo(intersectionPoint);
    double lightDistanceSquared = intersectionPoint.SquaredDistanceTo(light.position);

    // Compute cosine of angle between surface normal and light direction
    double lightAngle = normal.dot(dirToLight);

    // Compute attenuation (inverse-square law) and final light intensity
    LightPower attenuation = light.power / lightDistanceSquared;
    LightPower lightIntensity =  attenuation * lightAngle;

    // Test for casted shadow
    bool canSeeLightSource = true;

    Ray shadowRay{
        intersectionPoint + BIAS * dirToLight,
        dirToLight
    };


    for (const Sphere& other : spheres) {

        // NOTE Could Ignore self but in the future we might have object that cast shadow on themselves

        double lightIntersect = rayIntersectSphere(shadowRay, other);

        if (pow(lightIntersect, 2) < lightDistanceSquared && lightIntersect > 0) {
            canSeeLightSource = false;
            break;
        }
    }

    if (!canSeeLightSource)
        lightIntensity = lightIntensity * 0;


    return lightIntensity;
}

// REFACTO : Divide in multiple functions ? (cleaner but potentially more overhead OR I use static inline for the "helpers" functions ?)
vector<Color> computeSpheresIntersect(const Light& light, const vector<Sphere>& spheres, double cameraOpening, int WIDTH, int HEIGHT, Color backgroundColor) {
    
    // Initialisation
    vector<Color> colVec(WIDTH * HEIGHT);
    double verticalOffset = WIDTH / 2;
    double horizontalOffset = HEIGHT / 2;

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {

            // Loop Initialisation
            double nearestDist = INFINITY;
            Color colorValue = backgroundColor;
            const Sphere* hitSphere = nullptr;

            // Compute the ray direction for this pixel in camera space
            Point pNearPlane = Point(x, y, 0);
            Point pNearPlanePrime = Point(x - verticalOffset, y - horizontalOffset, 0);
            Point pFarPlane = Point((x - verticalOffset) * cameraOpening, (y - horizontalOffset) * cameraOpening, 1);
            NormalisedDirection planeDistance = pNearPlanePrime.NormalisedDirectionTo(pFarPlane);
            Ray ray{ pNearPlane, planeDistance };
            
            // For this pixel try to see if there's an interesect with a sphere
            // If there's multiple intersections keep the closest hit that is > 0
            for (const Sphere& sphere : spheres) {
                double intersectionDist = rayIntersectSphere(ray, sphere);

                // Should be -1 if no hit but it's best being cautious and test for <= 0
                if (intersectionDist > 0 && intersectionDist < nearestDist) {
                    nearestDist = intersectionDist;
                    hitSphere = &sphere;
                }
            }
            
            // If a sphere is hit set the color based on material and light intensity
            // Else set the color to background/missing texture
            if (hitSphere) {
                LightPower lightIntensity = lightIntersectSphere(light, ray, *hitSphere, spheres, nearestDist);
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