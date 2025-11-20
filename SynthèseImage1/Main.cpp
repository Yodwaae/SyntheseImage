
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
    Sphere rightWallSphere{ Point(wallSphereRadius + WIDTH, HEIGHT/2, 0), wallSphereRadius, Material( Albedo(0, 1, 0), Diffuse) };
    Sphere leftWallSphere{ Point(-wallSphereRadius, HEIGHT/2, 0), wallSphereRadius, Material(Albedo(1, 0, 0), Diffuse) };
    Sphere upWallSphere{ Point(WIDTH/2, -wallSphereRadius, 0), wallSphereRadius, Material(Albedo(1, 1, 1), Diffuse) };
    Sphere downWallSphere{ Point(WIDTH/2, wallSphereRadius + HEIGHT, 0), wallSphereRadius, Material(Albedo(1, 1, 1), Diffuse) };
    Sphere backWallSphere{ Point(WIDTH/2, HEIGHT/2, DEPTH + wallSphereRadius), wallSphereRadius, Material(Albedo(1, 1, 1), Diffuse) };
    Sphere frontWallSphere{ Point(WIDTH/2, HEIGHT/2, -wallSphereRadius), wallSphereRadius, Material(Albedo(1, 1, 1), Diffuse) };
    // Spheres creation
    Sphere sphere1{ Point(150, 350, 250), 80, Material(Albedo(0, 0, 1), Mirror)};
    Sphere sphere2{ Point(350, 350, 250), 80, Material(Albedo(1, 1, 1), Glass)};
    Sphere sphere3{ Point(250, 270, 410), 80, Material(Albedo(1, 1, 0), Diffuse)};
    // Sphere vector
    vector<Sphere> spheres = { rightWallSphere, leftWallSphere, upWallSphere, downWallSphere, backWallSphere, frontWallSphere, sphere1, sphere2, sphere3 };

    // ===== LIGHTS =====

    // Light creation
    Light light1{ Point(250, 250, 0), Color(255, 255, 255), 3000};
    Light light2{ Point(250, 150, 250), Color(255, 255, 255), 3000};
    // Light Vector
    vector<Light> lights = { light1, light2 };

    // Image creation
    vector<Color> colVec = computeSpheresIntersect(HEIGHT/2, WIDTH/2, lights, spheres, 1.001, WIDTH, HEIGHT);
    writeImage("sphere.ppm", WIDTH, HEIGHT, colVec);

    return 0;
}