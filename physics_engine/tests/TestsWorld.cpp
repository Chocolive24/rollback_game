#include "World.h"

#include "gtest/gtest.h"
#include "../../common/include/Metrics.h"

#include <array>

using namespace PhysicsEngine;
using namespace Math;

struct IntFixture : public ::testing::TestWithParam<int>{};

struct ArrayOfBody : public ::testing::TestWithParam<std::array<Body, 3>>{};

class TestContactListener : public ContactListener
{
public:
    bool Enter = false;
    bool Stay = false;
    bool Exit = false;

    void OnTriggerEnter(PhysicsEngine::ColliderRef colliderRefA,
                        PhysicsEngine::ColliderRef colliderRefB) noexcept override
    {
        Enter = true;
        Stay = false;
        Exit = false;
    }

    void OnTriggerStay(PhysicsEngine::ColliderRef colliderRefA,
                       PhysicsEngine::ColliderRef colliderRefB) noexcept override
    {
        Enter = false;
        Stay = true;
        Exit = false;
    }

    void OnTriggerExit(PhysicsEngine::ColliderRef colliderRefA,
                        PhysicsEngine::ColliderRef colliderRefB) noexcept override
    {
        Enter = false;
        Stay = false;
        Exit = true;
    }

    // Inherited via ContactListener
    void OnCollisionEnter(ColliderRef colliderRefA, ColliderRef colliderRefB) noexcept override
    {
    }
    void OnCollisionExit(ColliderRef colliderRefA, ColliderRef colliderRefB) noexcept override
    {
    }
};

INSTANTIATE_TEST_SUITE_P(World, IntFixture, testing::Values(
        0, 1, 24, 500, 900, -10
        ));

INSTANTIATE_TEST_SUITE_P(World, ArrayOfBody, testing::Values(
        std::array<Body, 3>{Body(), Body(Vec2F::One(), Vec2F::One(), 1.f),
                            Body(Vec2F(50.f, 32.12f), Vec2F(23.f, 45.f), 50.f)},
        std::array<Body, 3>{Body(Vec2F(23.f, -10.12f), Vec2F(-223.f, -0.f), 2.f),
                            Body(Vec2F(50.f, -9832.12f), Vec2F(2543.f, 45000.f), 50019.f),
                            Body(Vec2F(3.f, 1.12f), Vec2F(2.f, 56.f), 0.f)}
));

TEST(World, DefaultConstructor)
{
    World world;

    EXPECT_EQ(world.GetBodyCount(), 0);
}

TEST_P(IntFixture, Init)
{
    int bodyCount = GetParam();

    World world;
    world.Init(Math::Vec2F::Zero(), bodyCount);

    if (bodyCount < 0)
    {
        EXPECT_EQ(world.GetBodyCount(), 0);
        return;
    }

    EXPECT_EQ(world.GetBodyCount(), bodyCount);
}

TEST(World, CreateAndDestroyBody)
{
    World world;
    world.Init(Math::Vec2F::Zero(), 5);

    auto bodyRef = world.CreateBody();
    auto& body = world.GetBody(bodyRef);
    body = Body(Vec2F::One(), Vec2F::One(), 10.f);

    EXPECT_EQ(bodyRef.Index, 0);
    EXPECT_EQ(bodyRef.GenerationIdx, 0);
    EXPECT_TRUE(world.GetBody(bodyRef).IsValid());

    world.DestroyBody(bodyRef);

    bodyRef = world.CreateBody();
    auto& newBody = world.GetBody(bodyRef);
    newBody = Body(Vec2F::One(), Vec2F::One(), 2.f);

    EXPECT_EQ(bodyRef.Index, 0);
    EXPECT_EQ(bodyRef.GenerationIdx, 1);
    EXPECT_TRUE(world.GetBody(bodyRef).IsValid());

    auto bodyRef2 = world.CreateBody();
    auto& body2 = world.GetBody(bodyRef2);
    body2 = Body(Vec2F::One(), Vec2F::One(), 50.f);

    EXPECT_EQ(bodyRef2.Index, 1);
    EXPECT_EQ(bodyRef2.GenerationIdx, 0);
    EXPECT_TRUE(world.GetBody(bodyRef2).IsValid());

    world.DestroyBody(bodyRef2);

    Body nullBodyRef;

    EXPECT_THROW(nullBodyRef =  world.GetBody(bodyRef2), std::runtime_error);
}

TEST_P(ArrayOfBody, Update)
{
    auto bodies = GetParam();
    std::array<BodyRef, bodies.size()> bodyRefs{};

    World world;
    world.Init(Math::Vec2F::Zero(), bodies.size());

    float deltaTime = 0.1f;

    Vec2F acceleration, velocity, position;

    for (std::size_t i = 0; i < bodies.size(); i++)
    {
        BodyRef bodyRef = world.CreateBody();
        Body& body = world.GetBody(bodyRef);
        body = bodies[i];

        bodyRefs[i] = bodyRef;

        body.ApplyForce(Vec2F(1.0f, 0.0f));
        bodies[i].ApplyForce(Vec2F(1.0f, 0.0f));
    }

    world.Update(deltaTime);

    for (std::size_t i = 0; i < bodies.size(); i++)
    {
        auto b = bodies[i];

        if (!b.IsValid()) continue;

        auto bRef = bodyRefs[i];

        auto body = world.GetBody(bRef);

        acceleration = b.Forces() / b.Mass();
        velocity = b.Velocity() + acceleration * deltaTime;
        position = b.Position() + velocity * deltaTime;

        EXPECT_FLOAT_EQ(body.Position().X, position.X);
        EXPECT_FLOAT_EQ(body.Position().Y, position.Y);

        EXPECT_FLOAT_EQ(body.Velocity().X, velocity.X);
        EXPECT_FLOAT_EQ(body.Velocity().Y, velocity.Y);

        EXPECT_FLOAT_EQ(body.Forces().X, 0.f);
        EXPECT_FLOAT_EQ(body.Forces().Y, 0.f);
    }
}

TEST(World, CreateAndDestroyCircleCollider)
{
    World world;
    world.Init(Math::Vec2F::Zero(), 5);

    auto bodyRef = world.CreateBody();
    auto& body = world.GetBody(bodyRef);
    body = Body(Vec2F::One(), Vec2F::One(), 10.f);

    auto colRef = world.CreateCollider(bodyRef);
    auto& collider = world.GetCollider(colRef);

    EXPECT_EQ(colRef.Index, 0);
    EXPECT_EQ(colRef.GenerationIdx, 0);
    EXPECT_TRUE(world.GetCollider(colRef).Enabled());

    world.DestroyCollider(colRef);

    bodyRef = world.CreateBody();
    auto& newBody = world.GetBody(bodyRef);
    newBody = Body(Vec2F::One(), Vec2F::One(), 2.f);

    colRef = world.CreateCollider(bodyRef);
    auto& newCollider = world.GetCollider(colRef);

    EXPECT_EQ(colRef.Index, 0);
    EXPECT_EQ(colRef.GenerationIdx, 1);
    EXPECT_TRUE(world.GetCollider(colRef).Enabled());

    auto bodyRef2 = world.CreateBody();
    auto& body2 = world.GetBody(bodyRef2);
    body2 = Body(Vec2F::One(), Vec2F::One(), 50.f);

    auto colRef2 = world.CreateCollider(bodyRef2);
    auto& collider2 = world.GetCollider(colRef2);

    EXPECT_EQ(colRef2.Index, 1);
    EXPECT_EQ(colRef2.GenerationIdx, 0);
    EXPECT_TRUE(world.GetCollider(colRef2).Enabled());

    world.DestroyCollider(colRef2);

    Collider nullCollider;

    EXPECT_THROW(nullCollider =  world.GetCollider(colRef2), std::runtime_error);
}

TEST(World, UpdateCollisionDetectionCircle)
{
    CircleF c1(Vec2F::Zero(), 0.5f);
    CircleF c2(Vec2F(0.03f, 0.03f), 0.2f);

    World world;
    world.Init(Math::Vec2F::Zero(), 2);

    TestContactListener testContactListener;
    world.SetContactListener(&testContactListener);

    float deltaTime = 0.1f;

    auto bodyRef = world.CreateBody();
    auto& body = world.GetBody(bodyRef);
    body = Body(c1.Center(), Vec2F::Zero(), 1);

    auto c1ColRef = world.CreateCollider(bodyRef);
    auto& collider = world.GetCollider(c1ColRef);
    collider.SetIsTrigger(true);
    collider.SetShape(CircleF(Vec2F::Zero(), c1.Radius()));

    auto bodyRef2 = world.CreateBody();
    auto& body2 = world.GetBody(bodyRef2);
    body2 = Body(c2.Center(), Vec2F::Zero(), 1);

    auto c2ColRef = world.CreateCollider(bodyRef2);
    auto& collider2 = world.GetCollider(c2ColRef);
    collider2.SetIsTrigger(true);
    collider.SetShape(CircleF(Vec2F::Zero(), c2.Radius()));

    // First Update, circle collide :
    world.Update(0.1f);

    EXPECT_TRUE(testContactListener.Enter);
    EXPECT_FALSE(testContactListener.Stay);
    EXPECT_FALSE(testContactListener.Exit);

    // Second Update, circle always collide :
    world.Update(0.1f);

    EXPECT_FALSE(testContactListener.Enter);
    EXPECT_TRUE(testContactListener.Stay);
    EXPECT_FALSE(testContactListener.Exit);

    // Third Update, circle stop collide :
    world.GetBody(bodyRef).SetPosition(Math::Vec2F(10.f, 10.f));

    world.Update(0.1f);

    EXPECT_FALSE(testContactListener.Enter);
    EXPECT_FALSE(testContactListener.Stay);
    EXPECT_TRUE(testContactListener.Exit);
}

TEST(World, UpdateCollisionDetectionRect)
{
    RectangleF r1(Vec2F::Zero(), Vec2F(1.f, 1.f));
    RectangleF r2(Vec2F(0.3f, 0.3f), Vec2F(0.7f, 0.7f));

    World world;
    world.Init(Math::Vec2F::Zero(), 2);

    TestContactListener testContactListener;
    world.SetContactListener(&testContactListener);

    float deltaTime = 0.1f;

    auto bodyRef = world.CreateBody();
    auto& body = world.GetBody(bodyRef);
    body = Body(r1.Center(), Vec2F::Zero(), 1);

    auto r1ColRef = world.CreateCollider(bodyRef);
    auto& collider = world.GetCollider(r1ColRef);
    collider.SetIsTrigger(true);
    const auto halfSize1 = r1.Size() * 0.5f;
    collider.SetShape(RectangleF(Vec2F::Zero() - halfSize1, Vec2F::Zero() + halfSize1));

    auto bodyRef2 = world.CreateBody();
    auto& body2 = world.GetBody(bodyRef2);
    body2 = Body(r2.Center(), Vec2F::Zero(), 1);

    auto r2ColRef = world.CreateCollider(bodyRef2);
    auto& collider2 = world.GetCollider(r2ColRef);
    collider2.SetIsTrigger(true);
    const auto halfSize2 = r1.Size() * 0.5f;
    collider2.SetShape(RectangleF(Vec2F::Zero() - halfSize2, Vec2F::Zero() + halfSize2));

    // First Update, circle collide :
    world.Update(0.1f);

    EXPECT_TRUE(testContactListener.Enter);
    EXPECT_FALSE(testContactListener.Stay);
    EXPECT_FALSE(testContactListener.Exit);

    // Second Update, circle always collide :
    world.Update(0.1f);

    EXPECT_FALSE(testContactListener.Enter);
    EXPECT_TRUE(testContactListener.Stay);
    EXPECT_FALSE(testContactListener.Exit);

    // Third Update, circle stop collide :
    world.GetBody(bodyRef).SetPosition(Math::Vec2F(10.f, 10.f));

    world.Update(0.1f);

    EXPECT_FALSE(testContactListener.Enter);
    EXPECT_FALSE(testContactListener.Stay);
    EXPECT_TRUE(testContactListener.Exit);
}

TEST(World, UpdateCollisionDetectionCircleAndRect)
{
    CircleF c1(Vec2F::Zero(), 0.5f);
    RectangleF r2(Vec2F(0.3f, 0.3f), Vec2F(0.7f, 0.7f));

    World world;
    world.Init(Math::Vec2F::Zero(), 2);

    TestContactListener testContactListener;
    world.SetContactListener(&testContactListener);

    float deltaTime = 0.1f;

    auto bodyRef = world.CreateBody();
    auto& body = world.GetBody(bodyRef);
    body = Body(c1.Center(), Vec2F::Zero(), 1);

    auto c1ColRef = world.CreateCollider(bodyRef);
    auto& collider = world.GetCollider(c1ColRef);
    collider.SetIsTrigger(true);
    collider.SetShape(CircleF(Vec2F::Zero(), c1.Radius()));

    auto bodyRef2 = world.CreateBody();
    auto& body2 = world.GetBody(bodyRef2);
    body2 = Body(r2.Center(), Vec2F::Zero(), 1);

    auto r2ColRef = world.CreateCollider(bodyRef2);
    auto& collider2 = world.GetCollider(r2ColRef);
    collider2.SetIsTrigger(true);
    const auto halfSize = r2.Size() * 0.5f;
    collider2.SetShape(RectangleF(Vec2F::Zero() - halfSize, Vec2F::Zero() + halfSize));

    // First Update, circle collide :
    world.Update(0.1f);

    EXPECT_TRUE(testContactListener.Enter);
    EXPECT_FALSE(testContactListener.Stay);
    EXPECT_FALSE(testContactListener.Exit);

    // Second Update, circle always collide :
    world.Update(0.1f);

    EXPECT_FALSE(testContactListener.Enter);
    EXPECT_TRUE(testContactListener.Stay);
    EXPECT_FALSE(testContactListener.Exit);

    // Third Update, circle stop collide :
    world.GetBody(bodyRef).SetPosition(Math::Vec2F(10.f, 10.f));

    world.Update(0.1f);

    EXPECT_FALSE(testContactListener.Enter);
    EXPECT_FALSE(testContactListener.Stay);
    EXPECT_TRUE(testContactListener.Exit);
}

TEST(World, UpdateCollisionDetectionPolygon)
{
    std::vector<Vec2F> v1 = {
            Vec2F::Zero(),
            Vec2F(1.f, 0.f),
            Vec2F(0.5f, 0.3f),
            Vec2F(0.7f, 0.5f),
            Vec2F(0.9f, 0.7f)
    };

    std::vector<Vec2F> v2 = {
            Vec2F(0.1f, 0.1f),
            Vec2F(0.8f, 0.1f),
            Vec2F(0.4f, 0.25f),
            Vec2F(0.6f, 0.6f),
            Vec2F(0.8f, 0.5f)
    };

    PolygonF p1(v1);
    PolygonF p2(v2);

    World world;
    world.Init(Math::Vec2F::Zero(), 2);

    TestContactListener testContactListener;
    world.SetContactListener(&testContactListener);

    float deltaTime = 0.1f;

    auto bodyRef = world.CreateBody();
    auto& body = world.GetBody(bodyRef);
    body = Body(p1.Center(), Vec2F::Zero(), 1);

    auto c1ColRef = world.CreateCollider(bodyRef);
    auto& collider = world.GetCollider(c1ColRef);
    collider.SetIsTrigger(true);
    collider.SetShape(PolygonF(v1));

    auto bodyRef2 = world.CreateBody();
    auto& body2 = world.GetBody(bodyRef2);
    body2 = Body(p2.Center(), Vec2F::Zero(), 1);

    auto c2ColRef = world.CreateCollider(bodyRef2);
    auto& collider2 = world.GetCollider(c2ColRef);
    collider2.SetIsTrigger(true);
    collider2.SetShape(PolygonF(v2));

    // First Update, circle collide :
    world.Update(0.1f);

    EXPECT_TRUE(testContactListener.Enter);
    EXPECT_FALSE(testContactListener.Stay);
    EXPECT_FALSE(testContactListener.Exit);

    // Second Update, circle always collide :
    world.Update(0.1f);

    EXPECT_FALSE(testContactListener.Enter);
    EXPECT_TRUE(testContactListener.Stay);
    EXPECT_FALSE(testContactListener.Exit);

    // Third Update, circle stop collide :
    world.GetBody(bodyRef).SetPosition(Math::Vec2F(10.f, 10.f));

    world.Update(0.1f);

    EXPECT_FALSE(testContactListener.Enter);
    EXPECT_FALSE(testContactListener.Stay);
    EXPECT_TRUE(testContactListener.Exit);
}

TEST(World, UpdateCollisionDetectionPolygonAndCircle)
{
    std::vector<Vec2F> v1 = {
            Vec2F::Zero(),
            Vec2F(1.f, 0.f),
            Vec2F(0.5f, 0.3f),
            Vec2F(0.7f, 0.5f),
            Vec2F(0.9f, 0.7f)
    };

    PolygonF p1(v1);
    CircleF c2(Vec2F::Zero(), 0.5f);

    World world;
    world.Init(Math::Vec2F::Zero(), 2);

    TestContactListener testContactListener;
    world.SetContactListener(&testContactListener);

    float deltaTime = 0.1f;

    auto bodyRef = world.CreateBody();
    auto& body = world.GetBody(bodyRef);
    body = Body(Vec2F::Zero(), Vec2F::Zero(), 1);

    auto c1ColRef = world.CreateCollider(bodyRef);
    auto& collider = world.GetCollider(c1ColRef);
    collider.SetIsTrigger(true);
    collider.SetShape(p1);

    auto bodyRef2 = world.CreateBody();
    auto& body2 = world.GetBody(bodyRef2);
    body2 = Body(Vec2F::Zero(), Vec2F::Zero(), 1);

    auto c2ColRef = world.CreateCollider(bodyRef2);
    auto& collider2 = world.GetCollider(c2ColRef);
    collider2.SetIsTrigger(true);
    collider.SetShape(CircleF(Vec2F::Zero(), c2.Radius()));

    // First Update, circle collide :
    world.Update(0.1f);

    EXPECT_TRUE(testContactListener.Enter);
    EXPECT_FALSE(testContactListener.Stay);
    EXPECT_FALSE(testContactListener.Exit);

    // Second Update, circle always collide :
    world.Update(0.1f);

    EXPECT_FALSE(testContactListener.Enter);
    EXPECT_TRUE(testContactListener.Stay);
    EXPECT_FALSE(testContactListener.Exit);

    // Third Update, circle stop collide :
    world.GetBody(bodyRef2).SetPosition(Math::Vec2F(1000.f, 1000.f));

    world.Update(0.1f);

    EXPECT_FALSE(testContactListener.Enter);
    EXPECT_FALSE(testContactListener.Stay);
    EXPECT_TRUE(testContactListener.Exit);
}

TEST(World, UpdateCollisionDetectionPolygonAndRect)
{
    std::vector<Vec2F> v1 =
    {
            Vec2F::Zero(),
            Vec2F(1.f, 0.f),
            Vec2F(0.5f, 0.3f),
            Vec2F(0.7f, 0.5f),
            Vec2F(0.9f, 0.7f)
    };

    PolygonF p1(v1);
    RectangleF r2(Vec2F(0.3f, 0.3f), Vec2F(0.7f, 0.7f));

    World world;
    world.Init(Math::Vec2F::Zero(), 2);

    TestContactListener testContactListener;
    world.SetContactListener(&testContactListener);

    float deltaTime = 0.1f;

    auto bodyRef = world.CreateBody();
    auto& body = world.GetBody(bodyRef);
    body = Body(Vec2F::Zero(), Vec2F::Zero(), 1);

    auto c1ColRef = world.CreateCollider(bodyRef);
    auto& collider = world.GetCollider(c1ColRef);
    collider.SetIsTrigger(true);
    collider.SetShape(p1);

    auto bodyRef2 = world.CreateBody();
    auto& body2 = world.GetBody(bodyRef2);
    body2 = Body(Vec2F::Zero(), Vec2F::Zero(), 1);

    auto c2ColRef = world.CreateCollider(bodyRef2);
    auto& collider2 = world.GetCollider(c2ColRef);
    collider2.SetIsTrigger(true);
    collider2.SetShape(r2);

    // First Update, circle collide :
    world.Update(deltaTime);

    EXPECT_TRUE(testContactListener.Enter);
    EXPECT_FALSE(testContactListener.Stay);
    EXPECT_FALSE(testContactListener.Exit);

    // Second Update, circle always collide :
    world.Update(deltaTime);

    EXPECT_FALSE(testContactListener.Enter);
    EXPECT_TRUE(testContactListener.Stay);
    EXPECT_FALSE(testContactListener.Exit);

    // Third Update, circle stop collide :
    world.GetBody(bodyRef).SetPosition(Math::Vec2F(1000.f, 1000.f));

    world.Update(0.1f);

    EXPECT_FALSE(testContactListener.Enter);
    EXPECT_FALSE(testContactListener.Stay);
    EXPECT_TRUE(testContactListener.Exit);
}