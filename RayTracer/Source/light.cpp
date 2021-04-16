#include "light.h"

Light::Light()
{
	this->position = Vector3(0, 0, 0);
	this->color = Color(0.2, 0.2, 0.2);
	this->brightness = 0.9;
	this->ambient = 0.9;
	this->diffuse = 0.9;
	this->specular = 0.9;
}

Light::Light(Vector3 position, Color colour,
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

Color Light::getPhongColor(Vector3 intersection_point,
	Vector3 unit_normal, Vector3 unit_camera,
	Material* material)
{
	Vector3 unit_light = (this->position - intersection_point).normalize();
	Vector3 unit_reflected = (-unit_light).reflect(unit_normal).normalize();
	Vector3 unit_view = (unit_camera - intersection_point).normalize();

	float ldotN = Vector3::dot(unit_light, unit_normal);
	float rdotV = Vector3::dot(unit_reflected, unit_view);
	Color light_color = this->color * this->brightness;

	// ambient
	float ambientness = material->getAmbient() * this->ambient;
	Color ambient = light_color * ambientness;

	// diffuse
	float diffuseness = material->getDiffuse() * this->diffuse;
	Color diffuse = light_color * diffuseness * fmax(ldotN, 0);

	// specular
	float specularness = material->getSpecular() * this->specular;
	float a = fmax(rdotV, 0);
	float b = powf(rdotV, 10);
	Color specular = light_color * powf(fmax(rdotV, 0), material->getShininess()) * specularness;

	Color phong_intensity = ambient + diffuse + specular;

	return material->getColor() * phong_intensity;
}