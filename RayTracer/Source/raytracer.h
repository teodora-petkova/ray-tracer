#pragma once

#pragma warning(push, 0)
#include <string.h>
#pragma warning(pop)

#include "raytracer_exports.h"
#include "tuple.h"
#include "scene.h"
#include "canvas.h"

class RAYTRACER_EXPORT RayTracer
{
public:
	RayTracer() {};
	Canvas TraceRays(const Scene& scene) const;
};