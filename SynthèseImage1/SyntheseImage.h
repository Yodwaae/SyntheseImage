#include <iostream>


class Vector3 {

	public :
		double a;
		double b;
		double c;

		Vector3();

		Vector3(double x, double y, double z);

		Vector3 operator+(const Vector3& other);

};