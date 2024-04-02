#pragma once

/**
 * @brief Matrix 2x2 class with all needed functions
 * @author Constantin
 */

#include "Vec2.h"
#include "Definition.h"

namespace Math
{
    template<class T>
    class Mat2x2
    {
    public:
        constexpr static std::size_t RowNbr = 3;
        constexpr static std::size_t ColNbr = 3;

        T Val[RowNbr][ColNbr] = {{ 1, 0 },
                                 { 0, 1 }};

        constexpr Mat2x2() noexcept = default;

        /**
         * @note Vectors are added in column major order.
         */
        constexpr Mat2x2(Vec2<T> a, Vec2<T> b) noexcept
        {
            Val[0][0] = a.X;
            Val[0][1] = b.X;
            Val[1][0] = a.Y;
            Val[1][1] = b.Y;
        }

        [[nodiscard]] NOALIAS constexpr static Mat2x2<T> Identity() noexcept
        {
            return Mat2x2<T>({ 1, 0 }, { 0, 1 });
        }

        [[nodiscard]] NOALIAS constexpr T Det() const noexcept
        {
            return (Val[0][0] * Val[1][1]) - (Val[0][1] * Val[1][0]);
        }

        [[nodiscard]] NOALIAS constexpr Mat2x2<T> Transposed() const noexcept
        {
            return Mat2x2<T>({ Val[0][0], Val[0][1] }, { Val[1][0], Val[1][1] });
        }

        template<typename U>
        [[nodiscard]] constexpr Mat2x2<U> Inverted() const
        {
            if (Det() == 0)
            {
                throw DivisionByZeroException();
            }

            Mat2x2<T> m({ Val[1][1], Val[1][0] * -1 }, { Val[0][1] * -1, Val[0][0] });

            return static_cast<Mat2x2<U>>(m) * (static_cast<U>(1) / static_cast<U>(Det()));
        }

        [[nodiscard]] NOALIAS constexpr Mat2x2<T> operator+(const Mat2x2<T> other) const noexcept
        {
            return Mat2x2<T>(
                    { Val[0][0] + other.Val[0][0],
                      Val[1][0] + other.Val[1][0] },
                    { Val[0][1] + other.Val[0][1],
                      Val[1][1] + other.Val[1][1] }
            );
        }

        constexpr Mat2x2<T>& operator+=(const Mat2x2<T> other) noexcept
        {
            return *this = *this + other;
        }

        [[nodiscard]] NOALIAS constexpr Mat2x2<T> operator-(const Mat2x2<T> other) const noexcept
        {
            return Mat2x2<T>(
                    { Val[0][0] - other.Val[0][0],
                      Val[0][1] - other.Val[0][1] },
                    { Val[1][0] - other.Val[1][0],
                      Val[1][1] - other.Val[1][1] }
            );
        }

        constexpr Mat2x2<T>& operator-=(const Mat2x2<T> other) noexcept
        {
            return *this = *this - other;
        }

        [[nodiscard]] NOALIAS constexpr Mat2x2<T> operator*(const T scalar) const noexcept
        {
            return Mat2x2<T>({ Val[0][0] * scalar, Val[1][0] * scalar },
                             { Val[0][1] * scalar, Val[1][1] * scalar });
        }

        constexpr Mat2x2<T>& operator*=(const T scalar) noexcept
        {
            return *this = *this * scalar;
        }

        [[nodiscard]] NOALIAS constexpr Mat2x2<T> operator*(const Mat2x2<T> other) const noexcept
        {
            return Mat2x2<T>(
                    { Val[0][0] * other.Val[0][0] + Val[0][1] * other.Val[1][0],
                      Val[1][0] * other.Val[0][0] + Val[1][1] * other.Val[1][0] },
                    { Val[0][0] * other.Val[0][1] + Val[0][1] * other.Val[1][1],
                      Val[1][0] * other.Val[0][1] + Val[1][1] * other.Val[1][1] }
            );
        }

        constexpr Mat2x2<T>& operator*=(const Mat2x2<T> other) noexcept
        {
            return *this = *this * other;
        }

        [[nodiscard]] NOALIAS constexpr Vec2<T> operator*(const Vec2<T> vec) const noexcept
        {
            return Vec2<T>(
                    Val[0][0] * vec.X + Val[0][1] * vec.Y,
                    Val[1][0] * vec.X + Val[1][1] * vec.Y
            );
        }

        template<typename U>
        constexpr explicit operator Mat2x2<U>() const noexcept
        {
            return Mat2x2<U>({ static_cast<U>(Val[0][0]), static_cast<U>(Val[1][0]) },
                             { static_cast<U>(Val[0][1]), static_cast<U>(Val[1][1]) });
        }
    };

    using Mat2x2F = Mat2x2<float>;
    using Mat2x2I = Mat2x2<int>;
}