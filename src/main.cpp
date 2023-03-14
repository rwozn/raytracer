#include "RayTracer.h"
#include "window/SceneWindow.h"

#include "scene/scenes/CornellBox.h"
#include "scene/scenes/ExampleScene.h"
#include "scene/scenes/TriangleMeshScene.h"

int main(int argc, char* argv[])
{
	CornellBox scene;
	Camera camera(Vector3D(278, 278, -800), Vector3D(278, 278, 0), 40, 1);
	SceneWindow window(500, RayTracer(&scene, &camera, 500));
	
	window.keepDrawing(window.drawScene());

	return 0;
}
