#include "OrthonormalBasis.h"

#include <cmath>

OrthonormalBasis::OrthonormalBasis(const Vector3D& normal)
{
	const Vector3D w = axes[Vector3D::Z] = normal.normalized();

	const Vector3D a = Vector3D::ofStandardBasis(fabs(w.x) > 0.9f ? Vector3D::Y : Vector3D::X);

	const Vector3D v = axes[Vector3D::Y] = w.crossProduct(a).normalized();

	axes[Vector3D::X] = w.crossProduct(v);
}

Vector3D OrthonormalBasis::u() const
{
	return axes[Vector3D::X];
}

Vector3D OrthonormalBasis::v() const
{
	return axes[Vector3D::Y];
}

Vector3D OrthonormalBasis::w() const
{
	return axes[Vector3D::Z];
}

Vector3D OrthonormalBasis::local(const Vector3D& vector) const
{
	Vector3D result;

	for(Vector3D::Axis i = Vector3D::X; i <= Vector3D::Z; i++)
		result += axes[i] * vector[i];

	return result;
}

Vector3D OrthonormalBasis::operator[](Vector3D::Axis axis) const
{
	Vector3D::validateAxis(axis);

	return axes[axis];
}