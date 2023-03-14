#pragma once

#include "Image.h"
#include "Camera.h"
#include "scene/Scene.h"

class RayTracer
{
	const Scene* scene;
	const Camera* camera;

	size_t maxDepth;
	size_t samplesPerPixel;

	// this color is used when ray doesn't hit anything
	Color backgroundColor;

	Color getColor(const Ray& ray, int depth) const;

public:
	RayTracer(const Scene* scene, const Camera* camera, size_t samplesPerPixel = 100, size_t maxDepth = 50);

	size_t getMaxDepth() const;
	const Scene* getScene() const;
	const Camera* getCamera() const;
	Color getBackgroundColor() const;
	size_t getSamplesPerPixel() const;
	Color getColor(size_t x, size_t y, size_t width, size_t height) const;
	
	void setMaxDepth(size_t maxDepth);
	void setScene(const Scene* scene);
	void setCamera(const Camera* camera);
	void setBackgroundColor(const Color& color);
	void setSamplesPerPixel(size_t samplesPerPixel);
	
	void colorImage(Image& image) const;
	void colorPixel(Image& image, size_t x, size_t y) const;

	Image createImage(size_t size) const;
	Image createImage(size_t width, size_t height) const;
};

