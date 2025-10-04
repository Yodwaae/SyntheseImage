#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "Raytrace.h"

using namespace std;

#pragma region ===== FUNCTIONS =====

double rayIntersectSphere(Ray ray, Sphere sphere) {

    // Initialisation
    double radius = sphere.radius;
    Point center = sphere.center;
    Point origin = ray.origin;
    Direction direction = ray.direction;


    Direction oc = origin.DistanceTo(center);
    double r2 = radius * radius;


    // Setting the 3 terms of que equation system
    double a = direction.dot(direction);
    double b = -2 * direction.dot(oc);
    double c = oc.dot(oc) - r2;

    // Solving the system
    double delta = (b * b) - (4 * a * c);
    double t0 = (-b - sqrt(delta)) / (2 * a);
    double t1 = (-b + sqrt(delta)) / (2 * a);

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

    for (int x = 0; x < 500; x++) {
        for (int y = 0; y < 500; y++) {

            // Computing the intersect
            Ray ray{ Point(x, y , 0), Direction(0, 0, 1) };
            double pixel = rayIntersectSphere(ray, sphere);

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