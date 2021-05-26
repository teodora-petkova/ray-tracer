#include "canvas.h"

Canvas::Canvas(int width, int height)
	:width(width),
	height(height),
	pixels(nullptr)
{
	this->pixels = new Color[this->width * this->height];
}

Canvas::Canvas(Canvas&& other)
{
	// copy
	this->width = other.width;
	this->height = other.height;
	int length = this->width * this->height;
	this->pixels = new Color[length];
	std::copy(other.pixels, other.pixels + length, this->pixels);

	// delete
	other.width = 0;
	other.height = 0;
	delete[] other.pixels;
	other.pixels = nullptr;
}

Canvas::~Canvas()
{
	if (this->pixels != nullptr)
	{
		delete[] this->pixels;
		this->pixels = nullptr;
	}
}

Color Canvas::PixelAt(int x, int y) const
{
	return this->pixels[x + y * this->width];
}

void Canvas::WritePixel(int x, int y, Color const& color)
{
	if (x >= this->width || y >= this->height)
		return;
	this->pixels[x + y * this->width] = color;
}