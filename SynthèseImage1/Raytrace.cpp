#include <iostream>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <cmath>
#include "Raytrace.h"

using namespace std;

#pragma region ===== FUNCTIONS =====

// TODO Refacto and improve the way the function chain each other, where some part of the logic are
// TODO Create a RayIntersect Object (should it wrap or replace rayIntersectSphere ?)

double rayIntersectSphere(const Ray& ray, const Sphere& sphere) {

    // Shorthand
    double radius = sphere.radius;
    Point center = sphere.center;
    Point origin = ray.origin;
    Direction direction = ray.direction.ToDirection();
    
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

// TODO Redo the comments + fix bug that create dark spot at the most lit area
double lightIntersectSphere(const Light& light, const Ray& ray, const Sphere& sphere, double intersectDistance) {

    // Initialisation
    Point intersectionPoint = ray.origin + (ray.direction * intersectDistance);
    NormalisedDirection normalisedDirectionToLight = intersectionPoint.NormalisedDirectionTo(light.position);
    Direction directionToLight = intersectionPoint.DirectionTo(light.position);
    NormalisedDirection sphereNormal = sphere.center.NormalisedDirectionTo(intersectionPoint);
    double lightDistanceSquared = directionToLight.dot(directionToLight);

    // Cosine of angle between light and normal
    double lightIntensity = sphereNormal.dot(normalisedDirectionToLight) / lightDistanceSquared * light.power;

    return lightIntensity;
}

// TODO Clean up the implementation : - lightIntersect only on the final "dist" but with the correct sphere
// - Remove color management from the function, it should just adjust the intensity and color be managed by the sphere
// - In the future shouldn't have to worry about a placeholder background color as we will build a cornell box, but in case could had an optionnal arg for background color just in case

vector<Color> computeSpheresIntersect(const Light& light, const vector<Sphere>& spheres, size_t WIDTH, size_t HEIGHT) {
    
    // Initialisation
    vector<Color> colVec(WIDTH * HEIGHT);
    double openingCoef = 1.001; //TODO openingCoef should have a better name and be a func arg

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {

            // Loop Initialisation

            // TODO Obviously there a big work of refactoring to do here
            Point pNearPlane = Point(x, y, 0);
            Point pNearPlanePrime = Point(x - 250, y - 250, 0);
            Point pFarPlane = Point((x - 250) * openingCoef, (y - 250) * openingCoef, 1);
            NormalisedDirection planeDistance = pNearPlanePrime.NormalisedDirectionTo(pFarPlane);

            Ray ray{ pNearPlane, planeDistance };
            double nearestDist = INFINITY;
            Color pixelColor = Color(255, 0, 0);

            
            // For each pixel try to see if there's an interesct with a sphere
            // If there's multiple intersections keep the closest hit that is > 0
            for (const Sphere& sphere : spheres) {
                double intersectionDist = rayIntersectSphere(ray, sphere);

                // Should be -1 if no hit but it's best being cautious and test for <= 0
                if (intersectionDist > 0 && intersectionDist < nearestDist) {
                    nearestDist = intersectionDist;

                    // TODO Better implementation but still not optimal as it does the light intersection even with pixel that will be overwritten just after
                    pixelColor = Color(sphere.color * lightIntersectSphere(light, ray, sphere, nearestDist));
                }
            }
            
            // Set the Color
            colVec[y * WIDTH + x] = pixelColor;
        
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