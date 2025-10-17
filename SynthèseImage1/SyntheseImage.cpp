
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
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
            
        if (abs(_a - other._a) < EPSILON && abs(_b - other._b) < EPSILON && abs(_c - other._c) < EPSILON)
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
        default: throw out_of_range("Index " + to_string(i) + "is out of range");
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

    // NORMALISED DIRECTION TO
    NormalisedDirection Point::NormalisedDirectionTo(const Point& other) const {
        NormalisedDirection res(other._vect - _vect);

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

    NormalisedDirection Direction::Normalise() const {
        NormalisedDirection res = NormalisedDirection(_vect);

        return res;
    }

    #pragma endregion

#pragma endregion


#pragma region ========== NORMALISED DIRECTION CLASS ==========

    #pragma region ===== FUNCTIONS =====

    #pragma endregion


#pragma endregion


#pragma region ========== COLOR CLASS ==========

    #pragma region ===== FUNCTIONS =====

    Color Color::operator*(const Albedo& albedo) const {
        Vector3 res = _vect * albedo.getVect();

        return Color(res);
    }

    Color Color::operator*(const LightPower& lightPower) const {
        Vector3 res = _vect * lightPower.getVect();

        return Color(res);
    }

    #pragma endregion

#pragma endregion


#pragma region ========== ALBEDO ==========

    #pragma region ===== FUNCTIONS =====

      
    #pragma endregion

#pragma endregion

#pragma region ========== LIGHT POWER ==========

#pragma region ===== FUNCTIONS =====

    LightPower const LightPower::GammaCorrection() const{
        LightPower res = LightPower(pow(getA(), GAMMA_CORRRECTION), pow(getB(), GAMMA_CORRRECTION), pow(getC(), GAMMA_CORRRECTION));

        return res;
    }

#pragma endregion

#pragma endregion
