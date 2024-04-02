#include "ContactSolver.h"
#include "World.h"

#include <gtest/gtest.h>

using namespace Math;
using namespace PhysicsEngine;

struct CollidingBodies
{
	std::variant<CircleF, RectangleF, PolygonF> Shape{CircleF(0.f)};
	Vec2F Position;
	Vec2F Velocity;
	float Mass;
	float Restitution;
	BodyType BodyTypee;

	CollidingBodies(std::variant<CircleF, RectangleF, PolygonF> shape,
		Vec2F position,
		Vec2F velocity,
		float mass,
		float restitution,
		BodyType bodyType = BodyType::Dynamic)
	{
		Shape = shape;
		Position = position;
		Velocity = velocity;
		Mass = mass;
		Restitution = restitution;
		BodyTypee = bodyType;
	}
};

struct PairOfCollidingBodies : public ::testing::TestWithParam<std::pair<CollidingBodies, CollidingBodies>> {};

INSTANTIATE_TEST_SUITE_P(ContactSolver, PairOfCollidingBodies, testing::Values(
	std::pair<CollidingBodies, CollidingBodies>{
	CollidingBodies{ CircleF(0.5f), Vec2F::Zero(), Vec2F(1, 0), 1.f, 1.f },
	CollidingBodies{ CircleF(0.5f), Vec2F(0.75, 0), Vec2F(-1, 0), 1.f, 1.f } },

	std::pair<CollidingBodies, CollidingBodies>{
	CollidingBodies{ CircleF(0.5f), Vec2F::Zero(), Vec2F(1, 0), 1.f, 1.f, BodyType::Static },
		CollidingBodies{ CircleF(0.5f), Vec2F(0.75, 0), Vec2F(-1, 0), 1.f, 1.f } },

	std::pair<CollidingBodies, CollidingBodies>{
	CollidingBodies{ RectangleF(Vec2F(-0.5f, -0.5f), Vec2F(0.5f, 0.5f)), Vec2F::Zero(), Vec2F(1, 0), 1.f, 1.f},
	CollidingBodies{ RectangleF(Vec2F(-0.5f, -0.55f), Vec2F(0.5f, 0.5f)), Vec2F(0.75, 0), Vec2F(-1, 0), 1.f, 1.f } },

	std::pair<CollidingBodies, CollidingBodies>{
	CollidingBodies{ CircleF(0.5f), Vec2F::Zero(), Vec2F(1, 0), 1.f, 1.f },
	CollidingBodies{ RectangleF(Vec2F(-0.5f, -0.55f), Vec2F(0.5f, 0.5f)), Vec2F(0.75, 0), Vec2F(-1, 0), 1.f, 1.f } },
	
	std::pair<CollidingBodies, CollidingBodies>{
	CollidingBodies{ RectangleF(Vec2F(-0.5f, -0.55f), Vec2F(0.5f, 0.5f)), Vec2F(0.75, 0), Vec2F(-1, 0), 1.f, 1.f },
	CollidingBodies{ CircleF(0.5f), Vec2F::Zero(), Vec2F(1, 0), 1.f, 1.f } }
));

void SetBodyAndColliderValues(const CollidingBodies& cb1,
	const CollidingBodies& cb2,
	Body& body1,
	Body& body2, 
	Collider& collider1, 
	Collider& collider2,
	ContactSolver& testedContactSolver,
	ContactSolver& expectedContactSolver)
{
	body1.SetPosition(cb1.Position);
	body1.SetVelocity(cb1.Velocity);
	body1.SetMass(cb1.Mass);

	collider1.SetRestitution(cb1.Restitution);

	switch (static_cast<ShapeType>(cb1.Shape.index()))
	{
	case ShapeType::Circle:
		collider1.SetShape(std::get<CircleF>(cb1.Shape));
		break;
	case ShapeType::Rectangle:
		collider1.SetShape(std::get<RectangleF>(cb1.Shape));
		break;
	case ShapeType::Polygon:
		collider1.SetShape(std::get<PolygonF>(cb1.Shape));
		break;
	}

	body2.SetPosition(cb2.Position);
	body2.SetVelocity(cb2.Velocity);
	body2.SetMass(cb2.Mass);

	collider2.SetRestitution(cb2.Restitution);

	switch (static_cast<ShapeType>(cb2.Shape.index()))
	{
	case ShapeType::Circle:
		collider2.SetShape(std::get<CircleF>(cb2.Shape));
		break;
	case ShapeType::Rectangle:
		collider2.SetShape(std::get<RectangleF>(cb2.Shape));
		break;
	case ShapeType::Polygon:
		collider2.SetShape(std::get<PolygonF>(cb2.Shape));
		break;
	}

	testedContactSolver.BodyA = &body1;
	testedContactSolver.BodyB = &body2;
	testedContactSolver.ColliderA = &collider1;
	testedContactSolver.ColliderB = &collider2;

	expectedContactSolver.BodyA = &body1;
	expectedContactSolver.BodyB = &body2;
	expectedContactSolver.ColliderA = &collider1;
	expectedContactSolver.ColliderB = &collider2;
}

void CalculateTestContactProperties(Body& body1, 
	Body& body2, 
	Collider& collider1,
	Collider& collider2,
	ContactSolver& expectedContactSolver)
{
	const auto colShape1 = collider1.Shape();
	const auto colShape2 = collider2.Shape();

	switch (static_cast<ShapeType>(colShape1.index()))
	{
	case ShapeType::Circle:
	{
		const auto circleA = std::get<CircleF>(colShape1) + body1.Position();

		switch (static_cast<ShapeType>(colShape2.index()))
		{
		case ShapeType::Circle:
		{
			const auto circleB = std::get<CircleF>(colShape2) + body2.Position();

			const auto cA = circleA.Center(), cB = circleB.Center();
			const auto rA = circleA.Radius(), rB = circleB.Radius();

			const auto delta = cA - cB;
			expectedContactSolver.Normal = delta.Normalized();
			expectedContactSolver.Point = cA + delta * 0.5f;
			expectedContactSolver.Penetration = rA + rB - delta.Length();

			break;
		}

		case ShapeType::Rectangle:
		{
			const auto rectB = std::get<RectangleF>(colShape2) + body2.Position();

			const auto circleCenter = circleA.Center(), rectCenter = rectB.Center();
			const auto rectHalfSize = rectB.HalfSize();

			const auto delta = circleCenter - rectCenter;

			Math::Vec2F closestPoint;

			closestPoint.X = Math::Clamp(delta.X, -rectHalfSize.X, rectHalfSize.X);
			closestPoint.Y = Math::Clamp(delta.Y, -rectHalfSize.Y, rectHalfSize.Y);

			const auto distance = (closestPoint - delta).Length();
			const auto closestPoinOnRect = rectCenter + closestPoint;

			auto circleToRect = (circleCenter - closestPoinOnRect);

			if (circleToRect.Length() <= Math::Epsilon)
			{
				circleToRect = Math::Vec2F(0.f, 1.f);
			}

			expectedContactSolver.Normal = circleToRect.Normalized();
			expectedContactSolver.Point = closestPoinOnRect;
			expectedContactSolver.Penetration = circleA.Radius() - distance;

			break;
		}
		}

		break;
	}

	case ShapeType::Rectangle:
	{
		const auto rectA = std::get<RectangleF>(colShape1) + body1.Position();

		switch (static_cast<ShapeType>(colShape2.index()))
		{
		case ShapeType::Circle:
		{
			const auto circleB = std::get<CircleF>(colShape2) + body2.Position();

			const auto circleCenter = circleB.Center(), rectCenter = rectA.Center();
			const auto rectHalfSize = rectA.HalfSize();

			const auto delta = circleCenter - rectCenter;

			Math::Vec2F closestPoint;

			closestPoint.X = Math::Clamp(delta.X, -rectHalfSize.X, rectHalfSize.X);
			closestPoint.Y = Math::Clamp(delta.Y, -rectHalfSize.Y, rectHalfSize.Y);

			const auto distance = (closestPoint - delta).Length();
			const auto closestPoinOnRect = rectCenter + closestPoint;

			auto circleToRect = (circleCenter - closestPoinOnRect);

			if (circleToRect.Length() <= Math::Epsilon)
			{
				circleToRect = Math::Vec2F(0.f, 1.f);
			}

			expectedContactSolver.Normal = circleToRect.Normalized();
			expectedContactSolver.Point = closestPoinOnRect;
			expectedContactSolver.Penetration = circleB.Radius() - distance;

			break;
		}

		case ShapeType::Rectangle:
		{
			const auto rectB = std::get<RectangleF>(colShape2) + body2.Position();

			const auto cA = rectA.Center(), cB = rectB.Center();
			const auto halfSizeA = rectA.HalfSize(), halfSizeB = rectB.HalfSize();

			const auto delta = cA - cB;
			expectedContactSolver.Point = cA + delta * 0.5f;

			// Calculate the penetration in x-axis
			const auto penetrationX = halfSizeA.X + halfSizeB.X - Math::Abs(delta.X);
			// Calculate the penetration in y-axis
			const auto penetrationY = halfSizeA.Y + halfSizeB.Y - Math::Abs(delta.Y);

			if (penetrationX < penetrationY)
			{
				expectedContactSolver.Normal = delta.X > 0 ?
					Math::Vec2F::Right() : Math::Vec2F::Left();

				expectedContactSolver.Penetration = penetrationX;
			}
			else
			{
				expectedContactSolver.Normal = delta.Y > 0 ? Math::Vec2F::Up() : Math::Vec2F::Down();

				expectedContactSolver.Penetration = penetrationY;
			}

			break;
		}
		default:
			break;
		}

		break;
	}

	case ShapeType::Polygon:
	{
		collider1.SetShape(std::get<PolygonF>(colShape1));
		break;
	}
	default:
		break;
	}
}

void CalculateTestPostColVel(const ContactSolver& testedContactSolver, ContactSolver& expectedContactSolver)
{
	const auto separatingVelocity = testedContactSolver.CalculateSeparatingVelocity();

	// If the seperating velocity is positive, we don't need to calculate the delta velocity.
	if (separatingVelocity > 0)
	{
		return;
	}

	auto* body1 = expectedContactSolver.BodyA;
	auto* body2 = expectedContactSolver.BodyB;
	auto* collider1 = expectedContactSolver.ColliderA;
	auto* collider2 = expectedContactSolver.ColliderB;

	const auto mA = body1->Mass(), mB = body2->Mass();
	const auto eA = collider1->Restitution(), eB = collider2->Restitution();

	const auto combinedRestitution = (mA * eA + mB * eB) / (mA + mB);

	const auto finalSepVelocity = -separatingVelocity * combinedRestitution;
	const auto deltaVelocity = finalSepVelocity - separatingVelocity;

	const auto inversMassBodyA = body1->InverseMass();
	const auto inversMassBodyB = body2->InverseMass();
	const auto totalInverseMass = inversMassBodyA + inversMassBodyB;

	const auto impusle = deltaVelocity / totalInverseMass;
	const auto impulsePerIMass = expectedContactSolver.Normal * impusle;

	if (body1->GetBodyType() == BodyType::Dynamic)
	{
		body1->SetVelocity(body1->Velocity() + impulsePerIMass * inversMassBodyA);
	}

	if (body2->GetBodyType() == BodyType::Dynamic)
	{
		body2->SetVelocity(body2->Velocity() - impulsePerIMass * inversMassBodyB);
	}
}

void CalculateTestPostColPos(const ContactSolver& testedContactSolver, ContactSolver& expectedContactSolver)
{
	auto* body1 = expectedContactSolver.BodyA;
	auto* body2 = expectedContactSolver.BodyB;
	auto* collider1 = expectedContactSolver.ColliderA;
	auto* collider2 = expectedContactSolver.ColliderB;

	if (expectedContactSolver.Penetration <= 0) return;

	const auto inversMassBodyA = body1->InverseMass();
	const auto inversMassBodyB = body2->InverseMass();
	const auto totalInverseMass = inversMassBodyA + inversMassBodyB;

	if (totalInverseMass <= 0) return;

	const auto movePerIMass = expectedContactSolver.Normal *
		(expectedContactSolver.Penetration / totalInverseMass);

	if (body1->GetBodyType() == BodyType::Dynamic)
	{
		body1->SetPosition(body1->Position() + movePerIMass * inversMassBodyA);
	}

	if (body2->GetBodyType() == BodyType::Dynamic)
	{
		body2->SetPosition(body2->Position() - movePerIMass * inversMassBodyB);
	}
}

TEST_P(PairOfCollidingBodies, CalculateContactProperties)
{
	auto& [cb1, cb2] = GetParam();

	World world;
	world.Init();

	const auto bodyRef1 = world.CreateBody();
	auto& body1 = world.GetBody(bodyRef1);
	const auto colRef1 = world.CreateCollider(bodyRef1);
	auto& collider1 = world.GetCollider(colRef1);

	const auto bodyRef2 = world.CreateBody();
	auto& body2 = world.GetBody(bodyRef2);
	const auto colRef2 = world.CreateCollider(bodyRef2);
	auto& collider2 = world.GetCollider(colRef2);

	ContactSolver testedContactSolver;
	ContactSolver expectedContactSolver;

	SetBodyAndColliderValues(cb1, cb2, body1, body2, collider1, collider2, 
		testedContactSolver, expectedContactSolver);

	testedContactSolver.CalculateContactProperties();
	CalculateTestContactProperties(body1, body2, collider1, collider2, expectedContactSolver);

	EXPECT_EQ(testedContactSolver.Normal, expectedContactSolver.Normal);
	EXPECT_EQ(testedContactSolver.Point, expectedContactSolver.Point);
	EXPECT_EQ(testedContactSolver.Penetration, expectedContactSolver.Penetration);
}


TEST_P(PairOfCollidingBodies, CalculateSeparatingVelocity)
{
	auto& [cb1, cb2] = GetParam();

	World world;
	world.Init();

	const auto bodyRef1 = world.CreateBody();
	auto& body1 = world.GetBody(bodyRef1);
	const auto colRef1 = world.CreateCollider(bodyRef1);
	auto& collider1 = world.GetCollider(colRef1);

	const auto bodyRef2 = world.CreateBody();
	auto& body2 = world.GetBody(bodyRef1);
	const auto colRef2 = world.CreateCollider(bodyRef2);
	auto& collider2 = world.GetCollider(colRef2);

	ContactSolver testedContactSolver;
	ContactSolver expectedContactSolver;

	SetBodyAndColliderValues(cb1, cb2, body1, body2, collider1, collider2,
		testedContactSolver, expectedContactSolver);

	testedContactSolver.CalculateContactProperties();
	CalculateTestContactProperties(body1, body2, collider1, collider2, expectedContactSolver);

	const auto testedVel = testedContactSolver.CalculateSeparatingVelocity();

	const auto relativeVelocity = body1.Velocity() - body2.Velocity();
	const auto separatingVelocity = relativeVelocity.Dot(expectedContactSolver.Normal);
}

TEST_P(PairOfCollidingBodies, CalculatePostCollisionVelocity)
{
	auto& [cb1, cb2] = GetParam();

	World world;
	world.Init();

	const auto bodyRef1 = world.CreateBody();
	auto& body1 = world.GetBody(bodyRef1);
	const auto colRef1 = world.CreateCollider(bodyRef1);
	auto& collider1 = world.GetCollider(colRef1);

	const auto bodyRef2 = world.CreateBody();
	auto& body2 = world.GetBody(bodyRef1);
	const auto colRef2 = world.CreateCollider(bodyRef2);
	auto& collider2 = world.GetCollider(colRef2);

	ContactSolver testedContactSolver;
	ContactSolver expectedContactSolver;

	SetBodyAndColliderValues(cb1, cb2, body1, body2, collider1, collider2,
		testedContactSolver, expectedContactSolver);

	testedContactSolver.CalculateContactProperties();
	CalculateTestContactProperties(body1, body2, collider1, collider2, expectedContactSolver);

	CalculateTestPostColVel(testedContactSolver, expectedContactSolver);

	EXPECT_EQ(testedContactSolver.BodyA->Velocity(), body1.Velocity());
	EXPECT_EQ(testedContactSolver.BodyB->Velocity(), body2.Velocity());
}

TEST_P(PairOfCollidingBodies, CalculatePostCollisionPosition)
{
	auto& [cb1, cb2] = GetParam();

	World world;
	world.Init();

	const auto bodyRef1 = world.CreateBody();
	auto& body1 = world.GetBody(bodyRef1);
	const auto colRef1 = world.CreateCollider(bodyRef1);
	auto& collider1 = world.GetCollider(colRef1);

	const auto bodyRef2 = world.CreateBody();
	auto& body2 = world.GetBody(bodyRef1);
	const auto colRef2 = world.CreateCollider(bodyRef2);
	auto& collider2 = world.GetCollider(colRef2);

	ContactSolver testedContactSolver;
	ContactSolver expectedContactSolver;

	SetBodyAndColliderValues(cb1, cb2, body1, body2, collider1, collider2,
		testedContactSolver, expectedContactSolver);

	testedContactSolver.CalculateContactProperties();
	CalculateTestContactProperties(body1, body2, collider1, collider2, expectedContactSolver);

	CalculateTestPostColPos(testedContactSolver, expectedContactSolver);

	EXPECT_EQ(testedContactSolver.BodyA->Position(), body1.Position());
	EXPECT_EQ(testedContactSolver.BodyB->Position(), body2.Position());
}

TEST_P(PairOfCollidingBodies, ResolveContact)
{
	auto& [cb1, cb2] = GetParam();

	World world;
	world.Init();

	const auto bodyRef1 = world.CreateBody();
	auto& body1 = world.GetBody(bodyRef1);
	const auto colRef1 = world.CreateCollider(bodyRef1);
	auto& collider1 = world.GetCollider(colRef1);

	const auto bodyRef2 = world.CreateBody();
	auto& body2 = world.GetBody(bodyRef1);
	const auto colRef2 = world.CreateCollider(bodyRef2);
	auto& collider2 = world.GetCollider(colRef2);

	ContactSolver testedContactSolver;
	ContactSolver expectedContactSolver;

	SetBodyAndColliderValues(cb1, cb2, body1, body2, collider1, collider2,
		testedContactSolver, expectedContactSolver);


	testedContactSolver.ResolveContact();

	CalculateTestContactProperties(body1, body2, collider1, collider2, expectedContactSolver);

	if (body1.GetBodyType() == BodyType::Static)
	{
		std::swap(body1, body2);
		expectedContactSolver.Normal = -expectedContactSolver.Normal;
	}

	CalculateTestPostColVel(testedContactSolver, expectedContactSolver);
	CalculateTestPostColPos(testedContactSolver, expectedContactSolver);

	EXPECT_EQ(testedContactSolver.Normal, expectedContactSolver.Normal);
	EXPECT_EQ(testedContactSolver.Point, expectedContactSolver.Point);
	EXPECT_EQ(testedContactSolver.Penetration, expectedContactSolver.Penetration);

	EXPECT_EQ(testedContactSolver.BodyA->Velocity(), body1.Velocity());
	EXPECT_EQ(testedContactSolver.BodyB->Velocity(), body2.Velocity());

	EXPECT_EQ(testedContactSolver.BodyA->Position(), body1.Position());
	EXPECT_EQ(testedContactSolver.BodyB->Position(), body2.Position());
}