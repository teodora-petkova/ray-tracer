#ifndef CANVAS_H
#define CANVAS_H

#pragma once

#pragma warning(push, 0)
#include <string.h>
#pragma warning(pop)

#include "raytracer_exports.h"
#include "color.h"

class RAYTRACER_EXPORT Canvas
{
public:
	Canvas(int width, int height);
	// a only movable object!
	Canvas(const Canvas& other) = delete;
	Canvas(Canvas&& other);
	~Canvas();

	int getWidth() const { return width; }
	int getHeight() const { return height; }

	Color PixelAt(int x, int y) const;
	void WritePixel(int x, int y, const Color& color);

	Color* pixels;
private:
	int width;
	int height;
	// the (0,0) point is at bottom-left!
};

#endif