#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "Raytrace.h"

using namespace std;

#pragma region ===== FUNCTIONS =====

double rayIntersectSphere(Ray ray, Sphere sphere) {

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

vector<Color> computeSphereIntersect(Sphere sphere, size_t WIDTH, size_t HEIGHT) {
    // Initialising color vector<>
    vector<Color> colVec(WIDTH * HEIGHT);
    Sphere sphere2{ Point(0, 250, 300), 100 };

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {

            // Computing the intersect
            Ray ray{ Point(x, y , 0), Direction(0, 0, 1) };
            double pixel1 = rayIntersectSphere(ray, sphere);
            double pixel2 = rayIntersectSphere(ray, sphere2);

            double pixel = max(pixel1, pixel2);

            // Setting pixel color depending on the result
            if (pixel < 0)
                colVec[y * WIDTH + x] = Color(255, 0, 0);// Red background
            else
                colVec[y * WIDTH + x] = Color(pixel); // Sphere

        }
    }

    return colVec;
}

int writeImage(const string& filename, int width, int height, const vector<Color>& vec) {

    // Creating file
    ofstream out(filename);

    // TODO : To replace with a true error throw
    if (!out) {
        cerr << "Can't create output file !";
        return -1;
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