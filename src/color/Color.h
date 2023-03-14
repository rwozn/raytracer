#pragma once

#include "../Vector3D.h"

struct Color
{
	float r, g, b;

	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;

	static const Color BLACK;
	static const Color WHITE;

	Color();
	Color(const Vector3D& vector);
	Color(float r, float g, float b);

	void clamp();

	void validateChannels() const;

	void removeNaNs(float replacement = 0);

	Color& operator+=(float scalar);
	Color& operator-=(float scalar);
	Color& operator*=(float scalar);
	Color& operator/=(float scalar);

	Color operator+(float scalar) const;
	Color operator-(float scalar) const;
	Color operator*(float scalar) const;
	Color operator/(float scalar) const;

	Color& operator+=(const Color& other);
	Color& operator-=(const Color& other);
	Color& operator*=(const Color& other);
	Color& operator/=(const Color& other);

	Color operator+(const Color& other) const;
	Color operator-(const Color& other) const;
	Color operator*(const Color& other) const;
	Color operator/(const Color& other) const;
};

std::ostream& operator<<(std::ostream& stream, const Color& color);
