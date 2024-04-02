#pragma once

/**
 * @headerfile A collection of classes and functions for working with 2D vectors.
 * @author Alexis
 */

#include "Angle.h"
#include "Exception.h"
#include "Utility.h"
#include "Definition.h"

#include <cmath>

namespace Math
{
    template<typename T>
    class Vec2
    {
    public:
	    constexpr Vec2() = default;
	    constexpr Vec2(T x, T y)
        {
            X = x;
            Y = y;
        }

        T X { 0 };
        T Y { 0 };

        constexpr static Vec2<T> Zero() noexcept { return Vec2<T>(0, 0); }
        constexpr static Vec2<T> One() noexcept { return Vec2<T>(1, 1); }
        constexpr static Vec2<T> Up() noexcept { return Vec2<T>(0, 1); }
        constexpr static Vec2<T> Down() noexcept { return Vec2<T>(0, -1); }
        constexpr static Vec2<T> Left() noexcept { return Vec2<T>(-1, 0); }
        constexpr static Vec2<T> Right() noexcept { return Vec2<T>(1, 0); }

#pragma region Operators

        [[nodiscard]] NOALIAS constexpr Vec2<T> operator+(const Vec2<T> vec) const noexcept
        {
            return Vec2<T>(X + vec.X, Y + vec.Y);
        }

        [[nodiscard]] NOALIAS constexpr Vec2<T> operator-(const Vec2<T> vec) const noexcept
        {
            return Vec2<T>(X - vec.X, Y - vec.Y);
        }

        [[nodiscard]] NOALIAS constexpr Vec2<T> operator-() const noexcept
        {
            return Vec2<T>(-X, -Y);
        }

        constexpr Vec2<T> operator+=(const Vec2<T> vec) noexcept
        {
            X += vec.X;
            Y += vec.Y;

            return *this;
        }

        constexpr Vec2<T> operator-=(const Vec2<T> vec) noexcept
        {
            X -= vec.X;
            Y -= vec.Y;

            return *this;
        }

        [[nodiscard]] NOALIAS constexpr Vec2<T> operator*(const T scalar) const noexcept
        {
            return Vec2<T>(X * scalar, Y * scalar);
        }

        [[nodiscard]] constexpr Vec2<T> operator/(const T scalar) const
        {
            if (scalar == 0)
            {
                throw DivisionByZeroException();
            }

            return Vec2<T>(X / scalar, Y / scalar);
        }

        constexpr Vec2<T> operator*=(const T scalar) noexcept
        {
            X *= scalar;
            Y *= scalar;

            return *this;
        }

        constexpr Vec2<T> operator/=(const T scalar)
        {
            if (scalar == 0)
            {
                throw DivisionByZeroException();
            }

            X /= scalar;
            Y /= scalar;

            return *this;
        }

        [[nodiscard]] NOALIAS constexpr Vec2<T> operator*(const Vec2<T> vec) const noexcept
        {
            return Vec2<T>(X * vec.X, Y * vec.Y);
        }

        [[nodiscard]] constexpr Vec2<T> operator/(const Vec2<T> vec) const
        {
            if (vec.X == 0 || vec.Y == 0)
            {
                throw DivisionByZeroException();
            }

            return Vec2<T>(X / vec.X, Y / vec.Y);
        }

        constexpr Vec2<T> operator*=(const Vec2<T> vec)
        {
            X *= vec.X;
            Y *= vec.Y;

            return *this;
        }

        constexpr Vec2<T> operator/=(const Vec2<T> vec)
        {
            if (vec.X == 0 || vec.Y == 0)
            {
                throw DivisionByZeroException();
            }

            X /= vec.X;
            Y /= vec.Y;

            return *this;
        }

        constexpr bool operator==(const Vec2<T> vec) const noexcept
        {
            return X == vec.X && Y == vec.Y;
        }

        constexpr bool operator!=(const Vec2<T>& vec) const noexcept
        {
            return !operator==(vec);
        }

        [[nodiscard]] constexpr T& operator[](const int index)
        {
            switch (index)
            {
                case 0:
                    return X;
                case 1:
                    return Y;
                default:
                    throw OutOfRangeException();
            }
        }

        template<typename U>
        explicit operator Vec2<U>() const noexcept
        {
            return { static_cast<U>(X), static_cast<U>(Y) };
        }

        constexpr friend Vec2<T> operator*(const T scalar, const Vec2<T> vec) noexcept
        {
            return vec * scalar;
        }

#pragma endregion

        [[nodiscard]] NOALIAS constexpr T Dot(const Vec2<T> vec) const noexcept
        {
            return X * vec.X + Y * vec.Y;
        }

	    [[nodiscard]] NOALIAS constexpr static T Dot(const Vec2<T> vec1, const Vec2<T> vec2) noexcept
        {
            return vec1.Dot(vec2);
        }

        template<typename U = T>
        [[nodiscard]] NOALIAS U Length() const noexcept
        {
            return static_cast<U>(std::sqrt(X * X + Y * Y));
        }

        template<typename U = T>
        [[nodiscard]] NOALIAS U SquareLength() const noexcept
        {
            return static_cast<U>(X * X + Y * Y);
        }

        // Returns a normalized copy of the vector
        template<typename U = T>
        [[nodiscard]] Vec2<U> Normalized() const
        {
            auto vec = static_cast<Vec2<U>>(*this);

            vec.template Normalize<U>();

            return vec;
        }

        // Normalizes the vector
        template<typename U = T>
        void Normalize()
        {
            U length = Length();

            if (length <= Epsilon)
            {
                throw DivisionByZeroException();
            }

            X /= length;
            Y /= length;
        }

        [[nodiscard]] NOALIAS constexpr bool IsPerpendicular(const Vec2<T> vec) const noexcept
        {
            if constexpr (std::is_floating_point_v<T>)
            {
                auto dot = Dot(vec);

                return std::abs(dot) < std::numeric_limits<T>::epsilon();
            }
            else
            {
                return Dot(*this, vec) == 0;
            }
        }

        [[nodiscard]] NOALIAS constexpr bool IsParallel(const Vec2<T> vec) const noexcept
        {
            T result = vec.X * Y - vec.Y * X;

            if constexpr (std::is_floating_point_v<T>)
            {
                return abs(result) < std::numeric_limits<T>::epsilon();
            }

            return result == 0;
        }

        template<typename U = T>
        [[nodiscard]] NOALIAS constexpr Vec2<U> Project(const Vec2<T> vec) const
        {
            auto vecA = static_cast<Vec2<U>>(*this);
            auto vecB = static_cast<Vec2<U>>(vec);
            const auto vecUnit = vecB.template Normalized<U>();
            return vecA.Dot(vecUnit) * vecUnit;
        }

        // Reflection of this vector over another vector
        template<typename U = T>
        [[nodiscard]] NOALIAS constexpr Vec2<U> Reflect(const Vec2<T> vec) const
        {
            // a - 2 * (a - b^) * b^
            auto vecA = static_cast<Vec2<U>>(*this);
            auto vecB = static_cast<Vec2<U>>(vec);
            Vec2<U> normalised = vecB.template Normalized<U>();

            return vecA - 2 * (vecA - normalised) * normalised;
        }

        // Get Angle between two vectors
        [[nodiscard]] NOALIAS static Radian Angle(const Vec2<T> vec1, const Vec2<T> vec2)
        {
            // acos((a * b) / (|a| * |b|))
            auto divisor = vec1.Length() * vec2.Length();

            if (divisor == 0)
            {
                throw DivisionByZeroException();
            }

            auto result = std::acos(vec1.Dot(vec2) / divisor);

            if (std::isnan(result))
            {
                throw NaNException();
            }

            return static_cast<Radian>(result);
        }

        // Angle between this vector and another vector
        [[nodiscard]] NOALIAS constexpr Radian Angle(const Vec2<T> vec) const
        {
            return Angle(*this, vec);
        }

        template<typename U = T>
        [[nodiscard]] NOALIAS constexpr static Vec2<U> Lerp(const Vec2<T> vec1, const Vec2<T> vec2, float t) noexcept
        {
            auto vec1u = static_cast<Vec2<U>>(vec1);
            auto vec2u = static_cast<Vec2<U>>(vec2);

            return vec1u + (vec2u - vec1u) * t;
        }

        template<typename U = T>
        [[nodiscard]] NOALIAS constexpr Vec2<U> Lerp(const Vec2<T> vec, float t) const noexcept
        {
            return Lerp<U>(*this, vec, t);
        }

        template<typename U = T>
        [[nodiscard]] NOALIAS constexpr static Vec2<U> Slerp(const Vec2<T> vec1, const Vec2<T> vec2, float t)
        {
            auto vec1u = static_cast<Vec2<U>>(vec1);
            auto vec2u = static_cast<Vec2<U>>(vec2);
            Radian angleBetween = vec1u.Angle(vec2u);

            float sinAngle = Sin(angleBetween);
            float sinAngleT = Sin(angleBetween * t);
            float sinAngle1T = Sin(angleBetween * (1 - t));

            if (sinAngle == 0)
            {
                throw DivisionByZeroException();
            }

            float tOnAngle = sinAngleT / sinAngle;
            float oneMinusTOnAngle = sinAngle1T / sinAngle;

            // sin((1 - t) * angle) / sin(angle) * a + sin(t * angle) / sin(angle) * b
            return vec1u * oneMinusTOnAngle + vec2u * tOnAngle;
        }

        template<typename U = T>
        [[nodiscard]] NOALIAS constexpr Vec2<U> Slerp(const Vec2<T> vec, float t) const
        {
            return Slerp(*this, vec, t);
        }

        [[nodiscard]] NOALIAS static float Distance(const Vec2<T> vec1, const Vec2<T> vec2) noexcept
        {
            return (vec1 - vec2).Length();
        }

        [[nodiscard]] NOALIAS float Distance(const Vec2<T> vec) const noexcept
        {
            return Distance(*this, vec);
        }

	    constexpr void Rotate(Radian angle) noexcept
        {
            const float cos = Cos(angle);
            const float sin = Sin(angle);

            T x = X * cos - Y * sin;
            T y = X * sin + Y * cos;

            X = x;
            Y = y;
        }

        [[nodiscard]] constexpr static Vec2<T> Rotate(const Vec2<T> vec, Radian angle)
        {
            Vec2<T> result = vec;

            result.Rotate(angle);

            return result;
        }
    };

    using Vec2I = Vec2<int>;
    using Vec2F = Vec2<float>;
}