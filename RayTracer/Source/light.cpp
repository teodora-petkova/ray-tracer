#include "light.h"

Light::Light()
{
	this->position = Tuple::Point(0, 0, 0);
	this->color = Color(0.2f, 0.2f, 0.2f);
	this->brightness = 0.9f;
	this->ambient = 0.9f;
	this->diffuse = 0.9f;
	this->specular = 0.9f;
}

Light::Light(Tuple position, Color colour,
	float brightness,
	float ambientIntensity,
	float diffuseIntensity,
	float specularIntensity)
{
	this->position = position;
	this->color = colour;
	this->brightness = brightness;
	this->ambient = ambientIntensity;
	this->diffuse = diffuseIntensity;
	this->specular = specularIntensity;
}

Color Light::getPhongColor(Tuple intersection_point,
	Tuple unit_normal, Tuple unit_camera,
	Material* material)
{
	Tuple unit_light = (this->position - intersection_point).normalize();
	Tuple unit_reflected = (-unit_light).reflect(unit_normal).normalize();
	Tuple unit_view = (unit_camera - intersection_point).normalize();

	float ldotN = Tuple::dot(unit_light, unit_normal);
	float rdotV = Tuple::dot(unit_reflected, unit_view);
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