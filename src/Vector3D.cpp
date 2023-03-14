#include "Utils.h"
#include "Vector3D.h"

#include <cmath>
#include <string>

Vector3D::Vector3D():
	Vector3D(0, 0, 0)
{

}

Vector3D::Vector3D(float x, float y, float z):
	x(x),
	y(y),
	z(z)
{

}

bool Vector3D::nearZero() const
{
	return fabs(x) < Utils::epsilon && fabs(y) < Utils::epsilon && fabs(z) < Utils::epsilon;
}

void Vector3D::validateAxis(Axis axis)
{
	if(axis >= AXES_COUNT)
		throw std::out_of_range("Invalid axis: " + std::to_string(axis) + "; must be " + std::to_string(X) + ", " + std::to_string(Y) + " or " + std::to_string(Z));
}

Vector3D Vector3D::random()
{
	return random(0, 1);
}

Vector3D Vector3D::random(float min, float max)
{
	return Vector3D(Utils::randomFloat(min, max), Utils::randomFloat(min, max), Utils::randomFloat(min, max));
}

Vector3D Vector3D::ofStandardBasis(Axis axis)
{
	return ofScaledStandardBasis(axis, 1);
}

Vector3D Vector3D::ofScaledStandardBasis(Axis axis, float scale)
{
	return Vector3D(axis == X ? scale : 0, axis == Y ? scale : 0, axis == Z ? scale : 0);
}

Vector3D Vector3D::filled(float value)
{
	return Vector3D(value, value, value);
}

Vector3D Vector3D::filledAlongAxis(float a, float b, Axis axis, float k)
{
	Axis axisA, axisB;

	return filledAlongAxis(a, b, axis, k, axisA, axisB);
}

Vector3D Vector3D::filledAlongAxis(float a, float b, Axis axis, float k, Axis& axisA, Axis& axisB)
{
	Vector3D vector = ofScaledStandardBasis(axis, k);

	getRemainingAxes(axis, axisA, axisB);

	vector[axisA] = a;
	vector[axisB] = b;

	return vector;
}

Vector3D Vector3D::randomInUnitDisk()
{
	while(1)
	{
		Vector3D point(Utils::randomFloat(-1, 1), Utils::randomFloat(-1, 1), 0);

		if(point.lengthSquared() < 1)
			return point;
	}
}

Vector3D Vector3D::randomUnitVector()
{
	return randomInUnitSphere().normalized();
}

Vector3D Vector3D::randomInUnitSphere()
{
	while(1)
	{
		Vector3D point = random(-1, 1);

		if(point.lengthSquared() < 1)
			return point;
	}
}

Vector3D Vector3D::randomCosineDirection()
{
	const float a = Utils::randomFloat();
	const float b = Utils::randomFloat();

	float phi = 2 * Utils::Math::pi * a;

	float sqrtb = sqrt(b);

	float x = cos(phi) * sqrtb;
	float y = sin(phi) * sqrtb;
	float z = sqrt(1 - b);

	return Vector3D(x, y, z);
}

Vector3D Vector3D::randomInHemisphere(const Vector3D& normal)
{
	Vector3D inUnitSphere = randomInUnitSphere();

	// dot > 0 => in the same hemisphere as the normal
	return inUnitSphere.dotProduct(normal) > 0 ? inUnitSphere : -inUnitSphere;
}

Vector3D Vector3D::randomToSphere(float radius, float distanceSquared)
{
	const float a = Utils::randomFloat();
	const float b = Utils::randomFloat();

	float z = 1 + b * (sqrt(1 - radius * radius / distanceSquared) - 1);

	float phi = 2 * Utils::Math::pi * a;

	float sqrtz = sqrt(1 - z * z);

	float x = cos(phi) * sqrtz;
	float y = sin(phi) * sqrtz;

	return Vector3D(x, y, z);
}

void Vector3D::getRemainingAxes(Axis axis, Axis& axisA, Axis& axisB)
{
	validateAxis(axis);

	if(axis == Vector3D::X)
	{
		axisA = Vector3D::Y;
		axisB = Vector3D::Z;

		return;
	}

	if(axis == Vector3D::Y)
	{
		axisA = Vector3D::X;
		axisB = Vector3D::Z;

		return;
	}

	axisA = Vector3D::X;
	axisB = Vector3D::Y;
}

Vector3D Vector3D::min(const Vector3D& other) const
{
	return Vector3D(fmin(x, other.x), fmin(y, other.y), fmin(z, other.z));
}

Vector3D Vector3D::max(const Vector3D& other) const
{
	return Vector3D(fmax(x, other.x), fmax(y, other.y), fmax(z, other.z));
}

Vector3D Vector3D::reflect(const Vector3D& normal) const
{
	return *this - normal * dotProduct(normal) * 2;
}

Vector3D Vector3D::refract(const Vector3D& normal, float refractionRatio) const
{
	float cosTheta = fmin((-*this).dotProduct(normal), 1.f);

	Vector3D perpendicular = (*this + normal * cosTheta) * refractionRatio;
	Vector3D parallel = normal * -sqrt(fabs(1 - perpendicular.lengthSquared()));

	return perpendicular + parallel;
}

float Vector3D::length() const
{
	return sqrt(lengthSquared());
}

float Vector3D::lengthSquared() const
{
	return x * x + y * y + z * z;
}

Vector3D Vector3D::normalized() const
{
	return *this / length();
}

void Vector3D::rotate(Axis axis, float angle)
{
	Axis axisA, axisB;

	getRemainingAxes(axis, axisA, axisB);

	float& a = (*this)[axisA];
	float& b = (*this)[axisB];

	const float sine = sin(angle);
	const float cosine = cos(angle);

	const float sina = sine * a;
	const float sinb = sine * b;

	a = cosine * a + (axis == Y ? sinb : -sinb);
	b = (axis == Y ? -sina : sina) + cosine * b;
}

Vector3D Vector3D::rotated(Axis axis, float angle) const
{
	Vector3D copy = Vector3D(*this);

	copy.rotate(axis, angle);

	return copy;
}

float Vector3D::dotProduct(const Vector3D& other) const
{
	return	x * other.x +
			y * other.y +
			z * other.z;
}

Vector3D Vector3D::crossProduct(const Vector3D& other) const
{
	return Vector3D(y * other.z - z * other.y,
					z * other.x - x * other.z,
					x * other.y - y * other.x);
}

Vector3D Vector3D::operator-() const
{
	return Vector3D(-x, -y, -z);
}

float& Vector3D::operator[](Axis axis)
{
	validateAxis(axis);

	switch(axis)
	{
		case X: return x;
		case Y: return y;
	}

	return z;
}

float Vector3D::operator[](Axis axis) const
{
	validateAxis(axis);

	switch(axis)
	{
		case X: return x;
		case Y: return y;
	}

	return z;
}

Vector3D& Vector3D::operator+=(float scalar)
{
	x += scalar;
	y += scalar;
	z += scalar;

	return *this;
}

Vector3D& Vector3D::operator-=(float scalar)
{
	x -= scalar;
	y -= scalar;
	z -= scalar;

	return *this;
}

Vector3D& Vector3D::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;

	return *this;
}

Vector3D& Vector3D::operator/=(float scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;

	return *this;
}

Vector3D Vector3D::operator+(float scalar) const
{
	return Vector3D(x + scalar, y + scalar, z + scalar);
}

Vector3D Vector3D::operator-(float scalar) const
{
	return Vector3D(x - scalar, y - scalar, z - scalar);
}

Vector3D Vector3D::operator*(float scalar) const
{
	return Vector3D(x * scalar, y * scalar, z * scalar);
}

Vector3D Vector3D::operator/(float scalar) const
{
	return Vector3D(x / scalar, y / scalar, z / scalar);
}

Vector3D& Vector3D::operator+=(const Vector3D& other)
{
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;

	return *this;
}

Vector3D& Vector3D::operator*=(const Vector3D& other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;

	return *this;
}

Vector3D& Vector3D::operator/=(const Vector3D& other)
{
	x /= other.x;
	y /= other.y;
	z /= other.z;

	return *this;
}

Vector3D Vector3D::operator+(const Vector3D& other) const
{
	return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D Vector3D::operator-(const Vector3D& other) const
{
	return Vector3D(x - other.x, y - other.y, z - other.z);
}

Vector3D Vector3D::operator*(const Vector3D& other) const
{
	return Vector3D(x * other.x, y * other.y, z * other.z);
}

Vector3D Vector3D::operator/(const Vector3D& other) const
{
	return Vector3D(x / other.x, y / other.y, z / other.z);
}

std::ostream& operator<<(std::ostream& stream, const Vector3D& vector)
{
	return stream << "{x: " << vector.x << ", y: " << vector.y << ", z: " << vector.z << '}';
}
