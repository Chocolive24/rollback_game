/**
 * @author Olivier
 */

#include "World.h"

#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#include <TracyC.h>
#endif // TRACY_ENABLE

#include <iostream>

namespace PhysicsEngine
{
    void World::Init(Math::Vec2F gravity, int preallocatedBodyCount) noexcept
    {
#ifdef TRACY_ENABLE
        ZoneScoped;
#endif // TRACY_ENABLE

        _gravity = gravity;

        if (preallocatedBodyCount < 0) preallocatedBodyCount = 0;

        _bodies.resize(preallocatedBodyCount, Body());
        _bodiesGenIndices.resize(preallocatedBodyCount, 0);

        _colliders.resize(preallocatedBodyCount, Collider());
        _collidersGenIndices.resize(preallocatedBodyCount, 0);

        _quadTree.Init();
    }

    void World::Update(const float deltaTime) noexcept
    {
    #ifdef TRACY_ENABLE
            ZoneScoped;
    #endif

    #ifdef TRACY_ENABLE
            ZoneNamedN(CalculateBodiesAcceleration, "CalculateBodiesAcceleration", true);
            ZoneValue(_bodies.size());
    #endif
        for (auto& body : _bodies)
        {
            if (!body.IsValid()) continue;

            switch (body.GetBodyType())
            {
                case BodyType::Dynamic:
                {
                    body.ApplyForce(_gravity);

                    // a = F / m
                    Math::Vec2F acceleration = body.Forces() * body.InverseMass();
                    
                    // Change velocity according to delta time.
                    body.SetVelocity(body.Velocity() + acceleration * deltaTime);

                    // Apply damping to velocity according to delta time.
                    body.SetVelocity(body.Velocity() *
                                     (1.0f - body.Damping() * deltaTime));

                    // Change position according to velocity and delta time.
                    body.SetPosition(body.Position() + body.Velocity() * deltaTime);

                    body.ResetForces();

                    break;
                }

                case BodyType::Kinematic:
                {
                    // Kinematic bodies are not impacted by forces.

                    // Change position according to velocity and delta time.
                    body.SetPosition(body.Position() + body.Velocity() * deltaTime);

                    break;
                }

                case BodyType::Static:
                case BodyType::None:
                    break;
            }
        }

        if (_contactListener)
        {
            resolveBroadPhase();
            resolveNarrowPhase();
        }
    }

    void World::resolveBroadPhase() noexcept
    {
    #ifdef TRACY_ENABLE
            ZoneScoped;
    #endif

    #ifdef TRACY_ENABLE
            ZoneNamedN(SetRoodNodeBoundary, "SetRootNodeBoundary", true);
            ZoneValue(_colliders.size());
    #endif

        _quadTree.Clear();

        // Sets the minimum and maximum collision zone limits of the world rectangle to floating maximum and
        // lowest values.
        Math::Vec2F worldMinBound(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
        Math::Vec2F worldMaxBound(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());

        // Adjust the size of the collision zone in the world rectangle to the most distant bodies.
        for (const auto& collider : _colliders)
        {
            if (!collider.Enabled()) continue;

            const auto colCenter = GetBody(collider.GetBodyRef()).Position();

            if (worldMinBound.X > colCenter.X)
            {
                worldMinBound.X = colCenter.X;
            }

            if (worldMaxBound.X < colCenter.X)
            {
                worldMaxBound.X = colCenter.X;
            }

            if (worldMinBound.Y > colCenter.Y)
            {
                worldMinBound.Y = colCenter.Y;
            }

            if (worldMaxBound.Y < colCenter.Y)
            {
                worldMaxBound.Y = colCenter.Y;
            }
        }

        // Set the first rectangle of the quad-tree to calculated collision area rectangle.
        _quadTree.SetRootNodeBoundary(Math::RectangleF(worldMinBound, worldMaxBound));

    #ifdef TRACY_ENABLE
            ZoneNamedN(InsertCollidersInQuadTree, "InsertCollidersInQuadTree", true);
            ZoneValue(_colliders.size());
    #endif

        for (std::size_t i = 0; i < _colliders.size(); i++)
        {
            ColliderRef colliderRef = {i, _collidersGenIndices[i]};
            const auto& collider = GetCollider(colliderRef);

            if (!collider.Enabled()) continue;

            const auto colShape = collider.Shape();

            switch (static_cast<Math::ShapeType>(colShape.index()))
            {
                case Math::ShapeType::Circle:
                {
                #ifdef TRACY_ENABLE
                       ZoneNamedN(InsertCircle, "InsertCircle", true);
                #endif
                    const auto circle = std::get<Math::CircleF>(colShape);
                    const auto radius = circle.Radius();
                    const auto simplifiedCircle = Math::RectangleF::FromCenter(
                            GetBody(collider.GetBodyRef()).Position() + collider.Offset(),
                            Math::Vec2F(radius, radius));

                    _quadTree.Insert(simplifiedCircle, colliderRef);
                    break;
                } // Case circle.

                case Math::ShapeType::Rectangle:
                {
                #ifdef TRACY_ENABLE
                       ZoneNamedN(InsertRectangle, "InsertRectangle", true);
                #endif

                    const auto rect = std::get<Math::RectangleF>(colShape) +
                           GetBody(collider.GetBodyRef()).Position() + collider.Offset();

                    _quadTree.Insert(rect, colliderRef);
                    break;
                } // Case rectangle.

                case Math::ShapeType::Polygon:
                {
                #ifdef TRACY_ENABLE
                    ZoneNamedN(InsertPolygon, "InsertPolygon", true);
                #endif

                    Math::Vec2F minVertex(std::numeric_limits<float>::max(), 
                                          std::numeric_limits<float>::max());

                    Math::Vec2F maxVertex(std::numeric_limits<float>::lowest(),
                                          std::numeric_limits<float>::lowest());

                    const auto poly = std::get<Math::PolygonF>(colShape) +
                        GetBody(collider.GetBodyRef()).Position() + collider.Offset();

                    for (const auto& vertex : poly.Vertices())
                    {
                        if (minVertex.X > vertex.X)
                        {
                            minVertex.X = vertex.X;
                        }

                        if (maxVertex.X < vertex.X)
                        {
                            maxVertex.X = vertex.X;
                        }

                        if (minVertex.Y > vertex.Y)
                        {
                            minVertex.Y = vertex.Y;
                        }

                        if (maxVertex.Y < vertex.Y)
                        {
                            maxVertex.Y = vertex.Y;
                        }
                    } // For range vertex.

                    Math::RectangleF simplifiedPoly(minVertex, maxVertex);

                    _quadTree.Insert(simplifiedPoly, colliderRef);
                    break;
                } // Case polygon.
            } // Switch collider shape index.
        } // For int i < colliders.size().

        _quadTree.CalculatePossiblePairs();
    }

    void World::resolveNarrowPhase() noexcept
    {
        #ifdef TRACY_ENABLE
                ZoneScoped;
        #endif

        const auto& possiblePairs = _quadTree.PossiblePairs();

        #ifdef TRACY_ENABLE
                ZoneValue(possiblePairs.size());
        #endif

        AllocVector<ColliderPair> newPairs{ StandardAllocator<ColliderPair>{_heapAllocator} };
        newPairs.reserve(possiblePairs.size());

        for (const auto& possiblePair : possiblePairs)
        {
            auto& colliderA = GetCollider(possiblePair.ColliderA);
            auto& colliderB = GetCollider(possiblePair.ColliderB);

            if (detectOverlap(colliderA, colliderB))
            {
                newPairs.push_back(possiblePair);
            }
        }

        for (const auto& newPair : newPairs)
        {
            Collider& colliderA = GetCollider(newPair.ColliderA);
            Collider& colliderB = GetCollider(newPair.ColliderB);

            const auto it = std::find(_colliderPairs.begin(), _colliderPairs.end(), newPair);

            // If there was no collision in the previous frame -> OnTriggerEnter.
            if (it == _colliderPairs.end())
            {
                if (colliderA.IsTrigger() || colliderB.IsTrigger())
                {
                    _contactListener->OnTriggerEnter(newPair.ColliderA, newPair.ColliderB);
                }

                else
                {
                    ContactSolver contactSolver;
                    contactSolver.InitContactActors(GetBody(colliderA.GetBodyRef()),
                                                    GetBody(colliderB.GetBodyRef()),
                                                    colliderA,
                                                    colliderB);

                    contactSolver.ResolveContact();
                    _contactListener->OnCollisionEnter(newPair.ColliderA, newPair.ColliderB);
                }
            }
            // If there was a collision in the previous frame and there is always a collision -> OnTriggerStay.
            else
            {
                if (colliderA.IsTrigger() || colliderB.IsTrigger())
                {
                    _contactListener->OnTriggerStay(newPair.ColliderA, newPair.ColliderB);
                }
                else
                {
                    ContactSolver contactSolver;
                    contactSolver.InitContactActors(GetBody(colliderA.GetBodyRef()),
                                                    GetBody(colliderB.GetBodyRef()),
                                                    colliderA,
                                                    colliderB);

                    contactSolver.ResolveContact();
                }
            }
        }

        for (auto& colliderPair : _colliderPairs)
        {
            Collider& colliderA = GetCollider(colliderPair.ColliderA);
            Collider& colliderB = GetCollider(colliderPair.ColliderB);

            const auto it = std::find(newPairs.begin(), newPairs.end(), colliderPair);

            // If there is no collision in this frame -> OnTriggerExit.
            if (it == newPairs.end())
            {
                if (colliderA.IsTrigger() || colliderB.IsTrigger())
                {
                    _contactListener->OnTriggerExit(colliderPair.ColliderA,
                                                    colliderPair.ColliderB);
                }
                else
                {
                    ContactSolver contactSolver;
                    contactSolver.InitContactActors(GetBody(colliderA.GetBodyRef()),
                                                    GetBody(colliderB.GetBodyRef()),
                                                    colliderA,
                                                    colliderB);

                    contactSolver.ResolveContact();
                    _contactListener->OnCollisionExit(colliderPair.ColliderA,
                                                      colliderPair.ColliderB);
                }
            }
        }

        _colliderPairs = newPairs;
    }

    bool World::detectOverlap(const Collider& colA, const Collider& colB) noexcept
    {
    #ifdef TRACY_ENABLE
            ZoneScoped;
    #endif
        auto& bodyA = GetBody(colA.GetBodyRef());
        auto& bodyB = GetBody(colB.GetBodyRef());

        const auto colShapeA = colA.Shape();
        const auto colShapeB = colB.Shape();

        bool doCollidersIntersect = false;

        switch (static_cast<Math::ShapeType>(colShapeA.index()))
        {
            case Math::ShapeType::Circle:
            {
                const auto circleA = std::get<Math::CircleF>(colShapeA) + colA.Offset() + bodyA.Position();
                switch (static_cast<Math::ShapeType>(colShapeB.index()))
                {
                    case Math::ShapeType::Circle:
                    {
                    #ifdef TRACY_ENABLE
                        std::string txt = "Circle-Circle";
                        ZoneText(txt.c_str(), txt.size());
                    #endif

                        const auto circleB = std::get<Math::CircleF>(colShapeB) + colB.Offset() + bodyB.Position();

                        doCollidersIntersect = Math::Intersect(circleA, circleB);

                        break;
                    } // Case circle B.
                    
                    case Math::ShapeType::Rectangle:
                    {
                    #ifdef TRACY_ENABLE
                            std::string txt = "Circle-Rectangle";
                            ZoneText(txt.c_str(), txt.size());
                    #endif
                        const auto rectB = std::get<Math::RectangleF>(colShapeB) +
                                colB.Offset() + bodyB.Position();

                        doCollidersIntersect = Math::Intersect(circleA, rectB);

                        break;
                    } // Case rectangle B.

                    case Math::ShapeType::Polygon:
                    {
                    #ifdef TRACY_ENABLE
                        std::string txt = "Circle-Polygon";
                        ZoneText(txt.c_str(), txt.size());
                    #endif
                        const auto polygonB = std::get<Math::PolygonF>(colShapeB) +
                            colB.Offset() + bodyB.Position();

                        doCollidersIntersect = Math::Intersect(circleA, polygonB);
                        break;
                    } // Case polygon B.

                    case Math::ShapeType::None:
                        break;
                    default:
                        break;
                } // Switch shape collider B.

                break;
            } // Case circle A.

            case Math::ShapeType::Rectangle:
            {
                const auto rectA = std::get<Math::RectangleF>(colShapeA) + bodyA.Position();

                switch (static_cast<Math::ShapeType>(colShapeB.index()))
                {
                    case Math::ShapeType::Circle:
                    {
                    #ifdef TRACY_ENABLE
                            std::string txt = "Rectangle-Cricle";
                            ZoneText(txt.c_str(), txt.size());
                    #endif

                        const auto circleB =
                                std::get<Math::CircleF>(colShapeB) +
                                colB.Offset() + bodyB.Position();

                        doCollidersIntersect = Math::Intersect(rectA, circleB);

                        break;
                    } // Case circle B.

                    case Math::ShapeType::Rectangle:
                    {
                    #ifdef TRACY_ENABLE
                            std::string txt = "Rectangle-Rectangle";
                            ZoneText(txt.c_str(), txt.size());
                    #endif

                        const auto rectB = std::get<Math::RectangleF>(colShapeB) +
                                colB.Offset() + bodyB.Position();

                        doCollidersIntersect = Math::Intersect(rectA, rectB);

                        break;
                    } // Case rectangle B.

                    case Math::ShapeType::Polygon:
                    {
                    #ifdef TRACY_ENABLE
                            std::string txt = "Rectangle-Polygon";
                            ZoneText(txt.c_str(), txt.size());
                    #endif

                        const auto polygonB = std::get<Math::PolygonF>(colShapeB) +
                                              colB.Offset() + bodyB.Position();

                        doCollidersIntersect = Math::Intersect(rectA, polygonB);
                        break;
                    } // Case polygon B.

                    case Math::ShapeType::None:
                        break;
                    default:
                        break;
                } // Switch shape collider B.

                break;
            } // Case rectangle A.

            case Math::ShapeType::Polygon:
            {
                const auto polygonA = std::get<Math::PolygonF>(colShapeA) +
                                      colB.Offset() + bodyA.Position();

                switch (static_cast<Math::ShapeType>(colShapeB.index()))
                {
                    case Math::ShapeType::Circle:
                    {
                    #ifdef TRACY_ENABLE
                            std::string txt = "Polygon-Circle";
                            ZoneText(txt.c_str(), txt.size());
                    #endif

                        const auto circleB =
                                std::get<Math::CircleF>(colShapeB) +
                                colB.Offset() + bodyB.Position();

                        doCollidersIntersect = Math::Intersect(polygonA, circleB);
                        break;
                    } // Case circle B.

                    case Math::ShapeType::Rectangle:
                    {
                    #ifdef TRACY_ENABLE
                            std::string txt = "Polygon-Rectangle";
                            ZoneText(txt.c_str(), txt.size());
                    #endif

                        const auto rectB = std::get<Math::RectangleF>(colShapeB) +
                                colB.Offset() + bodyB.Position();

                        doCollidersIntersect = Math::Intersect(polygonA, rectB);
                        break;
                    } // Case rectangle B.

                    case Math::ShapeType::Polygon:
                    {
                    #ifdef TRACY_ENABLE
                            std::string txt = "Polygon-Polygon";
                            ZoneText(txt.c_str(), txt.size());
                    #endif

                        const auto polygonB = std::get<Math::PolygonF>(colShapeB) +
                                colB.Offset() + bodyB.Position();

                        doCollidersIntersect = Math::Intersect(polygonA, polygonB);
                        break;
                    } // Case polygon B.

                    case Math::ShapeType::None:
                        break;
                    default:
                        break;
                } // Switch shape collider B.

                break;
            } // Case Polygon A.

            case Math::ShapeType::None:
                break;
            default:
                break;
        } // Switch shape collider A.

        return doCollidersIntersect;
    }

    void World::Deinit() noexcept
    {
#ifdef TRACY_ENABLE
        ZoneScoped;
#endif // TRACY_ENABLE

        _bodies.clear();
        _bodiesGenIndices.clear();

        _colliders.clear();
        _collidersGenIndices.clear();
        _colliderPairs.clear();

        _contactListener = nullptr;

        _quadTree.Deinit();
    }

    [[nodiscard]] BodyRef World::CreateBody() noexcept
    {
        const auto it = std::find_if(_bodies.begin(), _bodies.end(),
            [](const Body& body)
        {
            return !body.IsValid();
        });

        if (it != _bodies.end())
        {
            // Found an invalid body.
            const std::size_t index = std::distance(_bodies.begin(), it);

            _bodies[index].SetMass(1.f);

            return BodyRef{index, _bodiesGenIndices[index]};
        }

        // No body with negative mass found.
        const std::size_t previousSize = _bodies.size();
        const auto newSize = static_cast<std::size_t>(static_cast<float>(previousSize) * _bodyAllocResizeFactor);

        _bodies.resize(newSize, Body());
        _bodiesGenIndices.resize(newSize, 0);

        _bodies[previousSize].SetMass(1.f);

        return {previousSize, _bodiesGenIndices[previousSize] };
    }

    void World::DestroyBody(BodyRef bodyRef) noexcept
    {
        _bodies[bodyRef.Index] = Body();
        _bodiesGenIndices[bodyRef.Index]++;
    }

    Body& World::GetBody(BodyRef bodyRef)
    {
        if (_bodiesGenIndices[bodyRef.Index] != bodyRef.GenerationIdx)
        {
            throw std::runtime_error("Null body reference exception");
        }

        return _bodies[bodyRef.Index];
    }

    Collider& World::GetCollider(ColliderRef colliderRef)
    {
        if (_collidersGenIndices[colliderRef.Index] != colliderRef.GenerationIdx)
        {
            throw std::runtime_error("Null collider reference exception");
        }

        return _colliders[colliderRef.Index];
    }

    ColliderRef World::CreateCollider(BodyRef bodyRef) noexcept
    {
        std::size_t colliderIdx = -1;

       const auto it = std::find_if(
                _colliders.begin(),
                _colliders.end(),
                [](const Collider& collider)
        {
            return !collider.IsInitialized();
        });

        if (it != _colliders.end())
        {
            // Found an invalid collider.
            colliderIdx = std::distance(_colliders.begin(), it);
        }
        else
        {
            // No collider with none shape found.
            const std::size_t previousSize = _colliders.size();
            const auto newSize = static_cast<std::size_t>(
                static_cast<float>(previousSize) * _bodyAllocResizeFactor);

            _colliders.resize(newSize, Collider());
            _collidersGenIndices.resize(newSize, 0);

            colliderIdx = previousSize;
        }

        auto& collider = _colliders[colliderIdx];

        collider.SetIsInitialized(true);
        collider.SetEnabled(true);
        collider.SetBodyRef(bodyRef);

        ColliderRef colRef = {colliderIdx, _collidersGenIndices[colliderIdx]};

        return colRef;
    }

    void World::DestroyCollider(ColliderRef colRef) noexcept
    {
        _colliders[colRef.Index] = Collider();
        _collidersGenIndices[colRef.Index]++;
    }
}