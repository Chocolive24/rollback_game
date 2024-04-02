/**
 * @headerfile ContactListener.h
 * This header file defines the ContactListener class which is an interface for 
 * handling collider collision events.
 *
 * @author Olivier Pachoud
 */

#pragma once

#include "Collider.h"
#include "Vec2.h"

namespace PhysicsEngine
{
    /**
     * @brief ContactListener is an abstract base class for handling collider collision events.
     */
    class ContactListener
    {
    public:
      virtual ~ContactListener() = default;
      /**
       * @brief OnTriggerEnter is an abstract method that is called when two colliders begin intersecting and
       * at least one of them is in trigger state.
       * @param colliderRefA The collider reference of the first collider in the world.
       * @param colliderRefB The collider reference of the second collider in the world.
       */
       virtual void OnTriggerEnter(ColliderRef colliderRefA, ColliderRef colliderRefB) noexcept = 0;

      /**
      * @brief OnTriggerStay is an abstract method that is called when two colliders are currently intersecting and
      * at least one of them is in trigger state.
      * @param colliderRefA The collider reference of the first collider in the world.
      * @param colliderRefB The collider reference of the second collider in the world.
      */
       virtual void OnTriggerStay(ColliderRef colliderRefA, ColliderRef colliderRefB) noexcept = 0;

       /**
       * @brief OnTriggerExit is an abstract method that is called when two colliders stop intersecting and
       * at least one of them is in trigger state.
       * @param colliderRefA The collider reference of the first collider in the world.
       * @param colliderRefB The collider reference of the second collider in the world.
       */
       virtual void OnTriggerExit(ColliderRef colliderRefA, ColliderRef colliderRefB) noexcept = 0;

       /**
       * @brief OnCollisionEnter is an abstract method that is called when two colliders enter in collision.
       * @param colliderRefA The collider reference of the first collider in the world.
       * @param colliderRefB The collider reference of the second collider in the world.
       */
       virtual void OnCollisionEnter(ColliderRef colliderRefA, ColliderRef colliderRefB) noexcept = 0;

       /**
      * @brief OnCollisionEnter is an abstract method that is called when two colliders stop colliding.
      * @param colliderRefA The collider reference of the first collider in the world.
      * @param colliderRefB The collider reference of the second collider in the world.
      */
       virtual void OnCollisionExit(ColliderRef colliderRefA, ColliderRef colliderRefB) noexcept = 0;
    };
}