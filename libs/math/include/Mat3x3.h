#pragma once

/**
 * @author Olivier
 */

#include "Exception.h"
#include "Mat2x2.h"
#include "Vec3.h"

namespace Math
{
    template<typename T>
    class Mat3x3
    {
    public:
        constexpr static std::size_t RowNbr = 3;
        constexpr static std::size_t ColNbr = 3;

        // Set the matrix to the 3x3 identity by default.
        T Val[RowNbr][ColNbr]{{ 1, 0, 0 },
                              { 0, 1, 0 },
                              { 0, 0, 1 }};

        constexpr Mat3x3() noexcept = default;

        /**
         * @note Vectors are added in column major order.
         */
        constexpr Mat3x3(Vec3<T> a, Vec3<T> b, Vec3<T> c) noexcept
        {
            for (std::size_t row = 0; row < RowNbr; row++)
            {
                Val[row][0] = a[row];
                Val[row][1] = b[row];
                Val[row][2] = c[row];
            }
        }

        [[nodiscard]] NOALIAS constexpr static Mat3x3<T> Identity() noexcept
        {
            return Mat3x3<T>(Vec3<T>(1, 0, 0), Vec3<T>(0, 1, 0), Vec3<T>(0, 0, 1));
        }

        [[nodiscard]] NOALIAS constexpr Mat3x3<T> operator+(const Mat3x3<T>& m) const noexcept
        {
            Mat3x3<T> mResult;

            for (std::size_t row = 0; row < RowNbr; row++)
            {
                for (std::size_t col = 0; col < ColNbr; col++)
                {
                    mResult.Val[row][col] = Val[row][col] + m.Val[row][col];
                }
            }

            return mResult;
        }

        constexpr Mat3x3<T>& operator+=(const Mat3x3<T>& m) noexcept
        {
            return *this = *this + m;
        }

        [[nodiscard]] NOALIAS constexpr Mat3x3<T> operator-(const Mat3x3<T>& m) const noexcept
        {
            Mat3x3<T> mResult;

            for (std::size_t row = 0; row < RowNbr; row++)
            {
                for (std::size_t col = 0; col < ColNbr; col++)
                {
                    mResult.Val[row][col] = Val[row][col] - m.Val[row][col];
                }
            }

            return mResult;
        }

        constexpr Mat3x3<T>& operator-=(const Mat3x3<T>& m) noexcept
        {
            return *this = *this - m;
        }

        [[nodiscard]] NOALIAS constexpr Mat3x3<T> operator*(const T scalar) const noexcept
        {
            Mat3x3<T> mResult;

            for (std::size_t row = 0; row < RowNbr; row++)
            {
                for (std::size_t col = 0; col < ColNbr; col++)
                {
                    mResult.Val[row][col] = Val[row][col] * scalar;
                }
            }

            return mResult;
        }

        [[nodiscard]] NOALIAS constexpr friend Mat3x3<T> operator*(const T scalar, const Mat3x3<T>& m) noexcept
        {
            return m * scalar;
        }

        constexpr Mat3x3<T>& operator*=(const T scalar) noexcept
        {
            return *this = *this * scalar;
        }

        [[nodiscard]] NOALIAS constexpr Mat3x3<T> operator*(const Mat3x3<T>& m) const noexcept
        {
            Mat3x3<T> mResult;

            for (std::size_t row = 0; row < RowNbr; row++)
            {
                for (std::size_t col = 0; col < ColNbr; col++)
                {
                    T sum = 0;

                    for (std::size_t k = 0; k < 3; k++)
                    {
                        sum += Val[row][k] * m.Val[k][col];
                    }

                    mResult.Val[row][col] = sum;
                }
            }

            return mResult;
        }

        constexpr Mat3x3<T>& operator*=(const Mat3x3<T>& m) noexcept
        {
            return *this = *this * m;
        }

        [[nodiscard]] NOALIAS constexpr Vec3<T> operator*(const Vec3<T> v) const noexcept
        {
            Vec3<T> vResult;

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
        NOALIAS constexpr explicit operator Mat3x3<U>() const noexcept
        {
            Mat3x3<U> castedMatrix;

            for (std::size_t row = 0; row < RowNbr; row++)
            {
                for (std::size_t col = 0; col < ColNbr; col++)
                {
                    castedMatrix.Val[row][col] = static_cast<U>(Val[row][col]);
                }
            }

            return castedMatrix;
        }

        /**
         * @brief Det is a method that calculates the determinant of the matrix using the rule of Sarrus.
         * @return Determinant of type T
         */
        [[nodiscard]] NOALIAS constexpr T Det() const noexcept
        {
            // Rule of Sarrus: det(A) = aei + bfg + cdh - gec - hfa - idb
            return Val[0][0] * Val[1][1] * Val[2][2] + Val[0][1] * Val[1][2] * Val[2][0]  // aei + bfg.
                   + Val[0][2] * Val[1][0] * Val[2][1] - Val[2][0] * Val[1][1] * Val[0][2]  // + cdh - gec
                   - Val[2][1] * Val[1][2] * Val[0][0] - Val[2][2] * Val[1][0] * Val[0][1]; // - hfa - idb
        }

        /**
         * @brief Transposed is a method that creates a transposed copy of the matrix.
         * @return A transposed copy matrix of type T.
         */
        [[nodiscard]] NOALIAS constexpr Mat3x3<T> Transposed() const noexcept
        {
            Mat3x3<T> transposed;

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
        [[nodiscard]] NOALIAS constexpr Mat3x3<U> Inverted() const
        {
            if (Det() == 0)
            {
                throw DivisionByZeroException();
            }

            // Sum of determinants of all sub-matrices * (-1)^i+j
            Mat3x3<U> coFactorMatrix;

            for (std::size_t row = 0; row < RowNbr; row++)
            {
                for (std::size_t col = 0; col < ColNbr; col++)
                {
                    T sign = (col + row) % 2 == 0 ? 1 : -1;

                    Mat2x2<U> subMat;
                    std::size_t subMatRow = 0, subMatCol = 0;

                    for (std::size_t subRow = 0; subRow < RowNbr; subRow++)
                    {
                        if (subRow == row) continue;

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

                    coFactorMatrix.Val[row][col] = subMat.Det() * sign; // Det(Mij) * (-1)^i+j
                }
            }

            Mat3x3<U> inverted = coFactorMatrix.Transposed() * (static_cast<U>(1 / Det()));

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

    using Mat3x3I = Mat3x3<int>;
    using Mat3x3F = Mat3x3<float>;
}