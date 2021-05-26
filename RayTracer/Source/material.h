//---------------------------------------------------------------------
// material.h Header
//---------------------------------------------------------------------
#ifndef MATERIAL_H
#define MATERIAL_H

#include "raytracer_exports.h"
#include "tuple.h"
#include "color.h"

class RAYTRACER_EXPORT Material
{
public:
	Material();

	Material(const Color& color,
		float ambient, float diffuse,
		float specular, float shininess);

	Color getColor() const { return color; }
	float getAmbient() const { return ambient; }
	float getDiffuse() const { return diffuse; }
	float getShininess() const { return shininess; }
	float getSpecular() const { return specular; }

private:
	Color color;
	float ambient;
	float diffuse;
	float specular;
	float shininess;
};

using MaterialPtr = std::shared_ptr<Material>;

#endif