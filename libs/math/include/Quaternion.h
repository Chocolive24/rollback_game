/**
 * @author Olivier
 */

#pragma once

#include "Angle.h"
#include "Definition.h"
#include "Exception.h"
#include "Vec3.h"
#include "Utility.h"

namespace Math
{
	template<typename T>
	class Quaternion
	{
	public:
		// Set to the identity Quaternion by default.
		T W{1};
		Vec3<T> V{0, 0, 0};

		constexpr Quaternion() noexcept = default;

		/**
		 * @note W is the real part, while X, Y and Z are the complex (imaginary) part
		 * that is stored in a vector called v.
		 */
		constexpr Quaternion(T w, T x, T y, T z) noexcept
		{
			W = w;
			V = Vec3<T>(x, y, z);
		}

		[[nodiscard]] NOALIAS constexpr static Quaternion<T> Identity() noexcept
		{ return Quaternion(1, 0, 0, 0); }

		[[nodiscard]] NOALIAS constexpr Quaternion<T> operator*(const Quaternion<T> q) const noexcept
		{
			Quaternion<T> qMultiplied;

			// Calculate the scalar part (w1w0 - v1·v0).
			// note : v0 * v1 is for q0q1. v1 * v0 is for q1q0. Here it is q0q1.
			qMultiplied.W = (W * q.W) - Vec3<T>::Dot(V, q.V);

			// Calculate the vector part (w1v0 + w0v1 + v0 x v1).
			// note : v0 x v1 is for q0q1. v1 x v0 is for q1q0. Here it is q0q1.
			qMultiplied.V = (q.W * V) + (W * q.V) + Vec3<T>::CrossProduct(V, q.V);

			return qMultiplied;
		}

		constexpr Quaternion<T>& operator*=(const Quaternion<T> q) noexcept
		{
			return *this = *this * q;
		}

		[[nodiscard]] NOALIAS constexpr Vec3<T> operator*(const Vec3<T> p) const noexcept
		{
			Quaternion<T> qNorm = Normalized<T>();

			// Calculate the rotated vector using the quaternion rotation formula.
			// p' = qpq^-1 = p + 2w(v x p) + 2(v x (v x p)).
			return p + 2 * qNorm.W * Vec3<T>::CrossProduct(qNorm.V, p) + 2 *
			                                                             Vec3<T>::CrossProduct(qNorm.V, Vec3<T>::CrossProduct(qNorm.V, p));
		}

		[[nodiscard]] NOALIAS constexpr friend Vec3<T> operator*(const Vec3<T> p, const Quaternion<T> q) noexcept
		{
			return q * p;
		}

		constexpr friend Vec3<T>& operator*=(Vec3<T>& p, const Quaternion<T> q) noexcept
		{
			Quaternion<T> qNorm = q.Normalized<T>();

			return p = p + 2 * qNorm.W * Vec3<T>::CrossProduct(qNorm.V, p) + 2 *
			                                                                 Vec3<T>::CrossProduct(qNorm.V, Vec3<T>::CrossProduct(qNorm.V, p));
		}

		template<typename U>
		NOALIAS constexpr explicit operator Quaternion<U>() const noexcept
		{
			Quaternion<U> castedQuaternion;

			castedQuaternion.W = static_cast<U>(W);
			castedQuaternion.V.X = static_cast<U>(V.X);
			castedQuaternion.V.Y = static_cast<U>(V.Y);
			castedQuaternion.V.Z = static_cast<U>(V.Z);

			return castedQuaternion;
		}

		/**
		 * @brief Magnitude is a method that calculates the magnitude of the quaternion.
		 * @return The magnitude of type U.
		 */
		template<typename U>
		[[nodiscard]] NOALIAS U Magnitude() const noexcept
		{
			return std::sqrt(W * W + V.X * V.X + V.Y * V.Y + V.Z * V.Z);
		}

		/**
		 * @brief Normalized is a method that creates a quaternion of magnitude 1.
		 * @return A type U normalized copy of the quaternion.
		 */
		template<typename U>
		[[nodiscard]] NOALIAS Quaternion<U> Normalized() const
		{
			const U magnitude = Magnitude<U>();

			if (magnitude == 0)
			{
				throw DivisionByZeroException();
			}

			const U invMagnitude = 1 / magnitude; // * inverse = division.

			return Quaternion<U>(W * invMagnitude, V.X * invMagnitude, V.Y * invMagnitude, V.Z * invMagnitude);
		}

		/**
		 * @brief In-place normalization.
		 */
		template<typename U>
		void Normalize()
		{
			*this = Normalized<U>();
		}

		/**
		 * @brief AngleAxis is a method that creates a quaternion rotation which rotates angle degrees around axis.
		 * @return A quaternion rotation which rotates angle degrees around axis of type U.
		 */
		template<typename U>
		[[nodiscard]] NOALIAS constexpr static Quaternion<U> AngleAxis(const Radian angle, const Vec3<U> axis)
		{
			Quaternion<U> q;

			q.W = Cos(angle / 2.f);
			q.V = Vec3<U>::Normalized(axis) * Sin(angle / 2.f);

			return q;
		}

		/**
		 * @brief Euler is a method that creates a rotation around Z, Y and X axis according to the Euler angles provided.
		 * @note The rotation is applied in the Z-Y-X order.
		 * @return A rotation that rotates z degrees around Z axis, y degrees around Y axis
		 * and x degrees around X axis.
		 */
		template<typename U>
		[[nodiscard]] NOALIAS constexpr static Quaternion<U> Euler(const Radian x, const Radian y, const Radian z) noexcept
		{
			float cosX = Cos(x * 0.5f);
			float sinX = Sin(x * 0.5f);
			float cosY = Cos(y * 0.5f);
			float sinY = Sin(y * 0.5f);
			float cosZ = Cos(z * 0.5f);
			float sinZ = Sin(z * 0.5f);

			Quaternion<U> q;
			q.W = cosX * cosY * cosZ + sinX * sinY * sinZ;
			q.V.X = sinX * cosY * cosZ - cosX * sinY * sinZ;
			q.V.Y = cosX * sinY * cosZ + sinX * cosY * sinZ;
			q.V.Z = cosX * cosY * sinZ - sinX * sinY * cosZ;

			return q;
		}
	};

	using QuaternionI = Quaternion<int>;
	using QuaternionF = Quaternion<float>;
}