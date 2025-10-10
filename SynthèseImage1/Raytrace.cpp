#include <iostream>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <cmath>
#include "Raytrace.h"

using namespace std;

#pragma region ===== FUNCTIONS =====

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

// TODO In the future directly return a color instead of a double
double lightIntersectSphere(const Light& light, const Ray& ray, const Sphere& sphere, double intersectDistance) {

    Point x = ray.origin + (ray.direction * intersectDistance);

    NormalisedDirection directionToLight = x.NormalisedDirectionTo(light.position);
    NormalisedDirection sphereNormal = x.NormalisedDirectionTo(sphere.center);

    // TODO Most likely need an abs value but let's just test without to see
    double coef = sphereNormal.dot(directionToLight);

    double pixelColor = coef * intersectDistance;

    return pixelColor;
}

vector<Color> computeSpheresIntersect(const Light& light, const vector<Sphere>& spheres, size_t WIDTH, size_t HEIGHT) {
    
    // Initialisation
    vector<Color> colVec(WIDTH * HEIGHT);

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {

            // Loop Initialisation
            Ray ray{ Point(x, y , 0), Direction(0, 0, 1) };
            double nearestDist = INFINITY;
            
            // For each pixel try to see if there's an interesct with a sphere
            // If there's multiple intersections keep the closest hit that is > 0
            for (const Sphere& sphere : spheres) {
                double intersectionDist = rayIntersectSphere(ray, sphere);

                // Should be -1 if no hit but it's best being cautious and test for <= 0
                if (intersectionDist > 0 && intersectionDist < nearestDist)
                    nearestDist = intersectionDist;
            }
            

            // TODO : only temp for test, need to think about how to implement it correctly
            if (nearestDist == INFINITY)
                colVec[y * WIDTH + x] = Color(255, 0, 0);
            else
                colVec[y * WIDTH + x] = Color(lightIntersectSphere(light, ray, spheres.front(), nearestDist));

            // If no hit set color as background (red) else set color as distance (clamped to 255)
            //colVec[y * WIDTH + x] = (nearestDist == INFINITY) ? Color(255, 0, 0) : Color(nearestDist);
        
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