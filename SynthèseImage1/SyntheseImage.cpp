
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "SyntheseImage.h"
#include "Raytrace.h"

using namespace std;


#pragma region ========== VECTOR3 CLASS ==========
        
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

    // SQUARED DISTANCE TO
    double Point::SquaredDistanceTo(const Point& other) const {
        double res = pow(DistanceTo(other), 2);

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

    // DIRECTION + NORMALISED DIRECTION
    Direction Direction::operator+(const NormalisedDirection& other) const {
        Direction res = other.getVect() + _vect;

        return res;
    }

    // DIRECTION * NORMALISED DIRECTION
    Direction Direction::operator*(const NormalisedDirection& other) const {
        Direction res = other.getVect() * _vect;

        return res;
    }

#pragma endregion


#pragma region ========== NORMALISED DIRECTION CLASS ==========

    #pragma region ===== FUNCTIONS =====

    // NORMALISED DIRECTION * NORMALISED DIRECTION
    NormalisedDirection NormalisedDirection::operator*(const NormalisedDirection& other) const {
        Direction res = _vect * other.getVect();

        return res.Normalise();
    }

    // NORMALISED DIRECTION * DIRECTION
    Direction NormalisedDirection::operator*(const Direction& other) const {
        Direction res = _vect * other.getVect();

        return res;
    }


    #pragma endregion


#pragma endregion


#pragma region ========== COLOR CLASS ==========

    #pragma region ===== OPERATORS =====

    #pragma region === ARITHMETIC OPERATORS ===

    #pragma region PURE/VALUE OPERATORS

    // COLOR + COLOR
    Color Color::operator+(const Color& color) const {
        Vector3 res = _vect + color.getVect();

        return Color(res);
    }

    // COLOR * ALBEDO
    Color Color::operator*(const Albedo& albedo) const {
        Vector3 res = _vect * albedo.getVect();

        return Color(res);
    }

    #pragma endregion

    #pragma region IN PLACE OPERATORS


    // TODO Problem is, the color isn't clamped when doing this
    // IN PLACE COLOR + COLOR
    Color Color::operator+=(const Color& color) {
        _vect += color.getVect();

        return *this;
    }

    #pragma endregion

    // DEAD CODE
    /*
    Color Color::operator*(const LightPower& lightPower) const {
        Vector3 res = _vect * lightPower.getVect();

        return Color(res);
    }*/

    #pragma endregion

    #pragma endregion

#pragma endregion


#pragma region ========== ALBEDO ==========

    #pragma region ===== FUNCTIONS =====

    // TODO : This should be in material class
    // TODO I need to clean this up
    NormalisedDirection Reflect(const NormalisedDirection& normal, const NormalisedDirection& ray) {
        NormalisedDirection projection = - normal.dot(ray);

        Direction reflectedRay = 2 * projection * normal + ray;
        NormalisedDirection normalisedReflectedRay = reflectedRay.Normalise();

        return normalisedReflectedRay;
    }


      
    #pragma endregion

#pragma endregion

#pragma region ========== LIGHT POWER ==========

    #pragma region ===== OPERATORS =====
    // DEAD CODE
    /*

    // TODO Should I add a function inline Vector3 Vector3CRTP::MULTIPLY(const Vector3CRTP& other) instead of writing _vect * other.getVect(); every time ? Or is just a bit redundant ?
    LightPower LightPower::operator+(const LightPower& other) const {
        LightPower res = _vect + other.getVect();

        return res;
    }

    Color LightPower::operator*(const Albedo& other) const {
        Color res = _vect * other.getVect();

        return res;
    }

    LightPower LightPower::operator+=(const LightPower& other) {
        LightPower res = _vect += other.getVect();

        return res;
    }

    #pragma endregion

    #pragma region ===== FUNCTIONS =====

    LightPower LightPower::GammaCorrection() const {
        LightPower res = Vector3(pow(getA(), GAMMA_CORRECTION), pow(getB(), GAMMA_CORRECTION), pow(getC(), GAMMA_CORRECTION));

        return res;
    }
    */

    #pragma endregion

#pragma endregion
