/**
 * @headerfile WorldRefTypes.h
 * This header file defines the reference types used to get objects in the World class.
 */

#pragma once

#include <cstddef>

namespace PhysicsEngine
{
    /**
     * @brief BodyRef is a struct used to reference a specific body.
     * @brief Attributes :
     * @brief Index : The index of the body inside the world body vector.
     * @brief GenerationIdx : The index inside the world generation number vector.
     */
    struct BodyRef
    {
        std::size_t Index;
        std::size_t GenerationIdx;

        constexpr bool operator==(const BodyRef& other) const noexcept
        {
            return Index == other.Index && GenerationIdx == other.GenerationIdx;
        }
    };

    /**
     * @brief ColliderRef is a struct used to reference a specific collider.
     * @brief Attributes :
     * @brief Index : The index of the collider inside the world colliders vector.
     * @brief GenerationIdx : The index inside the world colliders generation indices vector.
     */
    struct ColliderRef
    {
        std::size_t Index;
        std::size_t GenerationIdx;

        constexpr ColliderRef& operator=(const ColliderRef& colRef) noexcept = default;

        constexpr bool operator==(const ColliderRef& other) const noexcept
        {
            return Index == other.Index && GenerationIdx == other.GenerationIdx;
        }

        constexpr bool operator<(const ColliderRef& other) const noexcept
        {
            return Index < other.Index || (Index == other.Index && GenerationIdx < other.GenerationIdx);
        }
    };
}