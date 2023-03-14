#pragma once

#include "Vector3D.h"

class OrthonormalBasis
{
	Vector3D axes[Vector3D::AXES_COUNT];
	
public:
	OrthonormalBasis(const Vector3D& normal);

	Vector3D u() const;
	Vector3D v() const;
	Vector3D w() const;

	Vector3D local(const Vector3D& vector) const;

	Vector3D operator[](Vector3D::Axis axis) const;
};
