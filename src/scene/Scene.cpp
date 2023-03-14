#include "Scene.h"

const std::string Scene::DEFAULT_NAME = "unnamed";

Scene::Scene(const std::string& name):
	name(name)
{

}

std::string Scene::getName() const
{
	return name;
}

void Scene::addProp(const Hittable* prop)
{
	props.add(prop);
}

void Scene::addLight(const Hittable* light)
{
	lights.add(light);
}

const HittableList& Scene::getProps() const
{
	return props;
}

const HittableList& Scene::getLights() const
{
	return lights;
}

void Scene::setName(const std::string& name)
{
	this->name = name;
}