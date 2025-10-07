
#include <vector>
#include "SyntheseImage.h"
#include "Raytrace.h"

using namespace std;

int main()
{
    // Constants def
    constexpr size_t WIDTH = 500;
    constexpr size_t HEIGHT = 500;

    // Spheres creation
    Sphere sphere1{ Point(250, 250, 300), 200 };
    Sphere sphere2{ Point(0, 250, 300), 100 };
    Sphere sphere3{ Point(500, 250, 300), 100 };
    vector<Sphere> spheres = { sphere1, sphere2, sphere3 };

    // Image creation
    vector<Color> colVec = computeSpheresIntersect(spheres, WIDTH, HEIGHT);
    writeImage("sphere.ppm", WIDTH, HEIGHT, colVec);

    return 0;
}