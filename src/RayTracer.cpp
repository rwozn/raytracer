#include "Utils.h"
#include "RayTracer.h"
#include "pdf/MixturePDF.h"
#include "pdf/HittablePDF.h"

#include <cmath>

Color RayTracer::getColor(const Ray& ray, int depth) const
{
	// If we've exceeded the ray bounce limit, no more light is gathered.
	if(depth <= 0)
		return Color::BLACK;

	HitResult hitResult;

	// 0.001 fixes shadow acne
	// If the ray hits nothing, return background color
	if(!scene->getProps().hit(hitResult, ray, 0.001f, Utils::Math::infinity))
		return backgroundColor;

	ScatterResult scatterResult;

	Color emittedColor = hitResult.hitMaterial->emitColor(ray, hitResult, hitResult.hitPoint, hitResult.u, hitResult.v);

	if(!hitResult.hitMaterial->scatter(scatterResult, ray, hitResult))
		return emittedColor;

	if(scatterResult.isSpecular())
		return scatterResult.attenuation * getColor(scatterResult.specularRay, depth - 1);

	const PDF* scatterPdf = scatterResult.pdf;

	// return black because there are no lights. Might as well throw std::logic_error
	if(scene->getLights().isEmpty())
	{
		// clean up the pdf
		delete scatterPdf;

		return Color::BLACK;
	}

	HittablePDF lightPdf(&scene->getLights(), hitResult.hitPoint);

	MixturePDF mixturePdf(&lightPdf, scatterPdf);

	Ray scatteredRay(hitResult.hitPoint, mixturePdf.generateVector(), ray.getTime());

	float mixturePdfValue = mixturePdf.getValue(scatteredRay.getDirection());

	// clean up the pdf
	delete scatterPdf;

	Color color = getColor(scatteredRay, depth - 1) / mixturePdfValue;

	float scatteringPdf = hitResult.hitMaterial->getScatteringPdf(ray, hitResult, scatteredRay);

	return emittedColor + scatterResult.attenuation * color * scatteringPdf;
}

RayTracer::RayTracer(const Scene* scene, const Camera* camera, size_t samplesPerPixel, size_t maxDepth):
	scene(scene),
	camera(camera),
	maxDepth(maxDepth),
	samplesPerPixel(samplesPerPixel),
	backgroundColor(Color::BLACK)
{

}

size_t RayTracer::getMaxDepth() const
{
	return maxDepth;
}

const Scene* RayTracer::getScene() const
{
	return scene;
}

const Camera* RayTracer::getCamera() const
{
	return camera;
}

Color RayTracer::getBackgroundColor() const
{
	return backgroundColor;
}

size_t RayTracer::getSamplesPerPixel() const
{
	return samplesPerPixel;
}

Color RayTracer::getColor(size_t x, size_t y, size_t width, size_t height) const
{
	Color color;

	for(size_t i = 0; i < samplesPerPixel; i++)
	{
		float s = (Utils::randomFloat() + x) / (width - 1);
		float t = (Utils::randomFloat() + height - 1 - y) / (height - 1);

		Color sample = getColor(camera->getRay(s, t), maxDepth);

		// Replace NaN components with zero
		/*
		* Monte Carlo ray tracers average a pixel color using many samples.
		* One "bad" sample can kill the whole pixel if it's a huge number or a NaN.
		*/
		sample.removeNaNs();

		color += sample;
	}

	// Divide the color by the number of samples
	color /= samplesPerPixel;

	// gamma-correct for gamma=2.0
	color.r = sqrt(color.r);
	color.g = sqrt(color.g);
	color.b = sqrt(color.b);

	color.clamp();

	return color;
}

void RayTracer::setMaxDepth(size_t maxDepth)
{
	this->maxDepth = maxDepth;
}

void RayTracer::setScene(const Scene* scene)
{
	this->scene = scene;
}

void RayTracer::setCamera(const Camera* camera)
{
	this->camera = camera;
}

void RayTracer::setBackgroundColor(const Color& color)
{
	backgroundColor = color;
}

void RayTracer::setSamplesPerPixel(size_t samplesPerPixel)
{
	this->samplesPerPixel = samplesPerPixel;
}

void RayTracer::colorImage(Image& image) const
{
	const size_t width = image.getWidth();
	const size_t height = image.getHeight();

	for(size_t i = 0; i < width; i++)
		for(size_t j = 0; j < height; j++)
			colorPixel(image, i, j);
}

void RayTracer::colorPixel(Image& image, size_t x, size_t y) const
{
	image.setPixel(x, y, getColor(x, y, image.getWidth(), image.getHeight()));
}

Image RayTracer::createImage(size_t size) const
{
	return createImage(size, size);
}

Image RayTracer::createImage(size_t width, size_t height) const
{
	Image image(width, height);

	colorImage(image);

	return image;
}
