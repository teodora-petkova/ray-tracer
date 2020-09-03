#include "distantlight.h"

DistantLight::DistantLight(Vector3 position, Color colour, float intensity)
	:Light(position, colour, intensity)
{
	// lightDir = dir;
	//lightIntensity = colour * intensity;
	//distance = kInfinity;
}