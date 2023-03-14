#pragma once

#include "Material.h"
#include "../texture/Texture.h"

class TexturalMaterial: public Material
{
protected:
	const Texture* texture;

public:
	TexturalMaterial(const Texture* texture);
	virtual ~TexturalMaterial();

	const Texture* getTexture() const;
	virtual void setTexture(const Texture* texture);
};
