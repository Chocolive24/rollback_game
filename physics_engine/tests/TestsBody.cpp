#include "gtest/gtest.h"
#include "Body.h"

#include <array>

using namespace PhysicsEngine;
using namespace Math;

struct BodyAttributesFixture : public ::testing::TestWithParam<std::tuple<Vec2F, Vec2F, float, Vec2F>>{};

struct Vec2FloatFixture : public ::testing::TestWithParam<Vec2F>{};

struct FloatFixture : public ::testing::TestWithParam<float>{};

struct ArrayOfVec2FloatFixture : public ::testing::TestWithParam<std::array<Vec2F, 4>>{};

INSTANTIATE_TEST_SUITE_P(Body, BodyAttributesFixture, testing::Values(
        std::tuple{Vec2F::Zero(), Vec2F::Zero(), 0.f, Vec2F::Zero()},
        std::tuple{Vec2F(1000.24567f, -0.0000001), Vec2F(-56.3333f, 123.321f), 1987.f,
                   Vec2F(1234.45f, -98)}
        ));

INSTANTIATE_TEST_SUITE_P(Body, Vec2FloatFixture, testing::Values(
        Vec2F::Zero(),
        Vec2F::One(),
        Vec2F(-1024354.f, -0.000001f),
        Vec2F(192345.555555f, -983.4566789f)
        ));

INSTANTIATE_TEST_SUITE_P(Body, FloatFixture, testing::Values(
        0.f,
        1.f,
        -1024354.f,
        192345.555555f,
        -0.000001f
));

INSTANTIATE_TEST_SUITE_P(Body, ArrayOfVec2FloatFixture, testing::Values(
        std::array<Vec2F, 4>{Vec2F::Zero(), Vec2F::One(),
        Vec2F(-1024354.f, -0.000001f),
        Vec2F(192345.555555f, -983.4566789f)}
));

TEST(Body, DefaultConstructor)
{
    Body b;

    EXPECT_FLOAT_EQ(b.Position().X, 0.f);
    EXPECT_FLOAT_EQ(b.Position().Y, 0.f);

    EXPECT_FLOAT_EQ(b.Velocity().X, 0.f);
    EXPECT_FLOAT_EQ(b.Velocity().Y, 0.f);

    EXPECT_FLOAT_EQ(b.Mass(), -1.f);

    EXPECT_FLOAT_EQ(b.Forces().X, 0.f);
    EXPECT_FLOAT_EQ(b.Forces().Y, 0.f);
}

TEST_P(BodyAttributesFixture, Constructor)
{
    auto [pos, vel, mass, forces] = GetParam();

    Body b(pos, vel, mass);

    EXPECT_FLOAT_EQ(b.Position().X, pos.X);
    EXPECT_FLOAT_EQ(b.Position().Y, pos.Y);

    EXPECT_FLOAT_EQ(b.Velocity().X, vel.X);
    EXPECT_FLOAT_EQ(b.Velocity().Y, vel.Y);

    EXPECT_FLOAT_EQ(b.Mass(), mass);

    EXPECT_FLOAT_EQ(b.Forces().X, 0.f);
    EXPECT_FLOAT_EQ(b.Forces().Y, 0.f);
}

TEST_P(Vec2FloatFixture, GetAndSetPosition)
{
    auto v = GetParam();

    Body b(Vec2F(v.X, v.Y), Vec2F::Zero(), 0.f);

    auto bPos = b.Position();
    b.SetPosition(v * 3);
    auto newBPos = b.Position();


    EXPECT_FLOAT_EQ(bPos.X, v.X);
    EXPECT_FLOAT_EQ(bPos.Y, v.Y);

    EXPECT_FLOAT_EQ(newBPos.X, v.X * 3);
    EXPECT_FLOAT_EQ(newBPos.Y, v.Y * 3);
}

TEST_P(Vec2FloatFixture, GetAndSetVelocity)
{
    auto v = GetParam();

    Body b(Vec2F::Zero(), Vec2F(v.X, v.Y), 0.f);

    auto bVel = b.Velocity();
    b.SetVelocity(v * 3);
    auto newBVel = b.Velocity();

    EXPECT_FLOAT_EQ(bVel.X, v.X);
    EXPECT_FLOAT_EQ(bVel.Y, v.Y);

    EXPECT_FLOAT_EQ(newBVel.X, v.X * 3);
    EXPECT_FLOAT_EQ(newBVel.Y, v.Y * 3);
}

TEST_P(FloatFixture, GetAndSetMass)
{
    auto f = GetParam();

    Body b(Vec2F::Zero(), Vec2F::Zero(), f);

    auto bMass = b.Mass();
    b.SetMass(f * 3);
    auto newBMass = b.Mass();

    EXPECT_FLOAT_EQ(bMass, f);
    EXPECT_FLOAT_EQ(newBMass, f * 3);
}

TEST_P(Vec2FloatFixture, GetAndSetForces)
{
    auto v = GetParam();

    Body b(Vec2F::Zero(), Vec2F::Zero(), 0.f);

    auto bForces = b.Forces();
    b.ResetForces();
    auto newBForces = b.Forces();

    EXPECT_FLOAT_EQ(bForces.X, 0.f);
    EXPECT_FLOAT_EQ(bForces.Y, 0.f);

    EXPECT_FLOAT_EQ(newBForces.X, 0);
    EXPECT_FLOAT_EQ(newBForces.Y, 0);
}

TEST_P(ArrayOfVec2FloatFixture, AddForce)
{
    auto forces = GetParam();

    Body b;

    Vec2F sumOfForces = Vec2F::Zero();

    for (auto& force : forces)
    {
        b.ApplyForce(force);
        sumOfForces += force;
    }

    EXPECT_FLOAT_EQ(b.Forces().X, sumOfForces.X);
    EXPECT_FLOAT_EQ(b.Forces().Y, sumOfForces.Y);
}

TEST_P(FloatFixture, IsValid)
{
    auto mass = GetParam();

    Body b(Vec2F::Zero(), Vec2F::Zero(), mass);

    EXPECT_EQ(b.IsValid(), mass > 0);
}