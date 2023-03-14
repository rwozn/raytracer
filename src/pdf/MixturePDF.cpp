#include "../Utils.h"
#include "MixturePDF.h"

MixturePDF::MixturePDF(const PDF* a, const PDF* b):
	a(a),
	b(b)
{

}

const PDF* MixturePDF::getA() const
{
	return a;
}

const PDF* MixturePDF::getB() const
{
	return b;
}

void MixturePDF::setA(const PDF* pdf)
{
	a = pdf;
}

void MixturePDF::setB(const PDF* pdf)
{
	b = pdf;
}

Vector3D MixturePDF::generateVector() const
{
	const PDF* pdf = Utils::randomFloat() < 0.5f ? a : b;

	return pdf->generateVector();
}

float MixturePDF::getValue(const Vector3D& direction) const
{
	return 0.5f * a->getValue(direction) + 0.5f * b->getValue(direction);
}
