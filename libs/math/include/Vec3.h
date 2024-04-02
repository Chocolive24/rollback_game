#pragma once

/**
 * @author Rémy
 */

#include "Angle.h"
#include "Utility.h"
#include "Definition.h"

#include <cmath>

namespace Math
{
    template<class T>
    class Vec3
    {
    public :
        T X = 0;
        T Y = 0;
        T Z = 0;

        constexpr Vec3(T x, T y, T z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        constexpr Vec3() = default;

        constexpr static Vec3<T> Zero()
        { return Vec3<T>(0, 0, 0); }
        constexpr static Vec3<T> One()
        { return Vec3<T>(1, 1, 1); }
        constexpr static Vec3<T> Up()
        { return Vec3<T>(0, 1, 0); }
        constexpr static Vec3<T> Down()
        { return Vec3<T>(0, -1, 0); }
        constexpr static Vec3<T> Left()
        { return Vec3<T>(-1, 0, 0); }
        constexpr static Vec3<T> Right()
        { return Vec3<T>(1, 0, 0); }
        constexpr static Vec3<T> Forward()
        { return Vec3<T>(0, 0, 1); }
        constexpr static Vec3<T> Back()
        { return Vec3<T>(0, 0, -1); }

#pragma region operator override

        [[nodiscard]] NOALIAS constexpr Vec3<T> operator+(const Vec3<T> vec3Prime) const noexcept
        {
            return Vec3<T>(X + vec3Prime.X, Y + vec3Prime.Y, Z + vec3Prime.Z);
        }

        [[nodiscard]] NOALIAS constexpr Vec3<T> operator-(const Vec3<T> vec3Prime) const noexcept
        {
            return Vec3<T>(X - vec3Prime.X, Y - vec3Prime.Y, Z - vec3Prime.Z);
        }

	    [[nodiscard]] NOALIAS constexpr Vec3<T> operator-() const noexcept
        {
            return Vec3<T>(-X, -Y, -Z);
        }

        Vec3<T> operator+=(const Vec3<T> vec3Prime) noexcept
        {
            X += vec3Prime.X;
            Y += vec3Prime.Y;
            Z += vec3Prime.Z;
            return *this;
        }

        Vec3<T> operator-=(const Vec3<T> vec3Prime) noexcept
        {
            X -= vec3Prime.X;
            Y -= vec3Prime.Y;
            Z -= vec3Prime.Z;
            return *this;
        }

        [[nodiscard]] NOALIAS constexpr Vec3<T> operator*(T scalar) const noexcept
        {
            return Vec3<T>(X * scalar, Y * scalar, Z * scalar);
        }

	    [[nodiscard]] NOALIAS constexpr friend Vec3<T> operator*(T scalar, Vec3<T> v) noexcept
        {
            return v * scalar;
        }

        [[nodiscard]] NOALIAS constexpr Vec3<T> operator/(const T& scalar) const noexcept
        {
            return Vec3<T>(X / scalar, Y / scalar, Z / scalar);
        }

        Vec3<T> operator*=(const T& scalar) noexcept
        {
            X *= scalar;
            Y *= scalar;
            Z *= scalar;

            return *this;
        }

        Vec3<T> operator/=(const T scalar) noexcept
        {
            X /= scalar;
            Y /= scalar;
            Z /= scalar;

            return *this;
        }

        constexpr bool operator==(const Vec3<T> vec3) const noexcept
        {
            return X == vec3.X && Y == vec3.Y && Z == vec3.Z;
        }

        constexpr bool operator!=(const Vec3<T> vec3) const noexcept
        {
            return X != vec3.X || Y != vec3.Y || Z != vec3.Z;
        }

	    [[nodiscard]] NOALIAS T& operator[](const int index)
        {
            if (index == 0)
            {
                return X;
            }
            else if (index == 1)
            {
                return Y;
            }
            else if (index == 2)
            {
                return Z;
            }
            else
            {
                throw std::out_of_range("Index out of range");
            }
        }

	    [[nodiscard]] NOALIAS T operator[](const int index) const
        {
            if (index == 0)
            {
                return X;
            }
            else if (index == 1)
            {
                return Y;
            }
            else if (index == 2)
            {
                return Z;
            }
            else
            {
                throw std::out_of_range("Index out of range");
            }
        }

        template<typename U>
        constexpr explicit operator Vec3<U>() const noexcept
        {
            return Vec3<U>(static_cast<U>(X), static_cast<U>(Y), static_cast<U>(Z));
        }

#pragma endregion


#pragma region utility function

        [[nodiscard]] NOALIAS constexpr static float Dot(const Vec3<T> vec3A, const Vec3<T> vec3B) noexcept
        {
            return vec3A.X * vec3B.X + vec3A.Y * vec3B.Y + vec3A.Z * vec3B.Z;
        }

        template<typename U>
        [[nodiscard]] NOALIAS constexpr U Length() const noexcept
        {
            return static_cast<U> (std::sqrt(X * X + Y * Y + Z * Z));
        }

        template<typename U>
        [[nodiscard]] NOALIAS U Project(const Vec3<T> vec3) const noexcept
        {
            return (Dot(*this, vec3) / vec3.Length<U>());
        }

        [[nodiscard]] NOALIAS Vec3<T> Reflect(const Vec3<T> vec3) const noexcept
        {
            Vec3<T> currentVec(X, Y, Z);
            Vec3<T> normalizedVec = Normalized(vec3);
            float calculation = 2 * (Dot(currentVec, normalizedVec));
            Vec3<T> calculationVec = normalizedVec * calculation;
            return currentVec - calculationVec;
        }

        [[nodiscard]] static Vec3<T> Normalized(const Vec3<T> vec3)
        {
            auto length = vec3.Length<T>();

            if (length == 0)
            {
                throw DivisionByZeroException();
            }

            return vec3 / length;
        }

	    [[nodiscard]] NOALIAS constexpr static bool IsPerpendicular(const Vec3<T> vec3A, const Vec3<T> vec3B) noexcept
        {
            return Dot(vec3A, vec3B) == 0;
        }

	    [[nodiscard]] NOALIAS constexpr static bool IsParallel(const Vec3<T> vec3A, const Vec3<T> vec3B) noexcept
        {
            return Dot(vec3A, vec3B) == 0;
        }

        [[nodiscard]] NOALIAS constexpr static Vec3<T> Lerp(const Vec3<T> vec3A, const Vec3<T> vec3B, const float time) noexcept
        {
            return vec3A + (vec3B - vec3A) * time;
        }

	    [[nodiscard]] NOALIAS static Radian GetVectorAngle(const Vec3<T> vecA, const Vec3<T> vecB) noexcept
        {
            return (Radian) std::acos(Dot(vecA, vecB) / (vecA.Length<T>() * vecB.Length<T>()));
        }

        [[nodiscard]] NOALIAS constexpr static Vec3<T> Slerp(const Vec3<T> vec3A, const Vec3<T> vec3B, const float time) noexcept
        {
            Radian omega = GetVectorAngle(vec3A, vec3B);
            T calculation = (Sin(omega * (1 - time)) / Sin(omega));
            Vec3<T> vec = Vec3<T>(vec3A.X * calculation, vec3A.Y * calculation, vec3A.Z * calculation);

            float cal = Sin(omega * time) / Sin(omega);
            vec = vec + vec3B * cal;

            return vec;
        }

        template<typename U>
        [[nodiscard]] NOALIAS static U Distance(Vec3<T> vec3A, Vec3<T> vec3B) noexcept
        {
            return static_cast<U> ((vec3A - vec3B).template Length<U>());
        }

        [[nodiscard]] NOALIAS constexpr static Vec3<T> CrossProduct(const Vec3<T> vec3A, const Vec3<T> vec3B) noexcept
        {
            Vec3<T> vecCP;
            vecCP.X = vec3A.Y * vec3B.Z - vec3A.Z * vec3B.Y;
            vecCP.Y = vec3A.Z * vec3B.X - vec3A.X * vec3B.Z;
            vecCP.Z = vec3A.X * vec3B.Y - vec3A.Y * vec3B.X;
            return vecCP;
        }

#pragma endregion

    };

    using Vec3I = Vec3<int>;
    using Vec3F = Vec3<float>;
}