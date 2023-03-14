#include "Utils.h"
#include "Camera.h"

#include <cmath>

void Camera::updateViewport()
{
	w = (origin - lookPoint).normalized();
	u = up.crossProduct(w).normalized();
	v = w.crossProduct(u);

	float h = tan(fov / 2);

	viewportHeight = h * 2;
	viewportWidth = viewportHeight * aspectRatio;

	vertical = v * viewportHeight * focusDistance;
	horizontal = -u * viewportWidth * focusDistance;

	lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - w * focusDistance;
}

void Camera::updateAperture(float aperture)
{
	this->aperture = aperture;

	lensRadius = aperture / 2;
}

Camera::Camera(const Vector3D& origin, const Vector3D& lookPoint, float fov, float aspectRatio, float aperture, float focusDistance):
	fov(Utils::Math::deg2rad(fov)),
	aspectRatio(aspectRatio),
	focusDistance(focusDistance),
	shutterOpenTime(0),
	shutterCloseTime(0),
	origin(origin),
	lookPoint(lookPoint),
	up(Vector3D::ofStandardBasis(Vector3D::Y))
{
	updateAperture(aperture);

	updateViewport();
}

float Camera::getAperture() const
{
	return aperture;
}

Vector3D Camera::getOrigin() const
{
	return origin;
}

float Camera::getAspectRatio() const
{
	return aspectRatio;
}

Vector3D Camera::getLookPoint() const
{
	return lookPoint;
}

float Camera::getFocusDistance() const
{
	return focusDistance;
}

float Camera::getShutterOpenTime() const
{
	return shutterOpenTime;
}

Vector3D Camera::getLookDirection() const
{
	return lookPoint - origin;
}

float Camera::getShutterCloseTime() const
{
	return shutterCloseTime;
}

float Camera::getFov(bool inDegrees) const
{
	return inDegrees ? Utils::Math::rad2deg(fov) : fov;
}

// https://b3d.interplanety.org/en/vertical-and-horizontal-camera-fov-angles/
float Camera::getHorizontalFov(bool inDegrees) const
{
	float horizontalFov = atan(0.5f * viewportWidth * tan(0.5f * fov) / (0.5f * viewportHeight)) * 2;

	return inDegrees ? Utils::Math::rad2deg(horizontalFov) : horizontalFov;
}

void Camera::setFov(float fov)
{
	this->fov = Utils::Math::deg2rad(fov);

	updateViewport();
}

void Camera::setAperture(float aperture)
{
	updateAperture(aperture);

	updateViewport();
}

void Camera::setShutterOpenTime(float time)
{
	shutterOpenTime = time;
}

void Camera::setShutterCloseTime(float time)
{
	shutterCloseTime = time;
}

void Camera::setOrigin(const Vector3D& origin)
{
	this->origin = origin;

	updateViewport();
}

void Camera::setAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;

	updateViewport();
}

void Camera::setFocusDistance(float focusDistance)
{
	this->focusDistance = focusDistance;

	updateViewport();
}

void Camera::setShutterTimes(float openTime, float closeTime)
{
	setShutterOpenTime(openTime);
	setShutterCloseTime(closeTime);
}

Ray Camera::getRay(float s, float t) const
{
	Vector3D randomInUnitDisk = Vector3D::randomInUnitDisk() * lensRadius;

	Vector3D offset = u * randomInUnitDisk.x + v * randomInUnitDisk.y;

	return Ray(origin + offset, lowerLeftCorner + horizontal * s + vertical * t - origin - offset, Utils::randomFloat(shutterOpenTime, shutterCloseTime));
}

void Camera::lookAt(const Vector3D& lookPoint)
{
	this->lookPoint = lookPoint;

	updateViewport();
}

void Camera::setLookDirection(const Vector3D& lookDirection)
{
	lookPoint = origin + lookDirection;

	updateViewport();
}
