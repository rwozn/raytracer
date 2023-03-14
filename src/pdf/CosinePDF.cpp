#include "../Utils.h"
#include "CosinePDF.h"

CosinePDF::CosinePDF(const Vector3D& normal):
	uvw(normal)
{

}

Vector3D CosinePDF::generateVector() const
{
	return uvw.local(Vector3D::randomCosineDirection());
}

float CosinePDF::getValue(const Vector3D& direction) const
{
	float cosine = direction.normalized().dotProduct(uvw.w());

	return cosine <= 0 ? 0 : cosine / Utils::Math::pi;
}
