#include "light.h"

Light::Light() {}

Light::Light(Vector3 position, Color colour, float intensity)
{
	this->Position = position;
	this->Colour = colour;
	this->Intensity = intensity;
}