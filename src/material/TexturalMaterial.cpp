#include "TexturalMaterial.h"

TexturalMaterial::TexturalMaterial(const Texture* texture):
	texture(texture)
{

}

TexturalMaterial::~TexturalMaterial()
{

}

const Texture* TexturalMaterial::getTexture() const
{
	return texture;
}

void TexturalMaterial::setTexture(const Texture* texture)
{
	this->texture = texture;
}