#pragma once

#include "../../Camera.h"
#include "../TrackingScene.h"

class TriangleMeshScene: public TrackingScene
{
	Camera camera;

	static const std::string OBJ_FILES_DIRECTORY_PATH;

public:
	TriangleMeshScene(const std::string& objFileName);

	const Camera& getCamera();
};
