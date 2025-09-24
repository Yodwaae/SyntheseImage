// SynthèseImage1.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>

int main()
{

    Vector3 myVector = Vector3(1, 2, 3);
}

class Vector3 {

    private:
        double _a, _b, _c;

    public:
        Vector3():
        _a(0), _b(0), _c(0){}

        Vector3(double x, double y, double z):
        _a(x), _b(y), _c(z){}
        
        //region ===== OPERATORS =====

        Vector3 operator+(const Vector3& other) const {
            double newA = _a + other._a;
            double newB = _b + other._b;
            double newC = _c + other._c;


            return Vector3(newA, newB, newC);
        }

        Vector3 operator-(const Vector3& other) const {
            double newA = _a - other._a;
            double newB = _b - other._b;
            double newC = _c - other._c;

            return Vector3(newA, newB, newC);
        }

        Vector3 operator*(const Vector3& other) const {
            double newA = _a * other._a;
            double newB = _b * other._b;
            double newC = _c * other._c;

            return Vector3(newA, newB, newC);
        }

        Vector3 operator*(const float amount) const {
            double newA = _a * amount;
            double newB = _b * amount;
            double newC = _c * amount;

            return Vector3(newA, newB, newC);
        }

        //endregion

        //region ===== OPERATORS FUNCTIONS =====

        const double dot(const Vector3& other) {

            double res = _a * other._a + _b * other._b + _c * other._c;

            return res;
        }

        const double unsafeIndex(int i) {
            switch (i)
            {
            case 0: return _a;
            case 1: return _b;
            case 2: return _c;
            default: throw "Index Out of Range";
            }
        }

        const double length() {

            double res =  std::sqrt(this->dot(*this));

            return res;
        }

        //endregion

};


class Point {
    private:
        Vector3 myVect;

    public:
        Vector3 operator+(const Direction& other) {
            Vector3 res = myVect + other.myVect;

            return res;
        }


};

class Direction {
    private:

        Vector3 myVect;
        friend class Point;

};

class NormalizedDirection {
    private:
        Vector3 myVect;

    public:

        NormalizedDirection(int x, int y, int z){
            int normX, normY, normZ = 0;


            if (x > 0) normX = 1;
            else if (x < 0) normX = -1;

            if (y > 0) normY = 1;
            else if (y < 0) normY = -1;

            if (z > 0) normZ = 1;
            else if (z < 0) normZ = -1;

            myVect = Vector3(normX, normY, normZ);
                
        }

        Vector3 operator+(const NormalizedDirection& other) {
            
            Vector3 res = myVect + other.myVect;

            return res;
        }

};

class Color {
    private:
        Vector3 myVect;

    public:

        Color(int x, int y, int z) {
            int colX = x;
            int colY = y;
            int colZ = z;

            if (colX > 255) colX = 255;
            else if (colX < 0) colX = 0;

            if (colY > 255) colY = 255;
            else if (colY < 0) colY = 0;

            if (colZ > 255) colZ = 255;
            else if (colZ < 0) colZ = 0;

            myVect = Vector3(colX, colY, colZ);

        }

};

class SurfaceAbsorptionProperties {
    private:
        Vector3 myVect;

    public:
        SurfaceAbsorptionProperties(double x, double y, double z) {
            int normX = x;
            int normY = y;
            int normZ = z;


            if (x > 1) normX = 1;
            else if (x < 0) normX = 0;

            if (y > 1) normY = 1;
            else if (y < 0) normY = 0;

            if (z > 1) normZ = 1;
            else if (z < 0) normZ = 0;

            myVect = Vector3(normX, normY, normZ);

        }

};