#pragma once

#include "Ray.h"

class Camera
{
	// Vertical field of view in radians
	float fov;
	float aspectRatio;

	float aperture;
	float lensRadius;
	float focusDistance;

	float viewportWidth;
	float viewportHeight;

	float shutterOpenTime;
	float shutterCloseTime;

	Vector3D u, v, w;
	
	Vector3D origin;
	Vector3D lookPoint;

	Vector3D up;
	Vector3D vertical;
	Vector3D horizontal;

	Vector3D lowerLeftCorner;

	void updateViewport();

	void updateAperture(float aperture);

public:
	Camera(const Vector3D& origin, const Vector3D& lookPoint, const float fov = 90, float aspectRatio = 16.f / 9.f, float aperture = 0, float focusDistance = 1);
	
	float getAperture() const;
	Vector3D getOrigin() const;
	float getAspectRatio() const;
	Vector3D getLookPoint() const;
	float getFocusDistance() const;
	float getShutterOpenTime() const;
	Vector3D getLookDirection() const;
	float getShutterCloseTime() const;
	float getFov(bool inDegrees = true) const;
	float getHorizontalFov(bool inDegrees = true) const;
	
	void setFov(float fov);
	void setAperture(float aperture);
	void setShutterOpenTime(float time);
	void setShutterCloseTime(float time);
	void setOrigin(const Vector3D& origin);
	void setAspectRatio(float aspectRatio);
	void setFocusDistance(float focusDistance);
	void setShutterTimes(float openTime, float closeTime);
	
	Ray getRay(float s, float t) const;
	
	void lookAt(const Vector3D& lookPoint);
	void setLookDirection(const Vector3D& lookDirection);
};

