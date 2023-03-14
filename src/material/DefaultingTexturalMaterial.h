#pragma once

#include "TexturalMaterial.h"
#include "../texture/SolidColorTexture.h"

template <class DefaultTexture = SolidColorTexture>
class DefaultingTexturalMaterial: public TexturalMaterial
{
protected:
	bool createdDefaultTexture;

	void deleteDefaultTexture();

public:
	DefaultingTexturalMaterial(const Color& color);
	DefaultingTexturalMaterial(const Texture* texture);
	virtual ~DefaultingTexturalMaterial();

	virtual void setTexture(const Texture* texture) override;
};

template <class DefaultTexture>
void DefaultingTexturalMaterial<DefaultTexture>::deleteDefaultTexture()
{
	if(createdDefaultTexture)
	{
		delete texture;

		texture = nullptr;

		createdDefaultTexture = false;
	}
}

template <class DefaultTexture>
DefaultingTexturalMaterial<DefaultTexture>::DefaultingTexturalMaterial(const Color& color):
	TexturalMaterial(new DefaultTexture(color)),
	createdDefaultTexture(true)
{

}

template <class DefaultTexture>
DefaultingTexturalMaterial<DefaultTexture>::DefaultingTexturalMaterial(const Texture* texture):
	TexturalMaterial(texture),
	createdDefaultTexture(false)
{

}

template <class DefaultTexture>
DefaultingTexturalMaterial<DefaultTexture>::~DefaultingTexturalMaterial()
{
	deleteDefaultTexture();
}

template <class DefaultTexture>
void DefaultingTexturalMaterial<DefaultTexture>::setTexture(const Texture* texture)
{
	deleteDefaultTexture();

	TexturalMaterial::setTexture(texture);
}
