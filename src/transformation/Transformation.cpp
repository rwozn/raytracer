#include "Transformation.h"

Transformation::Transformation(const Hittable* subject):
	subject(subject)
{

}

const Hittable* Transformation::getSubject() const
{
	return subject;
}

void Transformation::setSubject(const Hittable* hittable)
{
	subject = hittable;
}