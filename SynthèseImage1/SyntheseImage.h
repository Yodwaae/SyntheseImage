#pragma once
#include <iostream>

// TODO : To move to different header and cpp files instead of using forward declaration
class Direction;
class Point;
class NormalisedDirection;
class Color;
class SurfaceAbsorption;

class Vector3 {

	private :

		double _a, _b, _c;
		static constexpr double ESPILON = 1e-9; // Might later be moved elsewhere

	public:

		#pragma region ===== CONSTRUCTORS =====
			
			// Default
			Vector3();

			// From Vector
			Vector3(const Vector3& other);
			
			// From Scalar
			Vector3(double scal);

			// Explicit (TODO : Deprecated ?)
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
		// TODO The Explicit constructor of the Vector3 class might have become useless

		// Default
		Vector3CRTP(): _vect() {}

		// From Scalar
		Vector3CRTP(double scal): _vect(T::Clamp(scal)) {}

		// From Vec3
		Vector3CRTP(const Vector3 vec): _vect(Clamp(vec)) {}

		 // Explicit
		Vector3CRTP(double x, double y, double z): _vect(Clamp(x, y, z)) {}

		#pragma endregion

		#pragma region ===== FUNCTIONS =====

		// TODO Might need to add a getter for each members (_a, _b, _c) in the future
		const Vector3& getVect() const { return _vect; }

		// Chain the clamp functions so the derived class only as to redefine the double one
		static double Clamp(const double scal) { return scal; }
		static Vector3 Clamp(const Vector3& vec) { return Vector3(Clamp(vec.getA()), Clamp(vec.getB()), Clamp(vec.getC())); }
		static Vector3 Clamp(double x, double y, double z) { return Vector3(Clamp(x), Clamp(y), Clamp(z)); }

		#pragma endregion

};

// NOTE : For all classes, the Vect3 will be stored as value and not pointer as it is a lightweight object, simpler and safer
// Also no need for copy constructor as in that case the compiler created one will work just fine
class Point : public Vector3CRTP<Point> {

    public:

		using Vector3CRTP<Point>::Vector3CRTP;

		#pragma region ===== OPERATORS =====

		Point operator+(const Direction& other) const;
		Point operator-(const Direction& other) const;

		#pragma endregion


		#pragma region ===== FUNCTIONS =====

		Direction DirectionTo(const Point& other) const;
		double DistanceTo(const Point& other) const;

		#pragma endregion

};

class Direction : public Vector3CRTP<Direction> {

	public :

		using Vector3CRTP<Direction>::Vector3CRTP;

		#pragma region ===== FUNCTIONS =====

		const double dot(const Direction& other) const;

		#pragma endregion

};

class NormalisedDirection : public Vector3CRTP<NormalisedDirection> {

	public:

		using Vector3CRTP<NormalisedDirection>::Vector3CRTP;

		#pragma region ===== FUNCTIONS =====

		static double Clamp(const double scal);

		#pragma endregion
};

class Color : public Vector3CRTP<Color> {

	public :

		using Vector3CRTP<Color>::Vector3CRTP;

		#pragma region ===== FUNCTIONS =====

		static double Clamp(const double scal);

		const double getRed() const { return _vect.getA(); }
		const double getGreen() const { return _vect.getB(); }
		const double getBlue() const{ return _vect.getC(); }

		#pragma endregion

};

class SurfaceAbsorption : public Vector3CRTP<SurfaceAbsorption> {
	
	public:
		using Vector3CRTP<SurfaceAbsorption>::Vector3CRTP;

		#pragma region ===== FUNCTIONS =====

		static double Clamp(const double scal);


		#pragma endregion

};