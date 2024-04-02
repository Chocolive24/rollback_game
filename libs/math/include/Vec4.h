#pragma once

/**
 * @author Alexis, Olivier, Constantin, Rémy
 */

#include <cmath>
#include "Angle.h"
#include "Definition.h"

namespace Math
{
	template<class T>
	class Vec4
	{
	public:
		constexpr Vec4() = default;
		constexpr Vec4(T x, T y, T z, T w)
		{
			X = x;
			Y = y;
			Z = z;
			W = w;
		}

		T X { 0 };
		T Y { 0 };
		T Z { 0 };
		T W { 0 };

		constexpr static Vec4<T> Zero() noexcept { return Vec4<T>(0, 0, 0, 0); }
		constexpr static Vec4<T> One() noexcept { return Vec4<T>(1, 1, 1, 1); }
		constexpr static Vec4<T> Up() noexcept { return Vec4<T>(0, 1, 0, 0); }
		constexpr static Vec4<T> Down() noexcept { return Vec4<T>(0, -1, 0, 0); }
		constexpr static Vec4<T> Left() noexcept { return Vec4<T>(-1, 0, 0, 0); }
		constexpr static Vec4<T> Right() noexcept { return Vec4<T>(1, 0, 0, 0); }
		constexpr static Vec4<T> Forward() noexcept { return Vec4<T>(0, 0, 1, 0); }
		constexpr static Vec4<T> Backward() noexcept { return Vec4<T>(0, 0, -1, 0); }

#pragma region Operators

		[[nodiscard]] NOALIAS constexpr Vec4<T> operator+(const Vec4<T> vec) const noexcept
		{
			return Vec4<T>(X + vec.X, Y + vec.Y, Z + vec.Z, W + vec.W);
		}

		[[nodiscard]] NOALIAS constexpr Vec4<T> operator-(const Vec4<T> vec) const noexcept
		{
			return Vec4<T>(X - vec.X, Y - vec.Y, Z - vec.Z, W - vec.W);
		}

		[[nodiscard]] NOALIAS constexpr Vec4<T> operator-() const noexcept
		{
			return Vec4<T>(-X, -Y, -Z, -W);
		}

		constexpr Vec4<T> operator+=(const Vec4<T> vec) noexcept
		{
			X += vec.X;
			Y += vec.Y;
			Z += vec.Z;
			W += vec.W;

			return *this;
		}

		constexpr Vec4<T> operator-=(const Vec4<T> vec) noexcept
		{
			X -= vec.X;
			Y -= vec.Y;
			Z -= vec.Z;
			W -= vec.W;

			return *this;
		}

		[[nodiscard]] NOALIAS constexpr Vec4<T> operator*(const T scalar) const noexcept
		{
			return Vec4<T>(X * scalar, Y * scalar, Z * scalar, W * scalar);
		}

		[[nodiscard]] constexpr Vec4<T> operator/(const T scalar) const
		{
			if (scalar == 0)
			{
				throw DivisionByZeroException();
			}

			return Vec4<T>(X / scalar, Y / scalar, Z / scalar, W / scalar);
		}

		constexpr Vec4<T> operator*=(const T scalar) noexcept
		{
			X *= scalar;
			Y *= scalar;
			Z *= scalar;
			W *= scalar;

			return *this;
		}

		constexpr Vec4<T> operator/=(const T scalar)
		{
			if (scalar == 0)
			{
				throw DivisionByZeroException();
			}

			X /= scalar;
			Y /= scalar;
			Z /= scalar;
			W /= scalar;

			return *this;
		}

		[[nodiscard]] NOALIAS constexpr Vec4<T> operator*(const Vec4<T> vec) const noexcept
		{
			return Vec4<T>(X * vec.X, Y * vec.Y, Z * vec.Z, W * vec.W);
		}

		[[nodiscard]] constexpr Vec4<T> operator/(const Vec4<T> vec) const
		{
			if (vec.X == 0 || vec.Y == 0 || vec.Z == 0 || vec.W == 0)
			{
				throw DivisionByZeroException();
			}

			return Vec4<T>(X / vec.X, Y / vec.Y, Z / vec.Z, W / vec.W);
		}

		constexpr Vec4<T> operator*=(const Vec4<T> vec) noexcept
		{
			X *= vec.X;
			Y *= vec.Y;
			Z *= vec.Z;
			W *= vec.W;

			return *this;
		}

		constexpr Vec4<T> operator/=(const Vec4<T> vec)
		{
			if (vec.X == 0 || vec.Y == 0 || vec.Z == 0 || vec.W == 0)
			{
				throw DivisionByZeroException();
			}

			X /= vec.X;
			Y /= vec.Y;
			Z /= vec.Z;
			W /= vec.W;

			return *this;
		}

		constexpr bool operator==(const Vec4<T> vec) const noexcept
		{
			return X == vec.X && Y == vec.Y && Z == vec.Z && W == vec.W;
		}

		constexpr bool operator!=(const Vec4<T> vec) const noexcept
		{
			return !operator==(vec);
		}

		[[nodiscard]] constexpr T operator[](const int index) const
		{
			switch (index)
			{
				case 0: return X;
				case 1: return Y;
				case 2: return Z;
				case 3: return W;
				default: throw OutOfRangeException();
			}
		}

		[[nodiscard]] constexpr T& operator[](const int index)
		{
			switch (index)
			{
				case 0: return X;
				case 1: return Y;
				case 2: return Z;
				case 3: return W;
				default: throw OutOfRangeException();
			}
		}

		template<typename U>
		explicit operator Vec4<U>() const noexcept
		{
			return Vec4<U>(static_cast<U>(X), static_cast<U>(Y), static_cast<U>(Z), static_cast<U>(W));
		}

		[[nodiscard]] NOALIAS constexpr friend Vec4<T> operator*(const T scalar, const Vec4<T> vec) noexcept
		{
			return vec * scalar;
		}

#pragma endregion

		[[nodiscard]] constexpr static Vec4<T> Abs(Vec4<T> vec) noexcept
		{
			return Vec4<T>(std::abs(vec.X), std::abs(vec.Y), std::abs(vec.Z), std::abs(vec.W));
		}

		[[nodiscard]] NOALIAS constexpr static T Dot(const Vec4<T> vecA, const Vec4<T> vecB) noexcept
		{
			return vecA.Dot(vecB);
		}

		[[nodiscard]] constexpr T Dot(const Vec4<T> vec) const noexcept
		{
			return X * vec.X + Y * vec.Y + Z * vec.Z + W * vec.W;
		}

		template<typename U = T>
		[[nodiscard]] NOALIAS U Length() const noexcept
		{
			return static_cast<U>(std::sqrt(X * X + Y * Y + Z * Z + W * W));
		}

		template<typename U = T>
		[[nodiscard]] NOALIAS constexpr static Vec4<U> Lerp(const Vec4<T> vecA, const Vec4<T> vecB, float t) noexcept
		{
			const auto vec1 = static_cast<Vec4<U>>(vecA);
			const auto vec2 = static_cast<Vec4<U>>(vecB);

			return vec1 + (vec2 - vec1) * t;
		}

		template<typename U = T>
		[[nodiscard]] NOALIAS constexpr Vec4<U> Lerp(const Vec4<T> vec, float t) const noexcept
		{
			return Lerp(*this, vec, t);
		}

		[[nodiscard]] NOALIAS static float Distance(const Vec4<T> vecA, const Vec4<T> vecB) noexcept
		{
			return (vecA - vecB).Length();
		}

		[[nodiscard]] NOALIAS float Distance(const Vec4<T> vec) const noexcept
		{
			return Distance(*this, vec);
		}
	};

	using Vec4F = Vec4<float>;
	using Vec4I = Vec4<int>;
}