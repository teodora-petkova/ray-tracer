#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)

#include "source\sphere.h"
#include "source\transformations.h"
#include "source\stripepattern.h"
#include "source\gradient.h"
#include "source\ringpattern.h"
#include "source\checkerpattern.h"


class PatternTests : public ::testing::Test
{
protected:
	PatternPtr white;
	PatternPtr black;
	Matrix<4, 4> identity;

	PatternTests() {
	}

	virtual ~PatternTests() {
	}

protected:
	void SetUp() override {

		this->white = std::make_shared<FlatColor>(Color::White());
		this->black = std::make_shared<FlatColor>(Color::Black());
		this->identity = Matrix<4, 4>::IdentityMatrix();
	}
};

TEST_F(PatternTests, Creating_a_stripe_pattern)
{
	StripePattern p = StripePattern(white, black, identity);
	EXPECT_EQ(p.getPattern1()->getColorAt(Tuple::Point(0, 0, 0)), Color::White());
	EXPECT_EQ(p.getPattern2()->getColorAt(Tuple::Point(0, 0, 0)), Color::Black());
}

TEST_F(PatternTests, A_stripe_pattern_is_constant_in_y)
{
	StripePattern p = StripePattern();
	EXPECT_EQ(p.getColorAt(Tuple::Point(0, 0, 0)), Color::White());
	EXPECT_EQ(p.getColorAt(Tuple::Point(0, 1, 0)), Color::White());
	EXPECT_EQ(p.getColorAt(Tuple::Point(0, 2, 0)), Color::White());
}

TEST_F(PatternTests, A_stripe_pattern_is_constant_in_z)
{
	StripePattern p = StripePattern();
	EXPECT_EQ(p.getColorAt(Tuple::Point(0, 0, 0)), Color::White());
	EXPECT_EQ(p.getColorAt(Tuple::Point(0, 0, 1)), Color::White());
	EXPECT_EQ(p.getColorAt(Tuple::Point(0, 0, 2)), Color::White());
}

TEST_F(PatternTests, A_stripe_pattern_alternates_in_x)
{
	StripePattern p = StripePattern();
	EXPECT_EQ(p.getColorAt(Tuple::Point(0.f, 0.f, 0.f)), Color::White());
	EXPECT_EQ(p.getColorAt(Tuple::Point(0.9f, 0.f, 0.f)), Color::White());
	EXPECT_EQ(p.getColorAt(Tuple::Point(1.f, 0.f, 0.f)), Color::Black());
	EXPECT_EQ(p.getColorAt(Tuple::Point(-0.1f, 0.f, 0.f)), Color::Black());
	EXPECT_EQ(p.getColorAt(Tuple::Point(-1.f, 0.f, 0.f)), Color::Black());
	EXPECT_EQ(p.getColorAt(Tuple::Point(-1.1f, 0.f, 0.f)), Color::White());
}

TEST_F(PatternTests, Stripes_with_an_object_transformation)
{
	MaterialPtr material = std::make_shared<Material>(
		std::make_shared<StripePattern>(),
		1, 0, 0, 0, 0);

	ObjectPtr sphere = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		material, Transformations::Scaling(2, 2, 2));

	StripePattern p = StripePattern();
	EXPECT_EQ(p.getColorAtObject(sphere, Tuple::Point(1.5, 0, 0)), Color::White());
}

TEST_F(PatternTests, Stripes_with_a_pattern_transformation)
{
	MaterialPtr material = std::make_shared<Material>(
		std::make_shared<StripePattern>(),
		1, 0, 0, 0, 0);

	ObjectPtr sphere = std::make_shared<Sphere>();

	StripePattern p = StripePattern(white, black,
		Transformations::Scaling(2, 2, 2));

	EXPECT_EQ(p.getColorAtObject(sphere, Tuple::Point(1.5, 0, 0)), Color::White());
	EXPECT_EQ(p.getColorAtObject(sphere, Tuple::Point(2.5, 0, 0)), Color::Black());
}

TEST_F(PatternTests, Stripes_with_a_pattern_transformation2)
{
	MaterialPtr material = std::make_shared<Material>(
		std::make_shared<StripePattern>(),
		1, 0, 0, 0, 0);

	ObjectPtr sphere = std::make_shared<Sphere>();

	StripePattern p = StripePattern(white, black,
		Transformations::Scaling(0.1f, 0.1f, 0.1f));

	EXPECT_EQ(p.getColorAtObject(sphere, Tuple::Point(0.f, 0.f, 0.f)), Color::White());
	EXPECT_EQ(p.getColorAtObject(sphere, Tuple::Point(0.09f, 0.f, 0.f)), Color::White());
	EXPECT_EQ(p.getColorAtObject(sphere, Tuple::Point(0.1f, 0.f, 0.f)), Color::Black());
	EXPECT_EQ(p.getColorAtObject(sphere, Tuple::Point(0.19f, 0.f, 0.f)), Color::Black());
}

TEST_F(PatternTests, Stripes_with_both_an_object_and_pattern_transformation)
{
	MaterialPtr material = std::make_shared<Material>(
		std::make_shared<StripePattern>(),
		1, 0, 0, 0, 0);

	ObjectPtr sphere = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		material, Transformations::Scaling(2, 2, 2));

	StripePattern p = StripePattern(white, black,
		Transformations::Translation(0.5, 0, 0));

	EXPECT_EQ(p.getColorAtObject(sphere, Tuple::Point(2.5, 0, 0)), Color::White());
}

TEST_F(PatternTests, A_gradient_linearly_interpolates_between_colors)
{
	Gradient gradient = Gradient(white, black, identity);
	EXPECT_EQ(gradient.getColorAt(Tuple::Point(0.f, 0.f, 0.f)), Color::White());
	EXPECT_EQ(gradient.getColorAt(Tuple::Point(0.25f, 0.f, 0.f)), Color(0.75, 0.75, 0.75));
	EXPECT_EQ(gradient.getColorAt(Tuple::Point(0.5f, 0.f, 0.f)), Color(0.5, 0.5, 0.5));
	EXPECT_EQ(gradient.getColorAt(Tuple::Point(0.75f, 0.f, 0.f)), Color(0.25, 0.25, 0.25));
}

TEST_F(PatternTests, A_ring_should_extend_on_both_x_and_z)
{
	RingPattern ringPattern = RingPattern(white, black, identity);
	EXPECT_EQ(ringPattern.getColorAt(Tuple::Point(0, 0, 0)), Color::White());
	EXPECT_EQ(ringPattern.getColorAt(Tuple::Point(1, 0, 0)), Color::Black());
	EXPECT_EQ(ringPattern.getColorAt(Tuple::Point(0, 0, 1)), Color::Black());
	// 0.708 = just slightly more than √2 / 2
	EXPECT_EQ(ringPattern.getColorAt(Tuple::Point(0.708f, 0.f, 0.708f)), Color::Black());
}

TEST_F(PatternTests, Checkers_should_repeat_in_x)
{
	CheckerPattern checkers = CheckerPattern(white, black, identity);
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0.f, 0.f, 0.f)), Color::White());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0.99f, 0.f, 0.f)), Color::White());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(1.01f, 0.f, 0.f)), Color::Black());
}

TEST_F(PatternTests, Checkers_should_repeat_in_y)
{
	CheckerPattern checkers = CheckerPattern(white, black, identity);
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0.f, 0.f, 0.f)), Color::White());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0.f, 0.99f, 0.f)), Color::White());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0.f, 1.01f, 0.f)), Color::Black());
}

TEST_F(PatternTests, Checkers_should_repeat_in_z)
{
	CheckerPattern checkers = CheckerPattern(white, black, identity);
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0.f, 0.f, 0.f)), Color::White());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0.f, 0.f, 0.99f)), Color::White());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0.f, 0.f, 1.01f)), Color::Black());
}

TEST_F(PatternTests, Checkers_with_nested_stripe_patterns)
{
	PatternPtr pattern1 = std::make_shared<StripePattern>(white, black,
		Transformations::Scaling(0.1, 0.1, 0.1));
	PatternPtr pattern2 = std::make_shared<FlatColor>(Color::White());

	CheckerPattern checkers = CheckerPattern(pattern1, pattern2, identity);

	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0.f, 0.f, 0.f)), Color::White());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0.09f, 0.f, 0.f)), Color::White());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0.1f, 0.f, 0.f)), Color::Black());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(0.19f, 0.f, 0.f)), Color::Black());
	EXPECT_EQ(checkers.getColorAt(Tuple::Point(1.01f, 0.f, 0.f)), Color::White());
}