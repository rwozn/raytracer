#pragma once

#include "Scene.h"
#include "../texture/Texture.h"
#include "../hittable/HittableList.h"

#include <stack>

// A container that automatically cleans up textures, materials and hittables
/*
	Note: don't make previously added object depend on a newly added one, e.g.:

	box = Box()
	rotation = track(Rotation(box, ...))
	track(box)

	because then `box` is deleted first and then `rotation`, which would have in invalid pointer to the `box`.
	Instead you should add `track` `box` first and then `track` `rotation`.
*/
class TrackingScene: public Scene
{
	// use stack because e.g. when adding transformations you want the last transformation to be deleted first
	// because it should still have access to its subject in the destructor
	/*	(e.g:
		box = track(new Box(...));
		track(new Rotation(box, ...), true); <-- remove rotation first, then the box; vector would remove box first then rotation
	*/
	std::stack<const Texture*> textures;
	std::stack<const Hittable*> hittables;
	std::stack<const Material*> materials;

public:
	TrackingScene(const std::string& name = DEFAULT_NAME);
	~TrackingScene();

	const Texture* track(const Texture* texture);
	const Material* track(const Material* material);
	const Hittable* track(const Hittable* hittable);
};
