#pragma once
#include <iostream>
#include <algorithm>

// NOTE : For all classes, the Vect3 will be stored as value and not pointer as it is a lightweight object, simpler and safer
// Also no need for copy constructor as in that case the compiler created one will work just fine
// NOTE : Commutative operator that impact two differents classes also have a function to reverse the expression allowing the operator to work both ways

// TODO : Also decide whether in class I organise things by access (public/private) then by type (variable/functions) or the other way around
// TODO : Rename all the clamp function that are not actually clamp to policy, and have policy call clamp for the class that needs it

#pragma region ========== FORWARD DECLARATIONS ==========

class Direction;
class Point;
class NormalisedDirection;
class Color;
class Albedo;
class LightPower;

#pragma endregion


#pragma region ========== GLOBALS ==========

static constexpr double EPSILON = 1e-9;
constexpr double GAMMA_CORRECTION = 1 / 2.2;

#pragma endregion


#pragma region ========== CLASSES ==========


// TODO the vector3 operators are not working both way, yet it might not be a problem as I should not access them outside the Vec3CRTP wrapper 
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

		// TODO Should others constructors be made explicit ? To keep an eye on

		// Default
		Vector3CRTP(): _vect() {}

		// From Scalar
		explicit Vector3CRTP(double scal): _vect(T::Clamp(scal)) {}

		// From Vec3
		Vector3CRTP(const Vector3& vec) : _vect(T::Clamp(vec)) {}

		 // Explicit
		Vector3CRTP(double x, double y, double z): _vect(T::Clamp(x, y, z)) {}

		#pragma endregion

		#pragma region ===== OPERATORS =====
		
		// NOTE : Stay vigilant about the T return, should be safer but could cause error wuth the way the logic is implemented
		// SCALAR
		T operator*(const double amount) const { return T(_vect * amount); }
		T operator/(const double amount) const { return T(_vect / amount); }

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

		NormalisedDirection Normalise() const;

		#pragma endregion

};

// TODO Better than before but there's still problem of code duplication (not much), also might need to redefine operators so operation between normalisedDir send back a normalisedDir (to see if it's useful)
class NormalisedDirection : public Direction {

	public:

		// OPTI If I keep this approach I'll compute the default normalisedDirection at compile time and store it as constexpr
		// Default, normalise a (1, 1, 1) vector because a (0, 0, 0) can't be normalised as it's length will always be 0
		NormalisedDirection(): Direction(Normalise(Vector3(1, 1, 1))) {}

		// From Scalar
		NormalisedDirection(double scal): Direction(Normalise(Vector3(scal, scal, scal))) {}

		// From Vec3
		NormalisedDirection(const Vector3& vec) : Direction(Normalise(vec)) {}

		// Explicit
		NormalisedDirection(double x, double y, double z): Direction(Normalise(Vector3(x, y, z))) {}

		#pragma region ===== FUNCTIONS =====

		static double Clamp(const double scal) { return Clamp(Vector3(scal, scal, scal)).getA(); }
		static Vector3 Clamp(double x, double y, double z) { return Clamp(Vector3(x, y, z)); }
		static Vector3 Clamp(const Vector3& vec) { return Normalise(vec); }

		/* Normalizes 'vec' so its length becomes 1
		* If 'vec' is nearly zero-length, returns (0, 0, 0) instead
		* to avoid division by zero */
		static Vector3 Normalise(const Vector3& vec) {

			double len = vec.length();
			if (len < EPSILON) return Vector3(0.0, 0.0, 0.0);
			return vec / len;
		}

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

		#pragma region ===== OPERATORS =====

		// COLOR * SURFACE ABSORPTION
		Color operator*(const Albedo& albedo) const;

		// COLOR * LIGHT POWER
		Color operator*(const LightPower& lightPower) const;

		#pragma endregion

};

class Albedo : public Vector3CRTP<Albedo> {
	
	public:
		using Vector3CRTP<Albedo>::Vector3CRTP;

		#pragma region ===== FUNCTIONS =====

		// Clamping between 0 and 1 to have an absorption coef
		static double Clamp(const double scal) {return std::clamp(scal, 0.0, 1.0);}
		static Vector3 Clamp(const Vector3& vec) { return Vector3(Clamp(vec.getA()), Clamp(vec.getB()), Clamp(vec.getC())); }
		static Vector3 Clamp(double x, double y, double z) { return Vector3(Clamp(x), Clamp(y), Clamp(z)); }


		#pragma endregion

};

class LightPower : public Vector3CRTP<LightPower>{

public:

	using Vector3CRTP<LightPower>::Vector3CRTP;

	#pragma region ===== FUNCTIONS =====

	static double Clamp(const double scal) { return scal; }
	static Vector3 Clamp(const Vector3& vec) { return vec; }
	static Vector3 Clamp(double x, double y, double z) { return Vector3(x, y, z); }
	
	LightPower GammaCorrection() const;

	#pragma endregion

};

#pragma endregion


#pragma region =========== NON-MEMBER OPERATORS ==========

// VECTOR3CRTP<T> * SCALAR
template <typename T>
inline T operator*(double scalar, const Vector3CRTP<T>& t) {
	return t * scalar;
}

// VECTOR3CRTP<T> / SCALAR
template <typename T>
inline T operator/(double scalar, const Vector3CRTP<T>& t) {
	return t / scalar;
}

// DIRECTION + POINT
inline Point operator+(const Direction& dir, const Point& point) {
	return point + dir;
}

// SURFACE ABSORPTION * COLOR
inline Color operator*(const Albedo& albedo, const Color& color) {
	return color * albedo;
}

// LIGHT POWER * COLOR
inline Color operator*(const LightPower& lightPower, const Color& color) {
	return color * lightPower;
}

#pragma endregion