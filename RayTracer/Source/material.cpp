//---------------------------------------------------------------------
// material.cpp
//---------------------------------------------------------------------
#include "material.h"

Material::Material()
{
	this->color = Color(0.2f, 0.2f, 0.2f);
	this->ambient = 0.1f;
	this->diffuse = 0.9f;
	this->specular = 0.9f;
	this->shininess = 200;
}

Material::Material(Color color, float ambient, float diffuse, float specular,
	float shininess)
{
	this->color = color;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
}
