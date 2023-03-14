#include "StaticHittable.h"

StaticHittable::StaticHittable(const Vector3D& origin, const Color& color):
	DefaultingMaterialHittable(color),
	origin(origin)
{

}

StaticHittable::StaticHittable(const Vector3D& origin, const Material* material):
	DefaultingMaterialHittable(material),
	origin(origin)
{

}

Vector3D StaticHittable::getOrigin() const
{
	return origin;
}

void StaticHittable::setOrigin(const Vector3D& origin)
{
	this->origin = origin;
}
