#include "Collider.h"

#include "gtest/gtest.h"

using namespace Math;
using namespace PhysicsEngine;

struct ColliderAttributesFixture : public ::testing::TestWithParam<std::tuple<float, float, bool>>{};

struct ColliderShapeFixture : public ::testing::TestWithParam<std::tuple<CircleF, RectangleF, PolygonF>> {};

struct RefFixture : public ::testing::TestWithParam<std::pair<std::size_t, std::size_t>> {};

struct FloatFixture : public ::testing::TestWithParam<float> {};

struct PairOfRefFixture : public ::testing::TestWithParam<std::pair<ColliderRef, ColliderRef>> {};

struct ColliderPairFixture : public ::testing::TestWithParam<ColliderPair> {};

struct PairOfColliderPairFixture : public ::testing::TestWithParam<std::pair<ColliderPair, ColliderPair>> {};

INSTANTIATE_TEST_SUITE_P(Collider, ColliderAttributesFixture, testing::Values(
        std::tuple{0.f, 0.f, false},
        std::tuple{ 5.f, 10.f, true}
        ));

INSTANTIATE_TEST_SUITE_P(Collider, ColliderShapeFixture, testing::Values(
        std::tuple{
            CircleF(Vec2F::Zero(), 0.f),
            RectangleF(Vec2F::Zero(), Vec2F::Zero()),
            PolygonF({Vec2F::Zero(), Vec2F::Zero(), Vec2F::Zero()})}
));

INSTANTIATE_TEST_SUITE_P(Collider, RefFixture, testing::Values(
        std::pair{0, 0},
        std::pair{ 5, 10},
        std::pair{ 3, 25}
));

INSTANTIATE_TEST_SUITE_P(Collider, FloatFixture, testing::Values(
        0.f, 1.f, 2.25f, 5.000001f, -10.987654321f, 1000.99999f
));

TEST(Collider, DefaultConstructor)
{
    Collider collider;

    EXPECT_FLOAT_EQ(collider.Restitution(), -1.f);
    EXPECT_FLOAT_EQ(collider.Friction(), -1.f);

    EXPECT_FALSE(collider.IsTrigger());
}

INSTANTIATE_TEST_SUITE_P(Collider, PairOfRefFixture, testing::Values(
        std::pair{ ColliderRef{0, 0}, ColliderRef{0, 0}},
        std::pair{ ColliderRef{1, 5}, ColliderRef{2, 3}},
        std::pair{ ColliderRef{1, 1}, ColliderRef{1, 1}},
        std::pair{ ColliderRef{50, 34}, ColliderRef{98, 100}}
));

INSTANTIATE_TEST_SUITE_P(Collider, ColliderPairFixture, testing::Values(
        ColliderPair{
                ColliderRef{0, 0},
                ColliderRef{0, 0}},
       ColliderPair{
                ColliderRef{10, 20},
                ColliderRef{80, 30}}

));

INSTANTIATE_TEST_SUITE_P(Collider, PairOfColliderPairFixture, testing::Values(
        std::pair{ ColliderPair{
            ColliderRef{0, 0},
            ColliderRef{0, 0}},
                   ColliderPair{
            ColliderRef{0, 0},
            ColliderRef{0, 0}}},
        std::pair{ ColliderPair{
                ColliderRef{10, 20},
                ColliderRef{80, 30}},
                   ColliderPair{
                           ColliderRef{9, 4},
                           ColliderRef{5, 1}}}
));

TEST_P(ColliderAttributesFixture, Constructor)
{
    auto [rest, frict, isTrigger] = GetParam();

    Collider collider(rest, frict, isTrigger);

    EXPECT_FLOAT_EQ(collider.Restitution(), rest);
    EXPECT_FLOAT_EQ(collider.Friction(), frict);

    EXPECT_EQ(collider.IsTrigger(), isTrigger);
}

TEST_P(ColliderShapeFixture, GetAndSetShape)
{
    auto [circle, rect, poly] = GetParam();

    Collider col;

    col.SetShape(circle);

    switch (col.Shape().index())
    {
        case static_cast<int>(ShapeType::Circle):
        {
            EXPECT_EQ(std::get<CircleF>(col.Shape()).Center(), circle.Center());
            break;
        }
    }
}

TEST_P(RefFixture, GetAndSetBodyRef)
{
    auto [idx, genIdx] = GetParam();
    Collider col;

    col.SetBodyRef(BodyRef{idx, genIdx});

    EXPECT_EQ(col.GetBodyRef().Index, idx);
    EXPECT_EQ(col.GetBodyRef().GenerationIdx, genIdx);
}

TEST_P(FloatFixture, Restitution)
{
    auto rest = GetParam();

    Collider collider;
    EXPECT_FLOAT_EQ(collider.Restitution(), -1.f);

    collider.SetRestitution(rest);

    EXPECT_FLOAT_EQ(collider.Restitution(), rest);
}

TEST_P(FloatFixture, Friction)
{
    auto frict = GetParam();

    Collider collider;
    EXPECT_FLOAT_EQ(collider.Friction(), -1.f);

    collider.SetFriction(frict);

    EXPECT_FLOAT_EQ(collider.Friction(), frict);
}

TEST(Collider, IsTrigger)
{
    Collider collider;

    EXPECT_FALSE(collider.IsTrigger());

    collider.SetIsTrigger(true);

    EXPECT_TRUE(collider.IsTrigger());
}

TEST(ColliderPair, ColliderPairDefaultConstructor)
{
    ColliderPair colliderPair{};

    EXPECT_EQ(colliderPair.ColliderA, ColliderRef());
    EXPECT_EQ(colliderPair.ColliderB, ColliderRef());
}

TEST_P(PairOfRefFixture, ColliderPairConstructor)
{
    auto [colliderRef1, colliderRef2] = GetParam();

    ColliderPair colliderPair{ colliderRef1, colliderRef2 };

    EXPECT_EQ(colliderPair.ColliderA, colliderRef1);
    EXPECT_EQ(colliderPair.ColliderB, colliderRef2);
}

TEST_P(PairOfColliderPairFixture, IsEqualOperator)
{
    auto [colPair1, colPair2] = GetParam();

    EXPECT_EQ(colPair1 == colPair2,
              colPair1.ColliderA == colPair2.ColliderA && colPair1.ColliderB == colPair2.ColliderB
              || colPair1.ColliderA == colPair2.ColliderB && colPair1.ColliderB == colPair2.ColliderA);
}

TEST_P(PairOfColliderPairFixture, IsLowerOperator)
{
    auto [colPair1, colPair2] = GetParam();

    bool isLower = (colPair1.ColliderA < colPair2.ColliderA)
                   || (colPair1.ColliderA == colPair2.ColliderA && colPair1.ColliderB < colPair2.ColliderB);

    EXPECT_EQ(colPair1 < colPair2, isLower);
}

TEST_P(ColliderPairFixture, HashOperator)
{
    auto colPair = GetParam();

    ColliderHash colliderHash;

    std::size_t h = colliderHash(colPair);

    const std::size_t hash1 = std::hash<int>{}(static_cast<int>(colPair.ColliderA.Index));
    const std::size_t hash2 = std::hash<int>{}(static_cast<int>(colPair.ColliderB.Index));

    const auto hashExpected = hash1 + hash2;

    EXPECT_EQ(h, hashExpected);
}