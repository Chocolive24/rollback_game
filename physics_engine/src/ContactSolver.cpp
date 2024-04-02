#include "ContactSolver.h"

namespace PhysicsEngine
{
    void ContactSolver::InitContactActors(Body& bodyA, 
        Body& bodyB, 
        Collider& colA, 
        Collider& colB) noexcept
    {
        BodyA = &bodyA;
        BodyB = &bodyB;
        ColliderA = &colA;
        ColliderB = &colB;
    }

    void ContactSolver::CalculateContactProperties() noexcept
    {
        const auto colShapeA = ColliderA->Shape();
        const auto colShapeB = ColliderB->Shape();

        switch (static_cast<Math::ShapeType>(colShapeA.index()))
        {
        case Math::ShapeType::Circle:
        {
            const auto circleA = std::get<Math::CircleF>(colShapeA) + BodyA->Position();

            switch (static_cast<Math::ShapeType>(colShapeB.index()))
            {
            case Math::ShapeType::Circle:
            {
                const auto circleB = std::get<Math::CircleF>(colShapeB) + BodyB->Position();
                const auto cA = circleA.Center(), cB = circleB.Center();
                const auto rA = circleA.Radius(), rB = circleB.Radius();

                const auto delta = cA - cB;
                const float distance = delta.Length();
                const auto normalizedDelta =
                    distance > Math::Epsilon ? delta.Normalized() : delta;

                Normal = delta.Normalized();
                Point = cA + delta * 0.5f;
                Penetration = rA + rB - delta.Length();

                break;
            } // Case circle B.

            case Math::ShapeType::Rectangle:
            {
                const auto rectB = std::get<Math::RectangleF>(colShapeB) +
                    BodyB->Position();

                const auto circleCenter = circleA.Center(), rectCenter = rectB.Center();
                const auto rectHalfSize = rectB.HalfSize();

                const auto delta = circleCenter - rectCenter;

                Math::Vec2F closestPoint;

                closestPoint.X = Math::Clamp(delta.X, -rectHalfSize.X, rectHalfSize.X);
                closestPoint.Y = Math::Clamp(delta.Y, -rectHalfSize.Y, rectHalfSize.Y);

                const auto distance = (closestPoint - delta).Length();
                const auto closestPoinOnRect = rectCenter + closestPoint;

                auto circleToRect = (circleCenter - closestPoinOnRect);

                const auto normalizedDelta =
                    circleToRect.Length() > Math::Epsilon ? circleToRect.Normalized()
                                                          : circleToRect;

                //if (circleToRect.Length() <= Math::Epsilon)
                //{
                //    circleToRect = Math::Vec2F(0.f, 1.f);
                //}

                Normal = circleToRect.Normalized();
                Point = closestPoinOnRect;
                Penetration = circleA.Radius() - distance;

                break;
            } // Case rectangle B.
            case Math::ShapeType::Polygon:
            case Math::ShapeType::None:
                break;
            } // Switch shape collider B.

            break;
        } // Case circle A.

        case Math::ShapeType::Rectangle:
        {
            const auto rectA = std::get<Math::RectangleF>(colShapeA) + BodyA->Position();

            switch (static_cast<Math::ShapeType>(colShapeB.index()))
            {
            case Math::ShapeType::Circle:
            {
                const auto circleB = std::get<Math::CircleF>(colShapeB) +
                    BodyB->Position();

                std::swap(BodyA, BodyB);
                std::swap(ColliderA, ColliderB);
                CalculateContactProperties();

                break;
            } // Case circle B.

            case Math::ShapeType::Rectangle:
            {
                const auto rectB = std::get<Math::RectangleF>(colShapeB) +
                    BodyB->Position();

                const auto cA = rectA.Center(), cB = rectB.Center();
                const auto halfSizeA = rectA.HalfSize(), halfSizeB = rectB.HalfSize();

                const auto delta = cA - cB;
                Point = cA + delta * 0.5f;

                // Calculate the penetration in x-axis
                const auto penetrationX = halfSizeA.X + halfSizeB.X - Math::Abs(delta.X);
                // Calculate the penetration in y-axis
                const auto penetrationY = halfSizeA.Y + halfSizeB.Y - Math::Abs(delta.Y);

                if (penetrationX < penetrationY)
                {
                    Normal = delta.X > 0 ?
                        Math::Vec2F::Right() : Math::Vec2F::Left();

                    Penetration = penetrationX;
                }
                else
                {
                    Normal = delta.Y > 0 ? Math::Vec2F::Up() : Math::Vec2F::Down();

                    Penetration = penetrationY;
                }

                break;
            } // Case rectangle B.

            case Math::ShapeType::None:
                break;
            default:
                break;
            } // Switch shape collider B.

            break;
        } // Case rectangle A.

        case Math::ShapeType::None:
            break;
        default:
            break;
        } // Switch shape collider A.
    }

    float ContactSolver::CalculateSeparatingVelocity() const noexcept
    {
        const auto relativeVelocity = BodyA->Velocity() - BodyB->Velocity();
        const auto separatingVelocity = relativeVelocity.Dot(Normal);

        return separatingVelocity;
    }

    void ContactSolver::ResolvePostCollisionVelocity() noexcept
    {
        const auto separatingVelocity = CalculateSeparatingVelocity();

        // If the seperating velocity is positive, we don't need to calculate the delta velocity.
        if (separatingVelocity > 0)
        {
            return;
        }

        const auto mA = BodyA->Mass(), mB = BodyB->Mass();
        const auto eA = ColliderA->Restitution(), eB = ColliderB->Restitution();

        const auto combinedRestitution = (mA * eA + mB * eB) / (mA + mB);

        const auto finalSepVelocity = -separatingVelocity * combinedRestitution;
        const auto deltaVelocity = finalSepVelocity - separatingVelocity;

        const auto inversMassBodyA = BodyA->InverseMass();
        const auto inversMassBodyB = BodyB->InverseMass();
        const auto totalInverseMass = inversMassBodyA + inversMassBodyB;

        const auto impusle = deltaVelocity / totalInverseMass;
        const auto impulsePerIMass = Normal * impusle;

        if (BodyA->GetBodyType() == BodyType::Dynamic)
        {
            BodyA->SetVelocity(BodyA->Velocity() + impulsePerIMass * inversMassBodyA);
        }

        if (BodyB->GetBodyType() == BodyType::Dynamic)
        {
            BodyB->SetVelocity(BodyB->Velocity() - impulsePerIMass * inversMassBodyB);
        }
    }

    void ContactSolver::ResolvePostCollisionPosition() noexcept
    {
        if (Penetration <= 0) return;

        const auto inversMassBodyA = BodyA->InverseMass();
        const auto inversMassBodyB = BodyB->InverseMass();
        const auto totalInverseMass = inversMassBodyA + inversMassBodyB;

        if (totalInverseMass <= 0) return;

        const auto movePerIMass = Normal * (Penetration / totalInverseMass);

        if (BodyA->GetBodyType() == BodyType::Dynamic)
        {
            BodyA->SetPosition(BodyA->Position() + movePerIMass * inversMassBodyA);
        }

        if (BodyB->GetBodyType() == BodyType::Dynamic)
        {
            BodyB->SetPosition(BodyB->Position() - movePerIMass * inversMassBodyB);
        }
    }

    void ContactSolver::ResolveContact() noexcept
    {
        CalculateContactProperties();

        if (BodyA->GetBodyType() == BodyType::Static)
        {
            std::swap(BodyA, BodyB);
            Normal = -Normal;
        }

        ResolvePostCollisionVelocity();
        ResolvePostCollisionPosition();
    }
}