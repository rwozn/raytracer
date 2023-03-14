#include "TrackingScene.h"

template <typename T>
static void deleteStack(std::stack<T>& stack)
{
	while(!stack.empty())
	{
		delete stack.top();

		stack.pop();
	}
}

TrackingScene::TrackingScene(const std::string& name):
	Scene(name)
{

}

TrackingScene::~TrackingScene()
{
	// first delete hittables because they might use materials
	deleteStack(hittables);

	// then delete materials because they might use textures
	deleteStack(materials);

	// at the end delete textures because nothing uses them anymore
	deleteStack(textures);
}

const Texture* TrackingScene::track(const Texture* texture)
{
	textures.push(texture);

	return texture;
}

const Material* TrackingScene::track(const Material* material)
{
	materials.push(material);

	return material;
}

const Hittable* TrackingScene::track(const Hittable* hittable)
{
	hittables.push(hittable);
	
	return hittable;
}
