
#include <vector>
#include "SyntheseImage.h"
#include "Raytrace.h"

using namespace std;

int main()
{
    constexpr size_t WIDTH = 500;
    constexpr size_t HEIGHT = 500;

    // Image creation
    Sphere sphere{ Point(250, 250, 300), 200 };
    vector<Color> colVec = computeSphereIntersect(sphere, WIDTH, HEIGHT);
    writeImage("sphere.ppm", WIDTH, HEIGHT, colVec);

    return 0;
}