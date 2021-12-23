#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)

#include "source\sphere.h"
#include "source\transformations.h"
#include "source\stripepattern.h"
#include "source\gradient.h"
#include "source\ringpattern.h"
#include "source\checkerpattern.h"

TEST(PatternTests, Creating_a_stripe_pattern)
{
	Color white = Color::White();
	Color black = Color::Black();
	Matrix<4, 4> identity = Matrix<4, 4> ::IdentityMatrix();
	StripePattern p = StripePattern(white, black, identity);
	EXPECT_EQ(p.getColor1(), Color::White());
	EXPECT_EQ(p.getColor2(), Color::Black());
}

TEST(PatternTests, A_stripe_pattern_is_constant_in_y)
{
	StripePattern p = StripePattern();
	EXPECT_EQ(p.getColorAt(Tuple::Point(0, 0, 0)), Color::White());
	EXPECT_EQ(p.getColorAt(Tuple::Point(0, 1, 0)), Color::White());
	EXPECT_EQ(p.getColorAt(Tuple::Point(0, 2, 0)), Color::White());
}

TEST(PatternTests, A_stripe_pattern_is_constant_in_z)
{
	StripePattern p = StripePattern();
	EXPECT_EQ(p.getColorAt(Tuple::Point(0, 0, 0)), Color::White());
	EXPECT_EQ(p.getColorAt(Tuple::Point(0, 0, 1)), Color::White());
	EXPECT_EQ(p.getColorAt(Tuple::Point(0, 0, 2)), Color::White());
}

TEST(PatternTests, A_stripe_pattern_alternates_in_x)
{
	StripePattern p = StripePattern();
	EXPECT_EQ(p.getColorAt(Tuple::Point(0, 0, 0)), Color::White());
	EXPECT_EQ(p.getColorAt(Tuple::Point(0.9, 0, 0)), Color::White());
	EXPECT_EQ(p.getColorAt(Tuple::Point(1, 0, 0)), Color::Black());
	EXPECT_EQ(p.getColorAt(Tuple::Point(-0.1, 0, 0)), Color::Black());
	EXPECT_EQ(p.getColorAt(Tuple::Point(-1, 0, 0)), Color::Black());
	EXPECT_EQ(p.getColorAt(Tuple::Point(-1.1, 0, 0)), Color::White());
}

TEST(PatternTests, Stripes_with_an_object_transformation)
{
	MaterialPtr material = std::make_shared<Material>(
		std::make_shared<StripePattern>(),
		1, 0, 0, 0);

	ObjectPtr sphere = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		material, Transformations::Scaling(2, 2, 2));

	StripePattern p = StripePattern();
	EXPECT_EQ(p.getColorAtObject(sphere, Tuple::Point(1.5, 0, 0)), Color::White());
}

TEST(PatternTests, Stripes_with_a_pattern_transformation)
{
	MaterialPtr material = std::make_shared<Material>(std::make_shared<StripePattern>(),
		1, 0, 0, 0);

	ObjectPtr sphere = std::make_shared<Sphere>();

	StripePattern p = StripePattern(Color::White(), Color::Black(),
		Transformations::Scaling(2, 2, 2));

	EXPECT_EQ(p.getColorAtObject(sphere, Tuple::Point(1.5, 0, 0)), Color::White());
}

TEST(PatternTests, Stripes_with_both_an_object_and_pattern_transformation)
{
	MaterialPtr material = std::make_shared<Material>(
		std::make_shared<StripePattern>(),
		1, 0, 0, 0);

	ObjectPtr sphere = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		material, Transformations::Scaling(2, 2, 2));

	StripePattern p = StripePattern(Color::White(), Color::Black(),
		Transformations::Translation(0.5, 0, 0));

	EXPECT_EQ(p.getColorAtObject(sphere, Tuple::Point(2.5, 0, 0)), Color::White());
}

TEST(PatternTests, A_gradient_linearly_interpolates_between_colors)
{
	Gradient gradient = Gradient(Color::White(), Color::Black(),
		Matrix<4, 4>::IdentityMatrix());
	EXPECT_EQ(gradient.getColorAt(Tuple::Point(0, 0, 0)), Color::White());
	EXPECT_EQ(gradient.getColorAt(Tuple::Point(0.25, 0, 0)), Color(0.75, 0.75, 0.75));
	EXPECT_EQ(gradient.getColorAt(Tuple::Point(0.5, 0, 0)), Color(0.5, 0.5, 0.5));
	EXPECT_EQ(gradient.getColorAt(Tuple::Point(0.75, 0, 0)), Color(0.25, 0.25, 0.25));
}

TEST(PatternTests, A_ring_should_extend_on_both_x_and_z)
{
	RingPattern ringPattern = RingPattern(Color::White(), Color::Black(),
		Matrix<4, 4>::IdentityMatrix());
	EXPECT_EQ(ringPattern.getColorAt(Tuple::Point(0, 0, 0)), Color::White());
	EXPECT_EQ(ringPattern.getColorAt(Tuple::Point(1, 0, 0)), Color::Black());
	EXPECT_EQ(ringPattern.getColorAt(Tuple::Point(0, 0, 1)), Color::Black());
	// 0.708 = just slightly more than √2 / 2
	EXPECT_EQ(ringPattern.getColorAt(Tuple::Point(0.708, 0, 0.708)), Color::Black());
}

TEST(PatternTests, Checkers_should_repeat_in_x)
{
	CheckerPattern checkers = CheckerPattern(Color::White(), Color::Black(),
		Matrix<4, 4>::IdentityMatrix());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0, 0, 0)), Color::White());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0.99, 0, 0)), Color::White());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(1.01, 0, 0)), Color::Black());
}

TEST(PatternTests, Checkers_should_repeat_in_y)
{
	CheckerPattern checkers = CheckerPattern(Color::White(), Color::Black(),
		Matrix<4, 4>::IdentityMatrix());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0, 0, 0)), Color::White());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0, 0.99, 0)), Color::White());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0, 1.01, 0)), Color::Black());
}

TEST(PatternTests, Checkers_should_repeat_in_z)
{
	CheckerPattern checkers = CheckerPattern(Color::White(), Color::Black(),
		Matrix<4, 4>::IdentityMatrix());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0, 0, 0)), Color::White());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0, 0, 0.99)), Color::White());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0, 0, 1.01)), Color::Black());
}