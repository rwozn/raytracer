#pragma once

#include "../hittable/Hittable.h"

class Transformation: public Hittable
{
protected:
	const Hittable* subject;

public:
	Transformation(const Hittable* subject);

	const Hittable* getSubject() const;
	virtual void setSubject(const Hittable* hittable);
};

