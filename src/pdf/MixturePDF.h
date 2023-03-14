#pragma once

#include "PDF.h"

class MixturePDF: public PDF
{
	const PDF* a;
	const PDF* b;

public:
	MixturePDF(const PDF* a, const PDF* b);

	const PDF* getA() const;
	const PDF* getB() const;

	void setA(const PDF* pdf);
	void setB(const PDF* pdf);
	
	virtual Vector3D generateVector() const override;
	virtual float getValue(const Vector3D& direction) const override;
};
