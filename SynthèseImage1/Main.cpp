
#include <vector>
#include "SyntheseImage.h"
#include "Raytrace.h"

using namespace std;

int main()
{
    // NOTE Obviously everything is hardcoded right now as it just debugging and testing

    // Constants def
    constexpr int WIDTH = 500;
    constexpr int HEIGHT = 500;
    constexpr int DEPTH = 500;
    constexpr int wallSphereRadius = 5000;

    // ===== SPHERES =====

    // Wall sphere (used to simulate a box
    Sphere rightWallSphere{ Point(wallSphereRadius + WIDTH, HEIGHT/2, 0), wallSphereRadius, Material{Color(0, 255, 0), Albedo(1, 1, 1)} };
    Sphere leftWallSphere{ Point(-wallSphereRadius, HEIGHT/2, 0), wallSphereRadius, Material{Color(255, 0, 0), Albedo(1, 1, 1)} };
    Sphere upWallSphere{ Point(WIDTH/2, -wallSphereRadius, 0), wallSphereRadius, Material{Color(255, 255, 255), Albedo(1, 1, 1)} };
    Sphere downWallSphere{ Point(WIDTH/2, wallSphereRadius + HEIGHT, 0), wallSphereRadius, Material{Color(255, 255, 255), Albedo(1, 1, 1)} };
    Sphere backWallSphere{ Point(WIDTH/2, HEIGHT/2, DEPTH + wallSphereRadius), wallSphereRadius, Material{Color(255, 255, 255), Albedo(1, 1, 1)} };
    // Spheres creation
    Sphere sphere1{ Point(150, 350, 250), 80, Material{Color(0, 0, 255), Albedo(1, 1, 1)} };
    Sphere sphere2{ Point(350, 350, 250), 80, Material{Color(255, 255, 255), Albedo(1, 1, 1)} };
    Sphere sphere3{ Point(250, 270, 410), 80, Material{Color(255, 255, 0), Albedo(1, 1, 1)} };
    // Sphere vector
    vector<Sphere> spheres = { rightWallSphere, leftWallSphere, upWallSphere, downWallSphere, backWallSphere, sphere1, sphere2 };

    // ===== LIGHTS =====

    // Light creation
    Light light1{ Point(250, 250, 0), Vector3(30000, 30000, 30000)};
    Light light2{ Point(250, 250, 250), Vector3(30000, 30000, 30000)};
    // Light Vector
    vector<Light> lights = { light1, light2 };

    // Image creation
    vector<Color> colVec = computeSpheresIntersect(lights, spheres, 1.001, WIDTH, HEIGHT);
    writeImage("sphere.ppm", WIDTH, HEIGHT, colVec);

    return 0;
}