#include <iostream>


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
		void operator-(const Vector3& other);
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

};