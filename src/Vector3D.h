#pragma once

#include <ostream>

class Vector3D
{
public:
	using Axis = size_t;

	static constexpr Axis X = 0;
	static constexpr Axis Y = X + 1;
	static constexpr Axis Z = Y + 1;

	static constexpr size_t AXES_COUNT = Z + 1;

	float x, y, z;

	Vector3D();
	Vector3D(float x, float y, float z);

	bool nearZero() const;

	static void validateAxis(Axis axis);

	static Vector3D random();
	static Vector3D random(float min, float max);

	static Vector3D ofStandardBasis(Axis axis);
	static Vector3D ofScaledStandardBasis(Axis axis, float scale);

	static Vector3D filled(float value);

	// sets vector[axis]=k, vector[axisA]=a, vector[axisB]=b
	// where axisA and axisB are the remaining axes in order
	// (e.g. for axis=Y axisA=X, axisB=Z)
	static Vector3D filledAlongAxis(float a, float b, Axis axis, float k);
	static Vector3D filledAlongAxis(float a, float b, Axis axis, float k, Axis& axisA, Axis& axisB);

	static Vector3D randomInUnitDisk();
	static Vector3D randomUnitVector();
	static Vector3D randomInUnitSphere();
	static Vector3D randomCosineDirection();
	static Vector3D randomInHemisphere(const Vector3D& normal);
	static Vector3D randomToSphere(float radius, float distanceSquared);
	
	static void getRemainingAxes(Axis axis, Axis& axisA, Axis& axisB);
	
	Vector3D min(const Vector3D& other) const;
	Vector3D max(const Vector3D& other) const;

	Vector3D reflect(const Vector3D& normal) const;
	Vector3D refract(const Vector3D& normal, float refractionRatio) const;

	float length() const;
	float lengthSquared() const;

	Vector3D normalized() const;

	// angle is in radians
	void rotate(Axis axis, float angle);
	Vector3D rotated(Axis axis, float angle) const;

	float dotProduct(const Vector3D& other) const;
	Vector3D crossProduct(const Vector3D& other) const;

	Vector3D operator-() const;

	float& operator[](Axis axis);
	float operator[](Axis axis) const;

	Vector3D& operator+=(float scalar);
	Vector3D& operator-=(float scalar);
	Vector3D& operator*=(float scalar);
	Vector3D& operator/=(float scalar);

	Vector3D operator+(float scalar) const;
	Vector3D operator-(float scalar) const;
	Vector3D operator*(float scalar) const;
	Vector3D operator/(float scalar) const;

	Vector3D& operator+=(const Vector3D& other);
	Vector3D& operator-=(const Vector3D& other);
	Vector3D& operator*=(const Vector3D& other);
	Vector3D& operator/=(const Vector3D& other);

	Vector3D operator+(const Vector3D& other) const;
	Vector3D operator-(const Vector3D& other) const;
	Vector3D operator*(const Vector3D& other) const;
	Vector3D operator/(const Vector3D& other) const;
};

std::ostream& operator<<(std::ostream& stream, const Vector3D& vector);