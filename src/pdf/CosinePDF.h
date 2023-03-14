#pragma once

#include "PDF.h"
#include "../OrthonormalBasis.h"

class CosinePDF: public PDF
{
	OrthonormalBasis uvw;

public:
	CosinePDF(const Vector3D& normal);

	virtual Vector3D generateVector() const override;
	virtual float getValue(const Vector3D& direction) const override;
};
