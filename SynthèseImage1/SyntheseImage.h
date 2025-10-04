#pragma once
#include <iostream>

// TODO : To move to different header and cpp files instead of using forward declaration
class Direction;
class Point;
class NormalizedDirection;
class Color;
class SurfaceAbsorption;

class Vector3 {

	private :

		double _a, _b, _c;
		static constexpr double ESPILON = 1e-9; // Might later be moved elsewhere

	public:

		#pragma region ===== CONSTRUCTORS =====

			Vector3();
			Vector3(const Vector3& other);
			Vector3(double scal);
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

	private:

		Vector3 _vect;

	public:

		#pragma region ===== CONSTRUCTORS =====

		Vector3CRTP();
		Vector3CRTP(double scal);
		Vector3CRTP(const Vector3 vec);
		Vector3CRTP(double x, double y, double z);

		#pragma endregion

		#pragma region ===== FUNCTIONS =====

		// TODO Might need to add a getter for each members (_a, _b, _c) in the future
		//const Vector3& getVect() const { return _vect; }

		#pragma endregion

};

// NOTE : For all classes, the Vect3 will be stored as value and not pointer as it is a lightweight object, simpler and safer
// Also no need for copy constructor as in that case the compiler created one will work just fine
class Point {

    private:

        Vector3 _vect;

    public:

		#pragma region ===== CONSTRUCTORS =====

		Point();
		Point(double scal);
		Point(const Vector3 vec);
		Point(double x, double y, double z);

		#pragma endregion
		
		#pragma region ===== OPERATORS =====

		Point operator+(const Direction& other) const;
		Point operator-(const Direction& other) const;

		#pragma endregion


		#pragma region ===== FUNCTIONS =====

		Direction DirectionTo(const Point& other) const;
		double DistanceTo(const Point& other) const;

		#pragma endregion

};

class Direction {

    private:

        Vector3 _vect;

	public:

		#pragma region ===== CONSTRUCTORS =====

		Direction();
		Direction(double scal);
		Direction(const Vector3 vec);
		Direction(double r, double g, double b);

		#pragma endregion

		#pragma region ===== FUNCTIONS =====

		const Vector3& getVect() const { return _vect;}
		const double dot(const Direction& other) const;

		#pragma endregion

};

class NormalisedDirection {

	private:
		Vector3 _vect;

	public:

		#pragma region ===== CONSTRUCTORS =====

		NormalisedDirection();
		NormalisedDirection(double scal);
		NormalisedDirection(const Vector3 vec);
		NormalisedDirection(double r, double g, double b);

		#pragma endregion

};

class Color {

	private:
		Vector3 _vect;

	public:

		#pragma region ===== CONSTRUCTORS =====

		Color();
		Color(double scal);
		Color(const Vector3 vec);
		Color(double r, double g, double b);

		#pragma endregion

		#pragma region ===== FUNCTIONS =====

		const double getRed() const { return _vect.getA(); }
		const double getGreen() const { return _vect.getB(); }
		const double getBlue() const{ return _vect.getC(); }

		#pragma endregion

};

class SurfaceAbsorption {

	private:
		Vector3 _vect;

	public:

		#pragma region ===== CONSTRUCTORS =====

		SurfaceAbsorption();
		SurfaceAbsorption(double scal);
		SurfaceAbsorption(const Vector3 vec);
		SurfaceAbsorption(double r, double g, double b);

		#pragma endregion

};