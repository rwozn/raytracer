#pragma once

#include "../hittable/HittableList.h"

class Scene
{
	HittableList props;
	HittableList lights;

protected:
	std::string name;
	
	static const std::string DEFAULT_NAME;

public:
	Scene(const std::string& name = DEFAULT_NAME);

	std::string getName() const;
	const HittableList& getProps() const;
	const HittableList& getLights() const;
	
	void addProp(const Hittable* prop);
	void addLight(const Hittable* light);
	void setName(const std::string& name);
};
