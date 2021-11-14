#include "light.h"

Color Light::CalculatePhongColor(const Tuple& intersection_point,
	const Tuple& unit_normal,
	const Tuple& camera_position,
	MaterialPtr material) const
{
	Tuple unit_light = (this->position - intersection_point).Normalize();
	Tuple unit_reflected = (-unit_light).Reflect(unit_normal).Normalize();
	Tuple unit_view = (camera_position - intersection_point).Normalize();

	float ldotN = unit_light.Dot(unit_normal);
	float rdotV = unit_reflected.Dot(unit_view);
	Color light_color = this->color * this->brightness;

	// ambient
	float ambientness = material->getAmbient() * this->ambient;
	Color ambient_color = light_color * ambientness;

	// diffuse
	float diffuseness = material->getDiffuse() * this->diffuse;
	Color diffuse_color = light_color * diffuseness * fmax(ldotN, 0.0f);

	// specular
	float specularness = material->getSpecular() * this->specular;
	Color specular_color = light_color * powf(fmax(rdotV, 0.0f), material->getShininess()) * specularness;

	Color phong_intensity = ambient_color + diffuse_color + specular_color;

	return material->getColor() * phong_intensity;
}