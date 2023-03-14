#pragma once

#include "MaterialHittable.h"
#include "../material/Diffuse.h"

template <class DefaultMaterial = Diffuse>
class DefaultingMaterialHittable: public MaterialHittable
{
protected:
	bool createdDefaultMaterial;

	void deleteDefaultMaterial();

public:
	DefaultingMaterialHittable(const Color& color);
	DefaultingMaterialHittable(const Material* material);
	virtual ~DefaultingMaterialHittable();

	virtual void setMaterial(const Material* material) override;
};

template <class DefaultMaterial>
void DefaultingMaterialHittable<DefaultMaterial>::deleteDefaultMaterial()
{
	if(createdDefaultMaterial)
	{
		delete material;

		material = nullptr;

		createdDefaultMaterial = false;
	}
}

template <class DefaultMaterial>
DefaultingMaterialHittable<DefaultMaterial>::DefaultingMaterialHittable(const Color& color):
	MaterialHittable(new DefaultMaterial(color)),
	createdDefaultMaterial(true)
{

}

template <class DefaultMaterial>
DefaultingMaterialHittable<DefaultMaterial>::DefaultingMaterialHittable(const Material* material):
	MaterialHittable(material),
	createdDefaultMaterial(false)
{

}

template <class DefaultMaterial>
DefaultingMaterialHittable<DefaultMaterial>::~DefaultingMaterialHittable()
{
	deleteDefaultMaterial();
}

template <class DefaultMaterial>
void DefaultingMaterialHittable<DefaultMaterial>::setMaterial(const Material* material)
{
	deleteDefaultMaterial();

	MaterialHittable::setMaterial(material);
}
