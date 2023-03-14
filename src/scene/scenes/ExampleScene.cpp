#include "ExampleScene.h"

#include "../../hittable/Box.h"
#include "../../hittable/Sphere.h"
#include "../../hittable/Rectangle.h"

#include "../../texture/ImageTexture.h"

#include "../../material/Metal.h"
#include "../../material/Diffuse.h"
#include "../../material/DiffuseLight.h"

#include "../../transformation/FaceFlip.h"
#include "../../transformation/Rotation.h"
#include "../../transformation/SpatialTranslation.h"

ExampleScene::ExampleScene():
	TrackingScene("Eye and mirror")
{
	auto red = track(new Diffuse(Color(0.65f, 0.05f, 0.05f)));
	auto white = track(new Diffuse(Color(0.73f, 0.73f, 0.73f)));
	auto green = track(new Diffuse(Color(0.12f, 0.45f, 0.15f)));
	auto light = track(new DiffuseLight(Color(30, 30, 30)));

	addProp(track(new Rectangle(0, 555, 0, 555, 0, Vector3D::X, green)));
	addProp(track(new Rectangle(0, 555, 0, 555, 555, Vector3D::X, red)));

	addProp(track(new Rectangle(0, 555, 0, 555, 0, Vector3D::Y, white)));
	addProp(track(new Rectangle(0, 555, 0, 555, 555, Vector3D::Y, white)));

	addProp(track(new Rectangle(0, 555, 0, 555, 555, Vector3D::Z, white)));

	addLight(track(new Rectangle(213, 343, 227, 332, 554, Vector3D::Y, track(new Material()))));
	addProp(track(new FaceFlip(track(new Rectangle(213, 343, 227, 332, 554, Vector3D::Y, light)))));

	auto box = track(new Box(Vector3D(0, 0, 0), Vector3D(195, 420, 165), white));
	box = track(new Rotation(box, Vector3D::Y, 22));
	box = track(new SpatialTranslation(box, Vector3D(290, 0, 295)));
	addProp(box);

	addProp(track(new Sphere(Vector3D(90, 90, 190), 120, track(new Diffuse(track(new ImageTexture(Image("textures/eye.png"))))))));

	addProp(track(new Sphere(Vector3D(290, 50, 280), 100, track(new Metal(Color(0.8f, 0.85f, 0.88f), 0)))));
}
