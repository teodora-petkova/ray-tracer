#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)
#include "source\canvas.h"

TEST(CanvasTests, CorrectlyInitializeCanvas) {
	Canvas canvas = Canvas(20, 10);
	EXPECT_EQ(canvas.getWidth(), 20);
	EXPECT_EQ(canvas.getHeight(), 10);
	EXPECT_EQ(canvas.PixelAt(0, 0), Color(0, 0, 0));

	for (int i = 0; i < canvas.getWidth(); i++)
	{
		for (int j = 0; j < canvas.getHeight(); j++)
		{
			canvas.WritePixel(i, j, Color(0.1f, 0.89f, 0.01f));
			EXPECT_EQ(canvas.PixelAt(i, j), Color(0.1f, 0.89f, 0.01f));
		}
	}
}

TEST(CanvasTests, CorrectlyWriteAndExtractPixelsFromCanvas) {
	Canvas canvas = Canvas(10, 5);

	for (int i = 0; i < canvas.getWidth(); i++)
	{
		for (int j = 0; j < canvas.getHeight(); j++)
		{
			canvas.WritePixel(i, j, Color(0.1f, 0.89f, 0.01f));
			EXPECT_EQ(canvas.PixelAt(i, j), Color(0.1f, 0.89f, 0.01f));
		}
	}
}
