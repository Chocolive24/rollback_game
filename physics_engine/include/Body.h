/**
 * @headerfile Body.h
 * This file defines the Body class that represents a physical body with attributes such as position, velocity, mass
 * and forces applied to it.
 *
 * @author Olivier Pachoud
 */

#pragma once

#include "Vec2.h"

namespace PhysicsEngine
{
    /**
     * @brief BodyType is an enumeration that represents the type of a physical body: 
     * Dynamic, Kinematic, Static, or None.
     */
    enum class BodyType
    {
        Dynamic,
        Kinematic,
        Static,
        None
    };

    /*
    * @brief The Body class represents a physical body with attributes including position, 
    * velocity, mass, and applied forces.
    */
    class Body
    {
    private:
        Math::Vec2F _position = Math::Vec2F::Zero();
        Math::Vec2F _velocity = Math::Vec2F::Zero();
        Math::Vec2F _forces = Math::Vec2F::Zero();
        Math::Vec2F _impulses = Math::Vec2F::Zero();
        float _mass = -1.f;
        float _inverseMass = -1.f;
        float _damping = 0.f;
        BodyType _bodyType = BodyType::Dynamic;

    public:
        constexpr Body() noexcept = default;
        constexpr Body(Math::Vec2F pos, Math::Vec2F vel, float mass) noexcept
        {
            _position = pos;
            _velocity = vel;
            _mass = mass;
            _inverseMass = 1.f / mass;
        }

        /**
         * @brief Position is a method that gives the position of the body.
         * @return The position of the body.
         */
        [[nodiscard]] constexpr Math::Vec2F Position() const noexcept { return _position; }

        /**
         * @brief SetPosition is a method that replaces the current position of the body with the new position
         * given in parameter.
         * @param newPosition The new position for the body.
         */
        void constexpr SetPosition(const Math::Vec2F newPosition) noexcept { _position = newPosition; }

        /**
         * @brief Velocity is a method that gives the velocity of the body.
         * @return The velocity of the body.
         */
        [[nodiscard]] const Math::Vec2F Velocity() const noexcept { return _velocity; }

        /**
         * @brief SetVelocity is a method that replaces the current velocity of the body with the new velocity
         * given in parameter.
         * @param newVelocity The new velocity for the body.
         */
        void constexpr SetVelocity(const Math::Vec2F newVelocity) noexcept { _velocity = newVelocity; }

        /**
         * @brief Mass is a method that gives the mass of the body.
         * @return The mass of the body.
         */
        [[nodiscard]] constexpr float Mass() const noexcept { return _mass; }

        /**
         * @brief SetMass is a method that replaces the current mass of the body with the new mass given in parameter.
         * It also recalculates the inverse mass of the body with the newMass.
         * @param newMass The new mass for the body.
         */
        void constexpr SetMass(const float newMass) noexcept 
        { 
            _mass = newMass; 
            _inverseMass = 1.f / _mass;
        }

        /**
         * @brief InverseMass is a method that gives the inverse mass of the body.
         * @return The inverse mass of the body.
         */
        [[nodiscard]] constexpr float InverseMass() const noexcept { return _inverseMass; }

        /**
         * @brief Damping is a method that gives the damping factor of the
         * body.
         * @return The damping factor of the body.
         */
        [[nodiscard]] constexpr float Damping() const noexcept {
            return _damping;
        }

        /**
         * @brief SetDamping is a method that replaces the current damping factor of
         * the body with the new damping factor given in parameter.
         * It also recalculates the inverse mass of the body with the newMass.
         * @param new_damping The new damping factor for the body.
         */
        void constexpr SetDamping(const float new_damping) noexcept 
        { 
            _damping = new_damping;
        }

        /**
         * @brief ApplyForce is a method that applies a force to the body and adds it to the sum of the body's forces
         * @param force The force to be applied to the body.
         */
        constexpr void ApplyForce(const Math::Vec2F force) noexcept { _forces += force; }

        /**
         * @brief Forces is a method that gives the sum of the body's forces.
         * @return The sum of the body's forces.
         */
        [[nodiscard]] constexpr Math::Vec2F Forces() const noexcept { return _forces; }

        /**
         * @brief ResetForces is a method that sets the sum of the body's forces to zero.
         */
        constexpr void ResetForces() noexcept { _forces = Math::Vec2F::Zero(); }

        /**
         * @brief ApplyImpulse is a method that applies an impulse to the body and
         * adds it to the sum of the body's impulses.
         * @param impulse The impulse to be applied to the body.
         */
        constexpr void ApplyImpulse(const Math::Vec2F impulse) noexcept {
            _impulses += impulse;
        }

        /**
         * @brief Forces is a method that gives the sum of the body's forces.
         * @return The sum of the body's forces.
         */
        [[nodiscard]] constexpr Math::Vec2F Impulses() const noexcept {
            return _impulses;
        }

        /**
         * @brief ResetForces is a method that sets the sum of the body's forces
         * to zero.
         */
        constexpr void ResetImpulses() noexcept { _impulses = Math::Vec2F::Zero(); }

        /**
         * @brief Enabled is a method that checks if the body is valid (aka if it has a mass greater than 0).
         * @return True if the body is valid.
         */
        [[nodiscard]] constexpr bool IsValid() const noexcept { return _mass > 0; }

        /**
         * @brief GetBodyType is a method that gives the body-type of the body.
         * @return The body-type of the body.
         */
        [[nodiscard]] constexpr BodyType GetBodyType() const noexcept { return _bodyType; }

        /**
         * @brief SetBodyType is a method that replaces the current body-type of the body by the new one given
         * in parameter.
         * @param newBodyType The new body-type for the body. 
         */
        constexpr void SetBodyType(const BodyType newBodyType) noexcept 
        { 
            _bodyType = newBodyType; 

            if (newBodyType == BodyType::Static || newBodyType == BodyType::Kinematic)
            {
                _inverseMass = 0.f;
            }
        }
    };
}