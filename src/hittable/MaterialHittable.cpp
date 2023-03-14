#include "MaterialHittable.h"

MaterialHittable::MaterialHittable(const Material* material):
	material(material)
{

}

MaterialHittable::~MaterialHittable()
{

}

const Material* MaterialHittable::getMaterial() const
{
	return material;
}

void MaterialHittable::setMaterial(const Material* material)
{
	this->material = material;
}
