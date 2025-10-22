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
		// TODO Should others constructors be made explicit ? To keep an eye on

		// Default
		Vector3CRTP(): _vect() {}

		// From Scalar
		explicit Vector3CRTP(double scal): _vect(ApplyPolicy(scal)) {}

		// From Vec3
		Vector3CRTP(const Vector3& vec) : _vect(ApplyPolicy(vec)) {}

		 // Explicit
		Vector3CRTP(double x, double y, double z): _vect(ApplyPolicy(x, y, z)) {}

		#pragma endregion

		#pragma region ===== OPERATORS =====
		
		// NOTE : Stay vigilant about the T return, should be safer but could cause error wuth the way the logic is implemented
		// SCALAR
		T operator*(const double amount) const { return T(_vect * amount); }
		T operator/(const double amount) const { return T(_vect / amount); }

		#pragma endregion

		#pragma region ===== FUNCTIONS =====

		// POLICY
		static double ApplyPolicy(double v) { return T::Policy(v); }

		static Vector3 ApplyPolicy(const Vector3& vec) {
			return Vector3(T::Policy(vec.getA()),
				T::Policy(vec.getB()),
				T::Policy(vec.getC()));
		}

		static Vector3 ApplyPolicy(double x, double y, double z) {
			return ApplyPolicy(Vector3(x, y, z));
		}


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

		// POLICY
		inline static double Policy(const double scal) { return scal; }

		// DIRECTION & DISTANCE
		Direction DirectionTo(const Point& other) const;
		NormalisedDirection NormalisedDirectionTo(const Point& other) const;
		double DistanceTo(const Point& other) const;
		double SquaredDistanceTo(const Point& other) const;

		#pragma endregion

};

class Direction : public Vector3CRTP<Direction> {

	public :

		using Vector3CRTP<Direction>::Vector3CRTP;

		#pragma region ===== FUNCTIONS =====

		// POLICY
		inline static double Policy(const double scal) { return scal; }

		// NORMALISE
		NormalisedDirection Normalise() const;

		#pragma endregion

};

// TODO Better than before but there's still problem of code duplication (not much), also might need to redefine operators so operation between normalisedDir send back a normalisedDir (to see if it's useful)
class NormalisedDirection : public Direction {

	public:

		#pragma region ===== CONSTRUCTORS =====

		// OPTI If I keep this approach I'll compute the default normalisedDirection at compile time and store it as constexpr
		// Default, normalise a (1, 1, 1) vector because a (0, 0, 0) can't be normalised as it's length will always be 0
		NormalisedDirection(): Direction(Normalise(Vector3(1, 1, 1))) {}

		// From Scalar
		NormalisedDirection(double scal): Direction(Normalise(Vector3(scal, scal, scal))) {}

		// From Vec3
		NormalisedDirection(const Vector3& vec) : Direction(Normalise(vec)) {}

		// Explicit
		NormalisedDirection(double x, double y, double z): Direction(Normalise(Vector3(x, y, z))) {}

		#pragma endregion

		#pragma region ===== FUNCTIONS =====

		// POLICY
		inline static double Policy(const double scal) { return Normalise(Vector3(scal, scal, scal)).getA(); }
		inline static Vector3 Policy(double x, double y, double z) { return Normalise(Vector3(x, y, z)); }
		inline static Vector3 Policy(const Vector3& vec) { return Normalise(vec); }

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

		// GETTERS
		const double getRed() const { return getA(); }
		const double getGreen() const { return getB(); }
		const double getBlue() const{ return getC(); }

		// POLICY
		inline static double Policy(const double scal) { return Clamp(scal); }

		// CLAMPING
		inline static double Clamp(const double scal) { return std::clamp(scal, 0.0, 255.0); }

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

		// POLICY
		inline static double Policy(const double scal) {return Clamp(scal);}

		// CLAMPING
		inline static double Clamp(const double scal) { return std::clamp(scal, 0.0, 1.0); }

		#pragma endregion

};

class LightPower : public Vector3CRTP<LightPower>{

public:

	using Vector3CRTP<LightPower>::Vector3CRTP;

	#pragma region ===== OPERATORS =====

	LightPower operator+(const LightPower& other) const;

	#pragma endregion


	#pragma region ===== FUNCTIONS =====

	// POLICY
	inline static double Policy(const double scal) { return scal; }
	
	// GAMMA CORRECTION
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