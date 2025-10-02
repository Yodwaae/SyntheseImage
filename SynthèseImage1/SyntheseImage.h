#include <iostream>


//TODO Maybe harmonise indent size in the file

class Vector3 {

	private :

		double _a, _b, _c;

	public:

	#pragma region ===== CONSTRUCTORS =====

		Vector3();
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

		void operator+=(const Vector3& other);
		void operator-=(const Vector3& other);
		void operator*(const Vector3& other);
		void operator/(const Vector3& other);

		// SCALAR
		void operator*(const float amount);
		void operator/(const float amount);

	#pragma endregion

	#pragma endregion

	bool operator==(const Vector3& other) const;
	bool operator!=(const Vector3& other) const;

	#pragma endregion

	#pragma region ===== FUNCTIONS =====

		const double dot(const Vector3& other);
		const double unsafeIndex(int i);
		const double length();
		const double lengthSquared();
		const bool isZero();

	#pragma endregion

};

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

		Point operator+(const Direction& other);
		Point operator-(const Direction& other);

       //endregion

};

class Direction {
    private:

        Vector3 myVect;
        friend class Point;

	public:
		Direction();
		Direction(double scal);
		Direction(double r, double g, double b);


};