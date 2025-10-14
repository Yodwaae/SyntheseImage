#pragma once
#include <iostream>
#include <algorithm>

// NOTE : For all classes, the Vect3 will be stored as value and not pointer as it is a lightweight object, simpler and safer
// Also no need for copy constructor as in that case the compiler created one will work just fine
// NOTE : Commutative operator that impact two differents classes also have a function to reverse the expression allowing the operator to work both ways

// TODO : Also decide whether in class I organise things by access (public/private) then by type (variable/functions) or the other way around

#pragma region ========== FORWARD DECLARATIONS ==========

class Direction;
class Point;
class NormalisedDirection;
class Color;
class SurfaceAbsorption;

#pragma endregion


#pragma region ========== GLOBALS ==========

static constexpr double EPSILON = 1e-9;

#pragma endregion


#pragma region ========== CLASSES ==========

class Vector3 {

	private :

		double _a, _b, _c;

	public:

		#pragma region ===== CONSTRUCTORS =====
			
			// Default
			Vector3();

			// From Vector
			Vector3(const Vector3& other);
			
			// From Scalar
			Vector3(double scal);

			// Explicit (NOTE :Shouldn't be used anymore but for the moment I keep it as it can be used during debug/temp integration of a feature
			Vector3(double x, double y, double z);

		#pragma endregion

		#pragma region ===== OPERATORS =====

		#pragma region === ARITHMETIC OPERATORS ===

		#pragma region PURE/VALUE OPERATORS

		Vector3 operator+(const Vector3& other) const;
		Vector3 operator-(const Vector3& other) const;
		Vector3 operator*(const Vector3& other) const;
		Vector3 operator/(const Vector3& other) const;

		// SCALAR
		Vector3 operator/(const double amount) const;
		Vector3 operator*(const double amount) const;

		#pragma endregion

		#pragma region IN PLACE OPERATORS

		Vector3& operator+=(const Vector3& other);
		Vector3& operator-=(const Vector3& other);
		Vector3& operator*=(const Vector3& other);
		Vector3& operator/=(const Vector3& other);

		// SCALAR
		Vector3& operator*=(const double amount);
		Vector3& operator/=(const double amount);

		#pragma endregion

		#pragma endregion

		bool operator==(const Vector3& other) const;
		bool operator!=(const Vector3& other) const;

		#pragma endregion

		#pragma region ===== FUNCTIONS =====

		double dot(const Vector3& other) const;
		double unsafeIndex(int i) const;
		double length() const;
		double lengthSquared() const;
		bool isZero() const;

		double getA() const { return _a; }
		double getB() const { return _b; }
		double getC() const { return _c; }


		#pragma endregion

};

template <typename T>
class Vector3CRTP {

	protected:

		Vector3 _vect;

	public:
	
		#pragma region ===== CONSTRUCTORS =====
		// Note : Using a static polyphormism hook to allow classes like Color to clamp the value
		// Also defining a default clamp because some classes (Point & Direction) won't define one

		// Default
		Vector3CRTP(): _vect() {}

		// From Scalar
		Vector3CRTP(double scal): _vect(T::Clamp(scal)) {}

		// From Vec3
		Vector3CRTP(const Vector3& vec) : _vect(T::Clamp(vec)) {}

		 // Explicit
		Vector3CRTP(double x, double y, double z): _vect(T::Clamp(x, y, z)) {}

		#pragma endregion

		#pragma region ===== OPERATORS =====
		
		// NOTE : Stay vigilant about the T return, should be safer but could cause error wuth the way the logic is implemented
		T operator*(const double amount) const { return T(_vect * amount); }

		#pragma endregion

		#pragma region ===== FUNCTIONS =====

		const double dot(const Vector3CRTP<T>& other) const { return _vect.dot(other._vect); }


		// NOTE : Do not create overhead as they are inlined by the compiler
		const Vector3& getVect() const { return _vect; }
		const double getA() const { return _vect.getA(); }
		const double getB() const { return _vect.getB(); }
		const double getC() const { return _vect.getC(); }

		#pragma endregion

};

class Point : public Vector3CRTP<Point> {

    public:

		using Vector3CRTP<Point>::Vector3CRTP;

		#pragma region ===== OPERATORS =====

		Point operator+(const Direction& other) const;
		Point operator-(const Direction& other) const; 

		#pragma endregion


		#pragma region ===== FUNCTIONS =====
		static double Clamp(const double scal) { return scal; }
		static Vector3 Clamp(const Vector3& vec) { return vec; }
		static Vector3 Clamp(double x, double y, double z) { return Vector3(x, y, z); }

		Direction DirectionTo(const Point& other) const;
		NormalisedDirection NormalisedDirectionTo(const Point& other) const;
		double DistanceTo(const Point& other) const;

		#pragma endregion

};

class Direction : public Vector3CRTP<Direction> {

	public :

		using Vector3CRTP<Direction>::Vector3CRTP;

		#pragma region ===== FUNCTIONS =====

		// Chain the clamp functions so the derived class only as to redefine the double one
		static double Clamp(const double scal) { return scal; }
		static Vector3 Clamp(const Vector3& vec) { return vec; }
		static Vector3 Clamp(double x, double y, double z) { return Vector3(x, y , z); }

		#pragma endregion

};

class NormalisedDirection : public Vector3CRTP<NormalisedDirection> {

	public:

		using Vector3CRTP<NormalisedDirection>::Vector3CRTP;

		#pragma region ===== FUNCTIONS =====

		static double Clamp(const double scal) { Clamp(Vector3(scal, scal, scal)); }
		static Vector3 Clamp(double x, double y, double z) { Clamp(Vector3(x, y, z)); }
		/* Normalizes 'vec' so its length becomes 1
		* If 'vec' is nearly zero-length, returns (0, 0, 0) instead
		* to avoid division by zero */
		static Vector3 Clamp(const Vector3& vec) {
			double len = vec.length();
			if (len < EPSILON) return Vector3(0.0, 0.0, 0.0);
			return vec / len;
		}


		// TODO Bandage fix need to rethink how to do manage Direction and NormalisedDirection
		Direction ToDirection() const { return Direction(_vect); }

		#pragma endregion
};

class Color : public Vector3CRTP<Color> {

	public :

		using Vector3CRTP<Color>::Vector3CRTP;

		#pragma region ===== FUNCTIONS =====

		// Getters
		const double getRed() const { return getA(); }
		const double getGreen() const { return getB(); }
		const double getBlue() const{ return getC(); }

		// Color clamping between 0 and 255
		static double Clamp(const double scal) { return std::clamp(scal, 0.0, 255.0); }
		static Vector3 Clamp(const Vector3& vec) { return Vector3(Clamp(vec.getA()), Clamp(vec.getB()), Clamp(vec.getC())); }
		static Vector3 Clamp(double x, double y, double z) { return Vector3(Clamp(x), Clamp(y), Clamp(z)); }

		#pragma endregion

};

class SurfaceAbsorption : public Vector3CRTP<SurfaceAbsorption> {
	
	public:
		using Vector3CRTP<SurfaceAbsorption>::Vector3CRTP;

		#pragma region ===== FUNCTIONS =====

		// Clamping between 0 and 1 to have an absorption coef
		static double Clamp(const double scal) {return std::clamp(scal, 0.0, 1.0);}
		static Vector3 Clamp(const Vector3& vec) { return Vector3(Clamp(vec.getA()), Clamp(vec.getB()), Clamp(vec.getC())); }
		static Vector3 Clamp(double x, double y, double z) { return Vector3(Clamp(x), Clamp(y), Clamp(z)); }


		#pragma endregion

};

#pragma endregion


#pragma region =========== NON-MEMBER OPERATORS ==========

// DIRECTION + POINT
inline Point operator+(const Direction& dir, const Point& point) {
	return point + dir;
}

// VECTOR3CRTP<T> * SCALAR
template <typename T>
inline Vector3CRTP<T> operator*(double scalar, const Vector3CRTP<T>& t) {
	return t * scalar;
}

#pragma endregion