#include <iostream>
#include <random>
#include <algorithm>
#include <tuple>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <cmath>
#include <optional>
#include "Raytrace.h"
#include "SyntheseImage.h"

using namespace std;

#pragma region ========== GLOBALS ==========

static constexpr double BIAS = 0.1;
static constexpr int MAX_DEPTH = 5;

#pragma endregion

#pragma region ===== FUNCTIONS =====

// TODO Refacto and improve the way the function chain each other, where some part of the logic are
// TODO Create a RayIntersect Object (should it wrap or replace rayIntersectSphere ?)

tuple<const Sphere*, double> rayIntersectSpheres(const Ray& ray, const vector<Sphere>& spheres) {
    // Loop Initialisation
    double nearestDist = INFINITY;
    const Sphere* hitSphere = nullptr;

    // For this pixel try to see if there's an interesect with a sphere
    // If there's multiple intersections keep the closest hit that is > 0
    for (const Sphere& sphere : spheres) {
        double intersectionDist = - INFINITY;

        // Shorthand
        double radius = sphere.radius;
        Point center = sphere.center;
        Point origin = ray.origin;
        NormalisedDirection direction = ray.direction;

        //Initialisation
        Direction oc = origin.DirectionTo(center);
        double r2 = radius * radius;

        // Setting the 3 terms of que equation system
        // NOTE : Removed A from the calculations as the dot of a normal direction is 1, a is always a factor and a factor of 1 is neutral in mults (a = direction.dot(direction))
        double b = - direction.dot(oc);
        double c = oc.dot(oc) - r2;

        // Solving delta
        // NOTE : avoid doing delta sqrt 2 times (although compiler might already optimise that)
        double delta = (b * b) - c;
        double sqrtDelta = sqrt(delta); 

        // Finding the solution of the system
        double t0 = (-b - sqrtDelta);
        double t1 = (-b + sqrtDelta);

        if (t0 >= 0) intersectionDist = t0;
        else if (t1 >= 0) intersectionDist = t1;

        // Should be -1 if no hit but it's best being cautious and test for <= 0
        if (intersectionDist > 0 && intersectionDist < nearestDist) {
            nearestDist = intersectionDist;
            hitSphere = &sphere;
        }
    }

    return make_tuple(hitSphere, nearestDist);
}

// TODO Add an alpha (maybe directly in color)
// TODO Fix the duplication of mirror (in glass) + add a depth limit ?
// TODO Actually not tracing two times as light -> point is more of a geometric computation, but still should see if I could fold that to point -> light tracing
// TODO There's no coeff taken into account
Color lightsIntersectSpheres(const vector<Light>& lights, const Ray& ray, const vector<Sphere>& spheres, const Color& backgroundColor, int depth) {

    // RECURSION LIMIT
    if (depth >= MAX_DEPTH)
        return Color(0, 0, 0);

    // TODO Clean up the mess moving rayIntersectSpheres here created
    // Sphere Intersection with Camera
    auto [hitSphere, intersectDistance] = rayIntersectSpheres(ray, spheres);

    // If no sphere are hit set the color to background/missing texture
    if (hitSphere == nullptr) // TODO Do I still need a ptr here or could optionnal be better ?
        return backgroundColor;

    // Else a sphere is hit, set the color based on material and light intensity

    // Initialisation
    Point intersectionPoint = ray.origin + (ray.direction * intersectDistance);
    NormalisedDirection normal = hitSphere->center.NormalisedDirectionTo(intersectionPoint);
    Color agglomeratedLightColor = Color(0, 0, 0);

    // TODO Comments and clean/opti once I implemented the others behaviors
    switch (hitSphere->material.getBehavior())
    {

    case Diffuse: {

        for (const Light& light : lights) {

            // ========== DIRECT LIGHTING ==========

            // ===== LIGHTS =====

            // Light loop initialisation (values for cosine and attenuation)
            NormalisedDirection dirToLight = intersectionPoint.NormalisedDirectionTo(light.position);
            double lightDistanceSquared = intersectionPoint.SquaredDistanceTo(light.position);

            // Compute cosine of angle between surface normal and light direction
            double lightAngle = normal.dot(dirToLight);

            // NOTE : Max is a security in case of a negative light (now that we add the lights), though I think the color clamping should be enough to avoid problem, so maybe I remove it to save some perfs ?
            // Compute attenuation (inverse-square law) and finalg light intensity
            double attenuation = max(0.0, light.power / lightDistanceSquared);
            double lightIntensity = attenuation * lightAngle;

            // ===== SHADOWS =====

            // Shadow loop initialisation
            Ray shadowRay{ intersectionPoint + BIAS * dirToLight, dirToLight };

            // If a sphere was in between the light and the intersected sphere doesn't add it to the aglomerated light
            auto [throwAway, intersectDist] = rayIntersectSpheres(shadowRay, spheres);
            if (intersectDist * intersectDist < lightDistanceSquared)
                continue;

            Color directLightContrib = light.color.computeDiffuseColor(hitSphere->material.getAlbedo(), lightIntensity);


            // ========== INDIRECT LIGHTING ==========
            // 
            // Random dir for indirect light
            double x = rand();
            double y = rand();
            double z = rand();

            // Get the reflected direction and create a nex ray with it 
            NormalisedDirection indirectDirection{ x, y, z };

            if (!sameSide(normal, indirectDirection, ray.direction.flipDirection()))
                indirectDirection = indirectDirection.flipDirection().Normalise();

            Ray indirectRay = Ray{ intersectionPoint + EPSILON * indirectDirection, indirectDirection }; 
            Color indirectLightContrib = lightsIntersectSpheres(lights, indirectRay, spheres, backgroundColor, depth + 1);


            // Add the light to the total light
            agglomeratedLightColor += (directLightContrib + indirectLightContrib)/2; // TODO Shouldn't be divided by two but instead use the coef (not yet implemented)
        }

        break;
    }

    case Mirror: {  
        // Get the reflected direction and create a nex ray with it 
        NormalisedDirection reflectedDirection = Albedo::Reflect(normal, ray.direction);
        Ray reflectedRay = Ray{ intersectionPoint + EPSILON * reflectedDirection, reflectedDirection }; // TODO Create a function that manages offsetting by espilon ?
        agglomeratedLightColor += lightsIntersectSpheres(lights, reflectedRay, spheres, backgroundColor, depth + 1);

        break;
    }

    case Glass: {
        // TODO Improve and clean up
        // Get the reflected direction and create a nex ray with it 
        double ior = 1.5; // TODO Add the IOR to the material
        bool outside = false;

        if (ray.direction.dot(normal) < 0) //TODO Move to the refract function
            outside = true;

        auto [coef,maybeTransDir] = Albedo::Refract(ior, normal, ray.direction, outside);

        if (!maybeTransDir) {
            // Get the reflected direction and create a nex ray with it 
            NormalisedDirection reflectedDirection = Albedo::Reflect(normal, ray.direction);
            Ray reflectedRay = Ray{ intersectionPoint + EPSILON * reflectedDirection, reflectedDirection }; // TODO Create a function that manages offsetting by espilon ?
            agglomeratedLightColor += lightsIntersectSpheres(lights, reflectedRay, spheres, backgroundColor, depth + 1);
        }
        if (maybeTransDir) {
            NormalisedDirection transmittedDirection = maybeTransDir.value();
            Ray transmittedRay = Ray{ intersectionPoint + EPSILON * transmittedDirection, transmittedDirection }; // TODO Create a function that manages offsetting by espilon ?
            agglomeratedLightColor += lightsIntersectSpheres(lights, transmittedRay, spheres, backgroundColor, depth + 1);
        }


        break;
    }

    default:
        break;
    }

    return agglomeratedLightColor;
}

// REFACTO : Divide in multiple functions ? (cleaner but potentially more overhead OR I use static inline for the "helpers" functions ?)
vector<Color> computeSpheresIntersect(const vector<Light>& lights, const vector<Sphere>& spheres, double cameraOpening, int WIDTH, int HEIGHT, Color backgroundColor) {
    
    // Initialisation
    // TODO Better way to manage the camera offset ?
    vector<Color> colVec(WIDTH * HEIGHT);
    double verticalOffset = HEIGHT / 2;
    double horizontalOffset = WIDTH / 2;
    int nbSamples = 4;

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            // Loop initialisation
            Color colorValue = Color(0, 0, 0); // TODO Change Implementation so that background color works with my appraoch

            // Sampling
            for (int i = 0; i < nbSamples; i++) {

                // Random delta for the ray for the sampling (anti-aliasing)
                double dx = ((rand() / (double)RAND_MAX) - 0.5);
                double dy = ((rand() / (double)RAND_MAX) - 0.5);

                // Compute the ray direction for this pixel in camera space
                Point pNearPlane = Point(x + dx, y + dy, 0);
                Point pNearPlanePrime = Point(x + dx - verticalOffset, y + dy - horizontalOffset, 0);
                Point pFarPlane = Point((x + dx - verticalOffset) * cameraOpening, (y + dy - horizontalOffset) * cameraOpening, 1);
                NormalisedDirection planeDistance = pNearPlanePrime.NormalisedDirectionTo(pFarPlane);
                Ray ray{ pNearPlane, planeDistance };
                colorValue += lightsIntersectSpheres(lights, ray, spheres, backgroundColor, 0) * (1.0 / nbSamples);

            }
                
            // Normalise the result based on the sampling
            colVec[y * WIDTH + x] = colorValue;
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
            //Color color = vec[index]; // TO OPTI I don't know which approach is better
            //out << color.getRed() << " " << color.getGreen() << " " << color.getBlue() << " ";

            out << vec[index].getRed() << " " << vec[index].getGreen() << " " << vec[index].getBlue() << " ";

        }

        // Go to the next line
        out << "\n";
    }

    // Close file and return success
    out.close();
    return 0;
}

bool sameSide(NormalisedDirection normal, Direction vector1, Direction vector2) { return (normal.dot(vector1) * normal.dot(vector2) > 0); }

#pragma endregion