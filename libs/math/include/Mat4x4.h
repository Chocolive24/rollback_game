#pragma once

/**
 * @author Olivier, Alexis, Rémy, Constantin
 */

#include "Exception.h"
#include "Mat3x3.h"
#include "Vec4.h"

namespace Math
{
    template<class T>
    class Mat4x4
    {
    public:
        constexpr static std::size_t RowNbr = 4;
        constexpr static std::size_t ColNbr = 4;

        // Set the matrix to the 4x4 identity by default.
        T Val[RowNbr][ColNbr]{{ 1, 0, 0, 0 },
                              { 0, 1, 0, 0 },
                              { 0, 0, 1, 0 },
                              { 0, 0, 0, 1 }};

        constexpr Mat4x4() noexcept = default;

        /**
         * @note Vectors are added in column major order.
         */
        constexpr Mat4x4(Vec4<T> a, Vec4<T> b, Vec4<T> c, Vec4<T> d) noexcept
        {
            for (std::size_t row = 0; row < RowNbr; row++)
            {
                Val[row][0] = a[row];
                Val[row][1] = b[row];
                Val[row][2] = c[row];
                Val[row][3] = d[row];
            }
        }

        [[nodiscard]] NOALIAS constexpr static Mat4x4<T> Identity() noexcept
        {
            return Mat4x4<T>(Vec4<T>(1, 0, 0, 0), Vec4<T>(0, 1, 0, 0), Vec4<T>(0, 0, 1, 0), Vec4<T>(0, 0, 0, 1));
        }

        [[nodiscard]] NOALIAS constexpr Mat4x4<T> operator+(const Mat4x4<T>& m) const noexcept
        {
            Mat4x4<T> mResult;

            for (std::size_t row = 0; row < RowNbr; row++)
            {
                for (std::size_t col = 0; col < ColNbr; col++)
                {
                    mResult.Val[row][col] = Val[row][col] + m.Val[row][col];
                }
            }

            return mResult;
        }

        constexpr Mat4x4<T>& operator+=(const Mat4x4<T>& m) noexcept
        {
            return *this = *this + m;
        }

        NOALIAS constexpr Mat4x4<T> operator-(const Mat4x4<T>& m) const noexcept
        {
            Mat4x4<T> mResult;

            for (std::size_t row = 0; row < RowNbr; row++)
            {
                for (std::size_t col = 0; col < ColNbr; col++)
                {
                    mResult.Val[row][col] = Val[row][col] - m.Val[row][col];
                }
            }

            return mResult;
        }

        constexpr Mat4x4<T>& operator-=(const Mat4x4<T>& m) noexcept
        {
            return *this = *this - m;
        }

        [[nodiscard]] NOALIAS constexpr Mat4x4<T> operator*(const T scalar) const noexcept
        {
            Mat4x4<T> mResult;

            for (std::size_t row = 0; row < RowNbr; row++)
            {
                for (std::size_t col = 0; col < ColNbr; col++)
                {
                    mResult.Val[row][col] = Val[row][col] * scalar;
                }
            }

            return mResult;
        }

        [[nodiscard]] NOALIAS constexpr friend Mat4x4<T> operator*(const T scalar, const Mat4x4<T>& m) noexcept
        {
            return m * scalar;
        }

        constexpr Mat4x4<T>& operator*=(const T scalar) noexcept
        {
            return *this = *this * scalar;
        }

        [[nodiscard]] NOALIAS constexpr Mat4x4<T> operator*(const Mat4x4<T>& m) const noexcept
        {
            Mat4x4<T> mResult;

            for (std::size_t row = 0; row < RowNbr; row++)
            {
                for (std::size_t col = 0; col < ColNbr; col++)
                {
                    T sum = 0;

                    for (std::size_t k = 0; k < 4; k++)
                    {
                        sum += Val[row][k] * m.Val[k][col];
                    }

                    mResult.Val[row][col] = sum;
                }
            }

            return mResult;
        }

        constexpr Mat4x4<T>& operator*=(const Mat4x4<T>& m) noexcept
        {
            return *this = *this * m;
        }

        [[nodiscard]] NOALIAS constexpr Vec4<T> operator*(const Vec4<T> v) const noexcept
        {
            Vec4<T> vResult;

            for (std::size_t row = 0; row < RowNbr; row++)
            {
                T internResult = 0;

                for (std::size_t j = 0; j < RowNbr; j++)
                {
                    internResult += Val[row][j] * v[j];
                }

                vResult[row] = internResult;
            }

            return vResult;
        }

        template<typename U>
        NOALIAS constexpr explicit operator Mat4x4<U>() const noexcept
        {
            Mat4x4<U> castedMatrix;

            for (std::size_t row = 0; row < 4; row++)
            {
                for (std::size_t col = 0; col < 4; col++)
                {
                    castedMatrix.Val[row][col] = static_cast<U>(Val[row][col]);
                }
            }

            return castedMatrix;
        }

        /**
         * @brief Det is a method that calculates the determinant of the matrix.
         * @return Determinant of type T
         */
        [[nodiscard]] NOALIAS constexpr T Det() const noexcept
        {
            T detA = 0;

            for (std::size_t col = 0; col < ColNbr; col++)
            {
                T sign = (1 + (col + 1)) % 2 == 0 ? 1 : -1;

                Mat3x3<T> subMat;
                std::size_t subMatRow = 0, subMatCol = 0;

                for (std::size_t subRow = 0; subRow < RowNbr; subRow++)
                {
                    if (subRow == 0) continue;

                    for (std::size_t subCol = 0; subCol < ColNbr; subCol++)
                    {
                        if (subCol == col) continue;

                        // Add the value to the sub-matrix.
                        subMat.Val[subMatRow][subMatCol] = Val[subRow][subCol];
                        subMatCol++;
                    }

                    subMatRow++;
                    subMatCol = 0;
                }

                // Det(A) = sum of all : (-1)^1+i * a1i * det(A1i)
                detA += sign * Val[0][col] * subMat.Det();
            }

            return detA;
        }

        /**
         * @brief Transposed is a method that creates a transposed copy of the matrix.
         * @return A transposed copy matrix of type T.
         */
        [[nodiscard]] NOALIAS constexpr Mat4x4<T> Transposed() const noexcept
        {
            Mat4x4<T> transposed;

            for (std::size_t row = 0; row < RowNbr; row++)
            {
                for (std::size_t col = 0; col < ColNbr; col++)
                {
                    transposed.Val[row][col] = Val[col][row];
                }
            }

            return transposed;
        }

        /**
         * @brief In-place transposition.
         */
        constexpr void Transpose() noexcept
        {
            *this = Transposed();
        }

        /**
         * @brief Inverted is a method that creates an inverted copy of the matrix.
         * @return An inverted copy matrix of type T.
         */
        template<typename U>
        [[nodiscard]] NOALIAS constexpr Mat4x4<U> Inverted() const
        {
            if (Det() == 0)
            {
                throw DivisionByZeroException();
            }

            // Sum of determinants of all sub-matrices * (-1)^i+j
            Mat4x4<U> coFactorMatrix;

            for (std::size_t row = 0; row < RowNbr; row++)
            {
                for (std::size_t col = 0; col < ColNbr; col++)
                {
                    T sign = (col + row) % 2 == 0 ? 1 : -1;

                    Mat3x3<U> subMat;
                    std::size_t subMatRow = 0, subMatCol = 0;

                    for (std::size_t subRow = 0; subRow < RowNbr; subRow++)
                    {
                        if (subRow == row) continue;

                        for (std::size_t subCol = 0; subCol < ColNbr; ++subCol)
                        {
                            if (subCol == col) continue;

                            // Add the value to the sub-matrix.
                            subMat.Val[subMatRow][subMatCol] = Val[subRow][subCol];
                            subMatCol++;
                        }

                        subMatRow++;
                        subMatCol = 0;
                    }

                    coFactorMatrix.Val[row][col] = subMat.Det() * sign; // Det(Mij) * (-1)^i+j
                }
            }

            Mat4x4<U> inverted = coFactorMatrix.Transposed() * (static_cast<U>(1 / Det()));

            return inverted;
        }

        /**
         * @brief In-place inversion
         */
        template<typename U>
        constexpr void Invert()
        {
            *this = Inverted<U>();
        }
    };

    using Mat4x4I = Mat4x4<int>;
    using Mat4x4F = Mat4x4<float>;
}