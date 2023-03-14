#include "SolidColorTexture.h"

SolidColorTexture::SolidColorTexture(const Color& color):
    color(color)
{

}

Color SolidColorTexture::getColor() const
{
    return color;
}

void SolidColorTexture::setColor(const Color& color)
{
    this->color = color;
}

Color SolidColorTexture::getTextureColor(const Vector3D& point, float u, float v) const
{
    return color;
}
