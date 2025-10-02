    // SynthèseImage1.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
    //

    #include <iostream>
    #include "SyntheseImage.h"


    int main()
    {

        Vector3 myVector = Vector3(1, 2, 3);
    }

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
        void Vector3::operator+=(const Vector3& other){
            _a += other._a;
            _b += other._b;
            _c += other._c;

        }

        // IN PLACE SUBSTRACTION
        void Vector3::operator-=(const Vector3& other){
            _a -= other._a;
            _b -= other._b;
            _c -= other._c;

        }

        // IN PLACE MULTIPLICATION
        void Vector3::operator*(const Vector3& other){
            _a *= other._a;
            _b *= other._b;
            _c *= other._c;

        }

        // IN PLACE SCALAR MULTIPLICATION
        void Vector3::operator*(const float amount){
            _a *= amount;
            _b *= amount;
            _c *= amount;

        }

        // IN PLACE DIVISION
        void Vector3::operator/(const Vector3& other){
            _a /= other._a;
            _b /= other._b;
            _c /= other._c;

        }

        // IN PLACE SCALAR DIVISION
        void Vector3::operator/(const float amount){
            _a /= amount;
            _b /= amount;
            _c /= amount;

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

        const double Vector3::dot(const Vector3& other) {

            double res = _a * other._a + _b * other._b + _c * other._c;

            return res;
        }

        const double Vector3::unsafeIndex(int i) {
            switch (i)
            {
            case 0: return _a;
            case 1: return _b;
            case 2: return _c;
            default: throw "Index Out of Range";
            }
        }

        const double Vector3::length() {

            double res =  std::sqrt(this->dot(*this));

            return res;
        }

        const double Vector3::lengthSquared() {

            double res = this->dot(*this);

            return res;
        }

        const bool Vector3::isZero() {

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

        // TODO Should maybe add a copy constructor to vec3
        //Point::Point(Vector3 vec) {
        //    myVect = Vector3(vec._a, vec._b, vec._c)
        //}

        Point::Point(double r, double g, double b) {
            myVect = Vector3(r, g, b);
        }



    #pragma endregion

    #pragma region ===== OPERATORS =====

    // TODO Mark those function as const 

    // POINT + DIRECTION
    Point Point::operator+(const Direction& other) {
        Vector3 res = myVect + other.myVect;

        return Point(res);
    }

    // POINT - DIRECTION
    Point Point::operator-(const Direction& other) {
        Vector3 res = myVect - other.myVect;

        return Point(res);
    }

    #pragma endregion

#pragma endregion

//
//        // POINT - DIRECTION
//        Vector3 operator-(const Direction& other) {
//            Vector3 res = myVect - other.myVect;
//
//            return res;
//        }
//
//        //endregion
//
//};
//
//class Direction {
//    private:
//
//        Vector3 myVect;
//        friend class Point;
//
//};
//
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