#pragma once

#include "DefaultingTexturalMaterial.h"

class Metal: public DefaultingTexturalMaterial<>
{
	float fuzziness;

public:
	Metal(const Color& albedo, float fuzziness);
	Metal(const Texture* albedo, float fuzziness);
	
	float getFuzziness() const;
	void setFuzziness(float fuzziness);

	virtual bool scatter(ScatterResult& scatterResult, const Ray& ray, const HitResult& hitResult) const override;
};

