#include <iostream>

// TEMP : Added anticipated class declaration to simplify things for the moment
class Direction; // TODO Might need to have multiple header and cpp files in the future, for clarity sake
class Point;
class NormalizedDirection;
class Color;
class SurfaceAbsorption;

class Vector3 {

	private :

		double _a, _b, _c;

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
		Vector3 operator/(const float amount) const;
		Vector3 operator*(const float amount) const;

		#pragma endregion

		#pragma region IN PLACE OPERATORS

		Vector3& operator+=(const Vector3& other);
		Vector3& operator-=(const Vector3& other);
		Vector3& operator*(const Vector3& other);
		Vector3& operator/(const Vector3& other);

		// SCALAR
		Vector3& operator*(const float amount);
		Vector3& operator/(const float amount);

		#pragma endregion

		#pragma endregion

		bool operator==(const Vector3& other) const;
		bool operator!=(const Vector3& other) const;

		#pragma endregion

		#pragma region ===== FUNCTIONS =====

		const double dot(const Vector3& other) const;
		const double unsafeIndex(int i) const;
		const double length() const;
		const double lengthSquared() const;
		const bool isZero() const;

		const double getA() const { return _a; }
		const double getB() const { return _b; }
		const double getC() const { return _c; }


		#pragma endregion

};


// NOTE : For all classes, the Vect3 will be stored as value and not pointer as it is a lightweight object, simpler and safer
// Also no need for copy constructor as in that case the compiler created one will work just fine

class Point {

    private:

        Vector3 myVect;

    public:

		#pragma region ===== CONSTRUCTORS =====

		Point();
		Point(double scal);
		Point(Vector3 vec);
		Point(double x, double y, double z);

		#pragma endregion
		
		#pragma region ===== OPERATORS =====

		Point operator+(const Direction& other) const;
		Point operator-(const Direction& other) const;

		#pragma endregion

};

class Direction {

    private:

        Vector3 myVect;

	public:

		#pragma region ===== CONSTRUCTORS =====

		Direction();
		Direction(double scal);
		Direction(Vector3 vec);
		Direction(double r, double g, double b);

		#pragma endregion

		#pragma region ===== FUNCTIONS =====

		const Vector3& getVect() const { return myVect;}

		#pragma endregion

};

class NormalisedDirection {

	private:
		Vector3 myVect;

	public:

		#pragma region ===== CONSTRUCTORS =====

		NormalisedDirection();
		NormalisedDirection(double scal);
		NormalisedDirection(Vector3 vec);
		NormalisedDirection(double r, double g, double b);

		#pragma endregion

};

class Color {

	private:
		Vector3 myVect;

	public:

		#pragma region ===== CONSTRUCTORS =====

		Color();
		Color(double scal);
		Color(Vector3 vec);
		Color(double r, double g, double b);

		#pragma endregion

		#pragma region ===== FUNCTIONS =====

		const double getRed() const { return myVect.getA(); }
		const double getGreen() const { return myVect.getB(); }
		const double getBlue() const{ return myVect.getC(); }

		#pragma endregion

};

class SurfaceAbsorption {

	private:
		Vector3 myVect;

	public:

		#pragma region ===== CONSTRUCTORS =====

		SurfaceAbsorption();
		SurfaceAbsorption(double scal);
		SurfaceAbsorption(Vector3 vec);
		SurfaceAbsorption(double r, double g, double b);

		#pragma endregion

};