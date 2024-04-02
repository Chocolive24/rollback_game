/**
 * @headerfile Collider.h
 * This file defines the Collider class and related structures for handling colliders in the physics world.
 *
 * @author Olivier Pachoud
 */

#pragma once

#include "WorldRefTypes.h"
#include "Shape.h"

#include <variant>
#include <utility>

namespace PhysicsEngine
{
    /**
     * @brief Collider is a class that represents a generic collider.
     */
    class Collider
    {
    private:
        std::variant<Math::CircleF, Math::RectangleF, Math::PolygonF> _shape{
                     Math::CircleF(Math::Vec2F::Zero(), 0.f)};
        BodyRef _bodyRef{};

        float _restitution{-1.f};
        float _friction{-1.f};
        bool _isTrigger{false};
        bool _enabled{false};

    public:
        constexpr Collider() noexcept = default;
        constexpr Collider(float restitution, float friction, bool isTrigger) noexcept
        {
            _restitution = restitution;
            _friction = friction;
            _isTrigger = isTrigger;
        };

        /**
         * @brief Shape is a method that gives the mathematical shape of the collider.
         * @return The mathematical shape of the collider.
         */
        [[nodiscard]] std::variant<Math::CircleF, Math::RectangleF, Math::PolygonF> Shape()
        const noexcept { return _shape; }

        /**
         * @brief SetShape is a method that replaces the current mathematical shape of the collider
         * with a circle shape given in parameter.
         * @param circle The new circle shape for the collider.
         */
        void SetShape(Math::CircleF circle) noexcept { _shape = circle; }

        /**
         * @brief SetShape is a method that replaces the current mathematical shape of the collider
         * with rectangle shape given in parameter.
         * @param rectangle The new rectangle shape for the collider.
         */
        void SetShape(Math::RectangleF rectangle) noexcept { _shape = rectangle; }

        /**
         * @brief SetShape is a method that replaces the current mathematical shape of the collider
         * with a polygon shape given in parameter.
         * @param polygon The new polygon shape for the collider.
         */
        void SetShape(Math::PolygonF polygon) noexcept { _shape = polygon; }

        /**
         * @brief GetBodyRef is a method that gives the body reference of the collider in the world.
         * @return The body reference of the collider in the world.
         */
        [[nodiscard]] constexpr BodyRef GetBodyRef() const noexcept { return _bodyRef; }

        /**
        * @brief SetBodyRef is a method that replaces the current body reference of the collider
        * with the new body reference given in parameter.
        * @param newBodyRef The new body reference for the collider.
        */
        constexpr void SetBodyRef(BodyRef newBodyRef) noexcept { _bodyRef = newBodyRef; }

        /**
         * @brief Restitution is a method that gives the restitution of the collider.
         * @return The restitution of the collider.
         */
        [[nodiscard]] constexpr float Restitution() const noexcept { return _restitution; }

        /**
        * @brief SetRestitution is a method that replaces the current restitution of the collider
        * with the new restitution given in parameter.
        * @param newRestitution The new restitution for the collider.
        */
        constexpr void SetRestitution(float newRestitution) noexcept { _restitution = newRestitution; }

        /**
         * @brief Friction is a method that gives the friction of the collider.
         * @return The friction of the collider.
         */
        [[nodiscard]] constexpr float Friction() const noexcept { return _friction; }

        /**
        * @brief SetFriction is a method that replaces the current friction of the collider
        * with the new friction given in parameter.
        * @param newFriction The new friction for the collider.
        */
        constexpr void SetFriction(float newFriction) noexcept { _friction = newFriction; }

        /**
         * @brief IsTrigger is a method that checks if the collider is trigger (aka if it detects collision
         * but don't act physically.
         * @return True if the collider is trigger.
         */
        [[nodiscard]] constexpr bool IsTrigger() const noexcept { return _isTrigger; }

        /**
        * @brief SetIsTrigger is a method that replaces the current trigger state of the collider
        * with the trigger state given in parameter.
        * @param isTrigger The new trigger state for the collider.
        */
        constexpr void SetIsTrigger(const bool isTrigger) noexcept { _isTrigger = isTrigger; }

        /**
         * @brief Enabled is a method that checks if the collider is valid (aka if it has a mathematical shape).
         * @return True if the collider is valid.
         */
        [[nodiscard]] constexpr bool Enabled() const noexcept { return _enabled; }

        /**
        * @brief SetEnabled is a method that replaces the current enabled state of the collider with the new enabled
        * state given in parameter.
        * @param enabled Whether the collider is enabled or not.
        */
        constexpr void SetEnabled(const bool enabled) noexcept { _enabled = enabled; }
    };

    /**
     * @brief ColliderPair is a class that represents a pair of colliders for collision detection.
     */
    struct ColliderPair
    {
        ColliderRef ColliderA;
        ColliderRef ColliderB;

        bool operator==(const ColliderPair& other) const noexcept
        {
            return ColliderA == other.ColliderA && ColliderB == other.ColliderB ||
                   ColliderA == other.ColliderB && ColliderB == other.ColliderA;
        }

        bool operator<(const ColliderPair& other) const noexcept
        {
            return ColliderA < other.ColliderA || (ColliderA == other.ColliderA && ColliderB < other.ColliderB);
        }
    };

    /**
      * @brief ColliderHash is a custom hash function for the 'ColliderPair' objects 
      * used as keys in unordered containers.
      */
    struct ColliderHash
    {
        /**
         * @brief A custom hash operator that calculates the hash value for a `ColliderPair`.
         * @param pair The `ColliderPair` object for which to calculate the hash.
         * @return The hash value for the `ColliderPair`.
         */
        std::size_t operator()(const ColliderPair& pair) const noexcept
        {
            // Calculate the hash for the ColliderA's index.
            const std::size_t hash1 = std::hash<int>{}(static_cast<int>(pair.ColliderA.Index));

            // Calculate the hash for the ColliderB's index.
            const std::size_t hash2 = std::hash<int>{}(static_cast<int>(pair.ColliderB.Index));

            // Combine the two hash values to create a unique hash value for the ColliderPair.
            return hash1 + hash2;
        }
    };
}