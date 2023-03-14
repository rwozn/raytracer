#include "../../Utils.h"
#include "TriangleMeshScene.h"

#include "../../hittable/BVHNode.h"
#include "../../hittable/Rectangle.h"
#include "../../hittable/mesh/TriangleMesh.h"

#include "../../material/Diffuse.h"
#include "../../material/DiffuseLight.h"

#include "../../transformation/FaceFlip.h"

#include <cmath>

const std::string TriangleMeshScene::OBJ_FILES_DIRECTORY_PATH = ".objs";

TriangleMeshScene::TriangleMeshScene(const std::string& objFileName):
	TrackingScene(objFileName),
	camera(Vector3D(), Vector3D(), 40)
{
	auto red = track(new Diffuse(Color(0.65f, 0.05f, 0.05f)));
	auto white = track(new Diffuse(Color(0.73f, 0.73f, 0.73f)));
	auto green = track(new Diffuse(Color(0.12f, 0.45f, 0.15f)));
	auto light = track(new DiffuseLight(Color(15, 15, 15)));
	auto black = track(new Diffuse(Color(0.12f, 0.12f, 0.12f)));

	auto mesh = (const TriangleMesh*)track(new TriangleMesh(OBJ_FILES_DIRECTORY_PATH + '/' + objFileName + ".obj", black));
	
	AABB aabb;
	mesh->getBoundingBox(aabb, 0, 0);
	
	const float roomSizeScale = 1.1f;
	
	Vector3D min = aabb.getMin() * Vector3D(roomSizeScale, 1, roomSizeScale);
	Vector3D max = aabb.getMax() * roomSizeScale;

	addProp(track(new Rectangle(min.y, max.y, min.z, max.z, min.x, Vector3D::X, red)));
	addProp(track(new Rectangle(min.y, max.y, min.z, max.z, max.x, Vector3D::X, green)));

	addProp(track(new Rectangle(min.x, max.x, min.z, max.z, min.y, Vector3D::Y, white)));
	addProp(track(new Rectangle(min.x, max.x, min.z, max.z, max.y, Vector3D::Y, white)));
	
	addProp(track(new Rectangle(min.x, max.x, min.y, max.y, max.z, Vector3D::Z, white)));
	
	addProp(track(new BVHNode(mesh->getTriangles(), 0, 0)));

	const float lightSizeScale = 0.4f;
	
	float minx = min.x * lightSizeScale;
	float minz = min.z * lightSizeScale;

	float maxx = max.x * lightSizeScale;
	float maxz = max.z * lightSizeScale;

	float maxy = max.y * 0.999f;
	
	addLight(track(new Rectangle(minx, maxx, minz, maxz, maxy, Vector3D::Y, track(new Material()))));
	addProp(track(new FaceFlip(track(new Rectangle(minx, maxx, minz, maxz, maxy, Vector3D::Y, light)))));

	Vector3D origin = (min + max) / 2;

	Vector3D dimensions = max - min;
	
	camera.setAspectRatio(dimensions.x / dimensions.y);
	
	float vfov = camera.getFov(false);
	float hfov = camera.getHorizontalFov(false);

	// https://stackoverflow.com/questions/21544336/how-to-position-the-camera-so-that-my-main-object-is-entirely-visible-and-fit-to
	float distance = fmin(dimensions.x, dimensions.y) / sin(fmin(vfov, hfov));

	camera.lookAt(origin);
	camera.setOrigin(Vector3D(origin.x, origin.y, min.z - distance));
}

const Camera& TriangleMeshScene::getCamera()
{
	return camera;
}
