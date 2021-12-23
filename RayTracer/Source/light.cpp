#include "light.h"
#include "object.h"

Color Light::CalculatePhongColor(const Tuple& intersectionPoint,
	const Tuple& unitNormal,
	const Tuple& cameraPosition,
	const ObjectPtr& object,
	bool isInShadow) const
{
	Color phongIntensity = Color::Black();

	Color lightColor = this->color * this->brightness;

	// ambient
	float ambientness = object->getMaterial()->getAmbient() * this->ambient;
	Color ambientColor = lightColor * ambientness;

	if (isInShadow)
	{
		phongIntensity = ambientColor;
	}
	else
	{
		Tuple unitLight = (this->position - intersectionPoint).Normalize();
		Tuple unitReflected = (-unitLight).Reflect(unitNormal).Normalize();
		Tuple unitView = (cameraPosition - intersectionPoint).Normalize();

		float ldotN = unitLight.Dot(unitNormal);
		float rdotV = unitReflected.Dot(unitView);

		// diffuse
		float diffuseness = object->getMaterial()->getDiffuse() * this->diffuse;
		Color diffuseColor = lightColor * diffuseness * fmax(ldotN, 0.0f);

		// specular
		float specularness = object->getMaterial()->getSpecular() * this->specular;
		Color specularColor = lightColor * powf(fmax(rdotV, 0.0f), object->getMaterial()->getShininess()) * specularness;

		phongIntensity = ambientColor + diffuseColor + specularColor;
	}

	return object->getMaterial()->getPattern()->getColorAtObject(object, intersectionPoint) * phongIntensity;
}