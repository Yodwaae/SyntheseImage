
#include <iostream>
#include <fstream>
#include <vector> // To be replaced with my own implementation
#include "SyntheseImage.h"
#include <cmath>

// TODO Harmonise the usage of the float and double


using namespace std;

// TEMP forward declarations (will become useless once moved to an other file)
int writeImage(const string& filename, int width, int height, const vector<Color>& vec);
struct Ray;
struct Sphere;

int main()
{

    vector<Color> vec{ Color(255) };
    writeImage("test.ppm", 1, 1, vec);

    return 0;
}

// To be moved to an other file later
#pragma region !!!!!! TEMPORARLY IN FILE !!!!!! 
struct Ray {

    public:
        Point origin;
        Vector3 direction;

};


struct Sphere {

    public:
        Point center;
        float radius;

};

// TODO Temporary auto
double rayIntersectSphere(Ray ray, Sphere sphere) {

    // Initialisation
    double radius = sphere.radius;
    Point center = sphere.center;
    Point origin = ray.origin;
    Vector3 direction = ray.direction;


    double oc = origin.DistanceTo(center);
    double r2 = radius * radius;


    // Setting the 3 terms of que equation system
    double a = direction.dot();
    double b = direction.dot(oc);
    double c = oc * oc - r2;

    // Solving the system
    double delta = (b * b) - (4 * a * c);
    double t0 = (-b - sqrt(delta)) / (2 * a);
    double t1 = (-b + sqrt(delta)) / (2 * a);

    if (delta < 0)
        return NULL;
    else if (t0 >= 0)
        return t0;
    else if (t1 >= 0)
        return t1;
    else
        return 0;

}


int writeImage(const string& filename, int width, int height, const vector<Color>& vec) {
    //NOTE Right now only create binary image, will replace with a vector of color instead of float in the future

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
    out.close();
    return 0;
}

#pragma endregion

#pragma region ========== VECTOR3 CLASS ==========

        #pragma region ===== CONSTRUCTORS =====
        
        // Default
        Vector3::Vector3():
        _a(0), _b(0), _c(0){}

        // From scalar
        Vector3::Vector3(double scal):
        _a(scal), _b(scal), _c(scal){}

        // Explicit
        Vector3::Vector3(double x, double y, double z):
        _a(x), _b(y), _c(z){}

        // Copy constructor (declared just for clarity sake as it's only a value copy, compiler created one would have worked this fine)
        Vector3::Vector3(const Vector3& other):
        _a(other._a), _b(other._b), _c(other._c){}

        #pragma endregion
        
        #pragma region ===== OPERATORS =====

        #pragma region === ARITHMETIC OPERATORS ===

        //HINT : If dividing by 0 will not crash but instead return +inf/-inf or NaN depending on the case

        #pragma region PURE/VALUE OPERATORS

        // ADDITION
        Vector3 Vector3::operator+(const Vector3& other) const {
            double newA = _a + other._a;
            double newB = _b + other._b;
            double newC = _c + other._c;


            return Vector3(newA, newB, newC);
        }

        // SUBSTRACTION
        Vector3 Vector3::operator-(const Vector3& other) const {
            double newA = _a - other._a;
            double newB = _b - other._b;
            double newC = _c - other._c;

            return Vector3(newA, newB, newC);
        }

        // MULTIPLICATION
        Vector3 Vector3::operator*(const Vector3& other) const {
            double newA = _a * other._a;
            double newB = _b * other._b;
            double newC = _c * other._c;

            return Vector3(newA, newB, newC);
        }

        // SCALAR MULTIPLICATION
        Vector3 Vector3::operator*(const float amount) const {
            double newA = _a * amount;
            double newB = _b * amount;
            double newC = _c * amount;

            return Vector3(newA, newB, newC);
        }

        // DIVISION
        Vector3 Vector3::operator/(const Vector3& other) const {
            double newA = _a / other._a;
            double newB = _b / other._b;
            double newC = _c / other._c;

            return Vector3(newA, newB, newC);
        }

        // SCALAR DIVISION
        Vector3 Vector3::operator/(const float amount) const {
            double newA = _a / amount;
            double newB = _b / amount;
            double newC = _c / amount;

            return Vector3(newA, newB, newC);
        }

        #pragma endregion

        #pragma region IN PLACE OPERATORS

        // IN PLACE ADDITION
        Vector3& Vector3::operator+=(const Vector3& other){
            _a += other._a;
            _b += other._b;
            _c += other._c;

            return *this;
        }

        // IN PLACE SUBSTRACTION
        Vector3& Vector3::operator-=(const Vector3& other){
            _a -= other._a;
            _b -= other._b;
            _c -= other._c;

            return *this;
        }

        // IN PLACE MULTIPLICATION
        Vector3& Vector3::operator*(const Vector3& other){
            _a *= other._a;
            _b *= other._b;
            _c *= other._c;

            return *this;
        }

        // IN PLACE SCALAR MULTIPLICATION
        Vector3& Vector3::operator*(const float amount){
            _a *= amount;
            _b *= amount;
            _c *= amount;

            return *this;
        }

        // IN PLACE DIVISION
        Vector3& Vector3::operator/(const Vector3& other){
            _a /= other._a;
            _b /= other._b;
            _c /= other._c;

            return *this;
        }

        // IN PLACE SCALAR DIVISION
        Vector3& Vector3::operator/(const float amount){
            _a /= amount;
            _b /= amount;
            _c /= amount;

            return *this;
        }

        #pragma endregion

        # pragma endregion

        // EQUAL COMPARISON
        bool Vector3::operator==(const Vector3& other) const {
            
            if (_a == other._a && _b == other._b && _c == other._c)
                return true;

            return false;
        }

        // NOT EQUAL COMPARISON
        bool Vector3::operator!=(const Vector3& other) const {

            return !(*this == other);
        }

        #pragma endregion

        #pragma region ===== FUNCTIONS =====

        const double Vector3::dot() const {

            double res = _a * _a + _b * _b + _c * _c;

            return res;
        }

        const double Vector3::dot(double scalar) const {

            double res = _a * scalar + _b * scalar + _c * scalar;

            return res;
        }

        const double Vector3::dot(const Vector3& other) const{

            double res = _a * other._a + _b * other._b + _c * other._c;

            return res;
        }

        const double Vector3::unsafeIndex(int i) const{
            switch (i)
            {
            case 0: return _a;
            case 1: return _b;
            case 2: return _c;
            default: throw "Index Out of Range"; //TODO Of course to implement better later
            }
        }

        const double Vector3::length() const{

            double res =  sqrt(lengthSquared());

            return res;
        }

        const double Vector3::lengthSquared() const{

            double res = dot();

            return res;
        }

        const bool Vector3::isZero() const{

            // Not the best implementation, should I allow a small delta to consider the value is 0 ?
            if (_a == 0 && _b == 0 && _c == 0)
                return true;

            return false;
        }

        #pragma endregion

#pragma endregion


#pragma region ========== POINT CLASS ==========

    #pragma region ===== CONSTRUCTORS =====

    // Default
    Point::Point() {
        myVect = Vector3::Vector3();
    }

    // From scalar
    Point::Point(double scal) {
        myVect = Vector3::Vector3(scal);
    }

    // From Vec3
    Point::Point(Vector3 vec) {
        myVect = Vector3(vec);
    }

    // Explicit
    Point::Point(double x, double y, double z) {
        myVect = Vector3(x, y, z);
    }

    #pragma endregion

    #pragma region ===== OPERATORS =====

    // TODO Mark those function as const 

    // POINT + DIRECTION
    Point Point::operator+(const Direction& other) const{
        Vector3 res = myVect + other.getVect();

        return Point(res);
    }

    // POINT - DIRECTION
    Point Point::operator-(const Direction& other) const{
        Vector3 res = myVect - other.getVect();

        return Point(res);
    }

    // DISTANCE TO
    double Point::DistanceTo(const Point& other) const {
        Vector3 diff = myVect - other.myVect;
        double distance = diff.dot();

        return distance;
    }

    #pragma endregion

#pragma endregion


#pragma region ========== DIRECTION CLASS ==========

    #pragma region ===== CONSTRUCTORS =====

    // Default
    Direction::Direction() {
        myVect = Vector3::Vector3();
    }

    // From scalar
    Direction::Direction(double scal) {
        myVect = Vector3::Vector3(scal);
    }

    // From Vec3
    Direction::Direction(Vector3 vec) {
        myVect = Vector3(vec);
    }

    // Explicit
    Direction::Direction(double x, double y, double z) {
        myVect = Vector3(x, y, z);
    }

    #pragma endregion


#pragma endregion


#pragma region ========== NORMALISED DIRECTION CLASS ==========

    #pragma region ===== CONSTRUCTORS =====

    // Default
    NormalisedDirection::NormalisedDirection() {
        myVect = Vector3::Vector3();
    }

    // From scalar
    NormalisedDirection::NormalisedDirection(double scal) {
        myVect = Vector3::Vector3(scal);
    }

    // From Vec3
    NormalisedDirection::NormalisedDirection(Vector3 vec) {
        myVect = Vector3(vec);
    }

    // Explicit
    NormalisedDirection::NormalisedDirection(double x, double y, double z) {
        myVect = Vector3(x, y, z);
    }

    #pragma endregion


#pragma endregion


#pragma region ========== COLOR CLASS ==========

    #pragma region ===== CONSTRUCTORS =====

    // Default
    Color::Color() {
        myVect = Vector3::Vector3();
    }

    // From scalar
    Color::Color(double scal) {
        myVect = Vector3::Vector3(scal);
    }

    // From Vec3
    Color::Color(Vector3 vec) {
        myVect = Vector3(vec);
    }

    // Explicit
    Color::Color(double x, double y, double z) {
        myVect = Vector3(x, y, z);
    }

    #pragma endregion


#pragma endregion


#pragma region ========== SURFACE ABSORBTION CLASS ==========

    #pragma region ===== CONSTRUCTORS =====

    // Default
    SurfaceAbsorption::SurfaceAbsorption() {
        myVect = Vector3::Vector3();
    }

    // From scalar
    SurfaceAbsorption::SurfaceAbsorption(double scal) {
        myVect = Vector3::Vector3(scal);
    }

    // From Vec3
    SurfaceAbsorption::SurfaceAbsorption(Vector3 vec) {
        myVect = Vector3(vec);
    }

    // Explicit
    SurfaceAbsorption::SurfaceAbsorption(double x, double y, double z) {
        myVect = Vector3(x, y, z);
    }

    #pragma endregion


#pragma endregion


//class NormalizedDirection {
//    private:
//        Vector3 myVect;
//
//    public:
//
//        NormalizedDirection(int x, int y, int z){
//            int normX, normY, normZ = 0;
//
//
//            if (x > 0) normX = 1;
//            else if (x < 0) normX = -1;
//
//            if (y > 0) normY = 1;
//            else if (y < 0) normY = -1;
//
//            if (z > 0) normZ = 1;
//            else if (z < 0) normZ = -1;
//
//            myVect = Vector3(normX, normY, normZ);
//                
//        }
//
//        Vector3 operator+(const NormalizedDirection& other) {
//            
//            Vector3 res = myVect + other.myVect;
//
//            return res;
//        }
//
//};
//
//class Color {
//    private:
//        Vector3 myVect;
//
//    public:
//
//        Color(int x, int y, int z) {
//            int colX = x;
//            int colY = y;
//            int colZ = z;
//
//            if (colX > 255) colX = 255;
//            else if (colX < 0) colX = 0;
//
//            if (colY > 255) colY = 255;
//            else if (colY < 0) colY = 0;
//
//            if (colZ > 255) colZ = 255;
//            else if (colZ < 0) colZ = 0;
//
//            myVect = Vector3(colX, colY, colZ);
//
//        }
//
//};
//
//class SurfaceAbsorptionProperties {
//    private:
//        Vector3 myVect;
//
//    public:
//        SurfaceAbsorptionProperties(double x, double y, double z) {
//            int normX = x;
//            int normY = y;
//            int normZ = z;
//
//
//            if (x > 1) normX = 1;
//            else if (x < 0) normX = 0;
//
//            if (y > 1) normY = 1;
//            else if (y < 0) normY = 0;
//
//            if (z > 1) normZ = 1;
//            else if (z < 0) normZ = 0;
//
//            myVect = Vector3(normX, normY, normZ);
//
//        }
//
//};