#include "Color.h"
#include "../Utils.h"

#include <cmath>
#include <string>

const Color Color::RED(1, 0, 0);
const Color Color::GREEN(0, 1, 0);
const Color Color::BLUE(0, 0, 1);

const Color Color::BLACK(0, 0, 0);
const Color Color::WHITE(1, 1, 1);

Color::Color():
	Color(BLACK.r, BLACK.g, BLACK.b)
{

}

Color::Color(const Vector3D& vector):
	Color(vector.x, vector.y, vector.z)
{

}

Color::Color(float r, float g, float b):
	r(r),
	g(g),
	b(b)
{

}

static void clampChannel(float& channel)
{
	channel = Utils::clamp(channel, 0, 1);
}

void Color::clamp()
{
	clampChannel(r);
	clampChannel(g);
	clampChannel(b);
}

static bool isChannelValid(float channel)
{
	return channel >= 0 && channel <= 1;
}

void Color::validateChannels() const
{
	if(!isChannelValid(r))
		throw std::out_of_range("Red channel out of range: " + std::to_string(r));

	if(!isChannelValid(g))
		throw std::out_of_range("Green channel out of range: " + std::to_string(g));

	if(!isChannelValid(b))
		throw std::out_of_range("Blue channel out of range: " + std::to_string(b));
}

static void removeNaN(float& channel, float replacement)
{
	if(std::isnan(channel))
		channel = replacement;
}

void Color::removeNaNs(float replacement)
{
	removeNaN(r, replacement);
	removeNaN(g, replacement);
	removeNaN(b, replacement);
}

Color& Color::operator+=(float scalar)
{
	r += scalar;
	g += scalar;
	b += scalar;

	return *this;
}

Color& Color::operator-=(float scalar)
{
	r -= scalar;
	g -= scalar;
	b -= scalar;

	return *this;
}

Color& Color::operator*=(float scalar)
{
	r *= scalar;
	g *= scalar;
	b *= scalar;

	return *this;
}

Color& Color::operator/=(float scalar)
{
	r /= scalar;
	g /= scalar;
	b /= scalar;

	return *this;
}

Color Color::operator+(float scalar) const
{
	return Color(r + scalar, g + scalar, b + scalar);
}

Color Color::operator-(float scalar) const
{
	return Color(r - scalar, g - scalar, b - scalar);
}

Color Color::operator*(float scalar) const
{
	return Color(r * scalar, g * scalar, b * scalar);
}

Color Color::operator/(float scalar) const
{
	return Color(r / scalar, g / scalar, b / scalar);
}

Color& Color::operator+=(const Color& other)
{
	r += other.r;
	g += other.g;
	b += other.b;

	return *this;
}

Color& Color::operator-=(const Color& other)
{
	r -= other.r;
	g -= other.g;
	b -= other.b;

	return *this;
}

Color& Color::operator*=(const Color& other)
{
	r *= other.r;
	g *= other.g;
	b *= other.b;

	return *this;
}

Color& Color::operator/=(const Color& other)
{
	r /= other.r;
	g /= other.g;
	b /= other.b;

	return *this;
}

Color Color::operator+(const Color& other) const
{
	return Color(r + other.r, g + other.g, b + other.b);
}

Color Color::operator-(const Color& other) const
{
	return Color(r - other.r, g - other.g, b - other.b);
}

Color Color::operator*(const Color& other) const
{
	return Color(r * other.r, g * other.g, b * other.b);
}

Color Color::operator/(const Color& other) const
{
	return Color(r / other.r, g / other.g, b / other.b);
}

std::ostream& operator<<(std::ostream& stream, const Color& color)
{
	return stream << "{r: " << (unsigned)(color.r * 255) << ", g: " << (unsigned)(color.g * 255) << ", b: " << (unsigned)(color.b * 255) << '}';
}
