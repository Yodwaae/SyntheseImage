
#include <vector>
#include "SyntheseImage.h"
#include "Raytrace.h"

using namespace std;

int main()
{
    // NOTE Obviously everything is hardcoded right now as it just debugging and testing

    // Constants def
    constexpr size_t WIDTH = 500;
    constexpr size_t HEIGHT = 500;
    constexpr int wallSphereRadius = 5000;

    // Wall sphere (used to simulate a box
    Sphere rightWallSphere{ Point(wallSphereRadius + 500, 250, 0), wallSphereRadius, Material{Color(0, 255, 0), Albedo(1, 1, 1)} };
    Sphere leftWallSphere{ Point(-wallSphereRadius, 250, 0), wallSphereRadius, Material{Color(255, 0, 0), Albedo(1, 1, 1)} };
    Sphere upWallSphere{ Point(250, -wallSphereRadius, 0), wallSphereRadius, Material{Color(255, 255, 255), Albedo(1, 1, 1)} };
    Sphere downWallSphere{ Point(250, wallSphereRadius + 500, 0), wallSphereRadius, Material{Color(255, 255, 255), Albedo(1, 1, 1)} };
    Sphere backWallSphere{ Point(250, 250, 500 + wallSphereRadius), wallSphereRadius, Material{Color(255, 255, 255), Albedo(1, 1, 1)} };

    // Spheres creation
    Sphere sphere1{ Point(150, 350, 250), 80, Material{Color(0, 0, 255), Albedo(1, 1, 1)} };
    Sphere sphere2{ Point(350, 350, 250), 80, Material{Color(255, 0, 255), Albedo(1, 1, 1)} };
    Sphere sphere3{ Point(250, 270, 410), 80, Material{Color(255, 255, 0), Albedo(1, 1, 1)} };

    // Sphere vector
    vector<Sphere> spheres = { rightWallSphere, leftWallSphere, upWallSphere, downWallSphere, backWallSphere, sphere1, sphere2, sphere3 };

    // Light creation
    Light light{ Point(250, 250, 0), 100000};

    // Image creation
    vector<Color> colVec = computeSpheresIntersect(light, spheres, 1.001, WIDTH, HEIGHT);
    writeImage("sphere.ppm", WIDTH, HEIGHT, colVec);

    return 0;
}