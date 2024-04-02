#include "Metrics.h"

#include "gtest/gtest.h"

using namespace Math;

struct FloatFixture : public ::testing::TestWithParam<float>{};

struct Vec2FloatFixture : public ::testing::TestWithParam<Vec2F>{};

INSTANTIATE_TEST_SUITE_P(Metrics, FloatFixture, testing::Values(
        0.f, 1.f, 5.f, 54.556f, 99999.11111f
));

INSTANTIATE_TEST_SUITE_P(Metrics, Vec2FloatFixture, testing::Values(
        Vec2F::Zero(), Vec2F::One(), Vec2F(-34.55f, -0.00001f), Vec2F(99999.543f, 234.6789999f)
));

TEST_P(FloatFixture, PixelsToMeters)
{
    auto pixels = GetParam();
    
    auto meters = Metrics::PixelsToMeters(pixels);

    EXPECT_FLOAT_EQ(meters, pixels * Metrics::PixelsToMetersRatio);
}

TEST_P(FloatFixture, MetersToPixels)
{
    auto meters = GetParam();

    auto pixels = Metrics::MetersToPixels(meters);

    EXPECT_FLOAT_EQ(pixels, meters * Metrics::MetersToPixelsRatio);
}

TEST_P(Vec2FloatFixture, PixelsToMeters)
{
    auto pixelsPos = GetParam();

    auto metersPos = Metrics::PixelsToMeters(pixelsPos);

    EXPECT_FLOAT_EQ(metersPos.X, pixelsPos.X * Metrics::PixelsToMetersRatio);
    EXPECT_FLOAT_EQ(metersPos.Y, pixelsPos.Y * Metrics::PixelsToMetersRatio * -1.f);
}

TEST_P(Vec2FloatFixture, MetersToPixels)
{
    auto metersPos = GetParam();

    auto pixelsPos = Metrics::MetersToPixels(metersPos);

    EXPECT_FLOAT_EQ(pixelsPos.X, metersPos.X * Metrics::MetersToPixelsRatio);
    EXPECT_FLOAT_EQ(pixelsPos.Y, metersPos.Y * Metrics::MetersToPixelsRatio * -1.f);
}