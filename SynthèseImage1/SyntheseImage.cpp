
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "SyntheseImage.h"
#include "Raytrace.h"

using namespace std;


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
    Vector3 Vector3::operator*(const double amount) const {
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
    Vector3 Vector3::operator/(const double amount) const {
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
    Vector3& Vector3::operator*=(const Vector3& other){
        _a *= other._a;
        _b *= other._b;
        _c *= other._c;

        return *this;
    }

    // IN PLACE SCALAR MULTIPLICATION
    Vector3& Vector3::operator*=(const double amount){
        _a *= amount;
        _b *= amount;
        _c *= amount;

        return *this;
    }

    // IN PLACE DIVISION
    Vector3& Vector3::operator/=(const Vector3& other){
        _a /= other._a;
        _b /= other._b;
        _c /= other._c;

        return *this;
    }

    // IN PLACE SCALAR DIVISION
    Vector3& Vector3::operator/=(const double amount){
        _a /= amount;
        _b /= amount;
        _c /= amount;

        return *this;
    }

    #pragma endregion

    # pragma endregion

    // EQUAL COMPARISON
    bool Vector3::operator==(const Vector3& other) const {
            
        if (abs(_a - other._a) < ESPILON && abs(_b - other._b) < ESPILON && abs(_c - other._c) < ESPILON)
            return true;

        return false;
    }

    // NOT EQUAL COMPARISON
    bool Vector3::operator!=(const Vector3& other) const {

        return !(*this == other);
    }

    #pragma endregion

    #pragma region ===== FUNCTIONS =====

    double Vector3::dot(const Vector3& other) const{

        double res = _a * other._a + _b * other._b + _c * other._c;

        return res;
    }

    double Vector3::unsafeIndex(int i) const{
        switch (i)
        {
        case 0: return _a;
        case 1: return _b;
        case 2: return _c;
        default: throw "Index Out of Range"; //TODO Of course to implement better later
        }
    }

    double Vector3::length() const{

        double res =  sqrt(lengthSquared());

        return res;
    }

    double Vector3::lengthSquared() const{

        double res = dot(*this);

        return res;
    }

    bool Vector3::isZero() const{

        // Not the best implementation, should I allow a small delta to consider the value is 0 ?
        if (_a == 0 && _b == 0 && _c == 0)
            return true;

        return false;
    }

    #pragma endregion

#pragma endregion


#pragma region ========== POINT CLASS ==========

    #pragma region ===== OPERATORS =====

    // POINT + DIRECTION
    Point Point::operator+(const Direction& other) const{
        Vector3 res = _vect + other.getVect();

        return Point(res);
    }

    // POINT - DIRECTION
    Point Point::operator-(const Direction& other) const{
        Vector3 res = _vect - other.getVect();

        return Point(res);
    }

    #pragma endregion

    #pragma region ===== FUNCTIONS =====
    
    // DIRECTION TO
    Direction Point::DirectionTo(const Point& other) const {
        Direction res = other._vect - _vect;

        return res;
    }

    // DISTANCE TO
    double Point::DistanceTo(const Point& other) const {
        double res = (other._vect - _vect).length();

        return res;
    }

    #pragma endregion

#pragma endregion


#pragma region ========== DIRECTION CLASS ==========

    #pragma region ===== FUNCTIONS =====


    const double Direction::dot(const Direction& other) const {

        double res = _vect.dot(other._vect);

        return res;
    }

    #pragma endregion

#pragma endregion


#pragma region ========== NORMALISED DIRECTION CLASS ==========

    #pragma region ===== FUNCTIONS =====

        // Clamp NormalisedDirection to either (-1, 0 or 1)
        double NormalisedDirection::Clamp(const double scal) {

            if (scal > 0) return 1;
            else if (scal < 0) return -1;
            else return 0;
        }

    #pragma endregion


#pragma endregion


#pragma region ========== COLOR CLASS ==========

    #pragma region ===== FUNCTIONS =====

    // Color clamping between 0 and 255
    // TODO Use the std clamp function from <algorithm> instead
    double Color::Clamp(const double scal) {

        if (scal > 255) return 255;
        else if (scal < 0) return 0;
        else return scal;
    }

    #pragma endregion

#pragma endregion


#pragma region ========== SURFACE ABSORBTION CLASS ==========

    #pragma region ===== FUNCTIONS =====

        // Clamping between 0 and 1 (as I don't have min and max value it's not really a normalisation
        // TODO Use the std clamp function from <algorithm> instead
        double SurfaceAbsorption::Clamp(const double scal) {

            if (scal > 1) return 1;
            else if (scal < 0) return 0;
            else return scal;
        }

    #pragma endregion

#pragma endregion


//class NormalizedDirection {
//    private:
//        Vector3 _vect;
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
//            _vect = Vector3(normX, normY, normZ);
//                
//        }
//
//        Vector3 operator+(const NormalizedDirection& other) {
//            
//            Vector3 res = _vect + other._vect;
//
//            return res;
//        }
//
//};
//
//class Color {
//    private:
//        Vector3 _vect;
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
//            _vect = Vector3(colX, colY, colZ);
//
//        }
//
//};
//
//class SurfaceAbsorptionProperties {
//    private:
//        Vector3 _vect;
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
//            _vect = Vector3(normX, normY, normZ);
//
//        }
//
//};