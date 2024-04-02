#pragma once

/**
 * @author Alexis
 */

#include "Intrinsics.h"
#include "Vec2.h"

#include <array>

namespace Math
{
    template<typename T, int N>
    class NVec2
    {

    public:
        constexpr NVec2() = default;
        constexpr NVec2(const NVec2<T, N>& vec) = default;

        constexpr explicit NVec2(const std::array<Vec2<T>, N> vecs) noexcept
        {
            for (int i = 0; i < N; i++)
            {
                _x[i] = vecs[i].X;
                _y[i] = vecs[i].Y;
            }
        }

        constexpr explicit NVec2(const Vec2<T> vec) noexcept
        {
            for (int i = 0; i < N; i++)
            {
                _x[i] = vec.X;
                _y[i] = vec.Y;
            }
        }

    private:
        std::array<T, N> _x = std::array<T, N>();
        std::array<T, N> _y = std::array<T, N>();

    public:
        [[nodiscard]] NOALIAS NVec2<T, N> operator+(const NVec2<T, N>& nVec2) const noexcept
        {
            NVec2<T, N> result = NVec2<T, N>();

            for (int i = 0; i < N; i++)
            {
                result._x[i] = _x[i] + nVec2._x[i];
                result._y[i] = _y[i] + nVec2._y[i];
            }

            return result;
        }

        NVec2<T, N>& operator+=(const NVec2<T, N>& nVec2) noexcept
        {
            for (int i = 0; i < N; i++)
            {
                _x[i] += nVec2._x[i];
                _y[i] += nVec2._y[i];
            }

            return *this;
        }

        [[nodiscard]] NOALIAS NVec2<T, N> operator-() const noexcept
        {
            NVec2<T, N> result = NVec2<T, N>();

            for (int i = 0; i < N; i++)
            {
                result._x[i] = -_x[i];
                result._y[i] = -_y[i];
            }

            return result;
        }

        [[nodiscard]] NOALIAS NVec2<T, N> operator-(const NVec2<T, N>& nVec2) const noexcept
        {
            NVec2<T, N> result = NVec2<T, N>();

            for (int i = 0; i < N; i++)
            {
                result._x[i] = _x[i] - nVec2._x[i];
                result._y[i] = _y[i] - nVec2._y[i];
            }

            return result;
        }

        NVec2<T, N>& operator-=(const NVec2<T, N>& nVec2) noexcept
        {
            for (int i = 0; i < N; i++)
            {
                _x[i] -= nVec2._x[i];
                _y[i] -= nVec2._y[i];
            }

            return *this;
        }

        [[nodiscard]] NOALIAS NVec2<T, N> operator*(const NVec2<T, N>& nVec2) const noexcept
        {
            NVec2<T, N> result = NVec2<T, N>();

            for (int i = 0; i < N; i++)
            {
                result._x[i] = _x[i] * nVec2._x[i];
                result._y[i] = _y[i] * nVec2._y[i];
            }

            return result;
        }

        NVec2<T, N>& operator*=(const NVec2<T, N>& nVec2) noexcept
        {
            for (int i = 0; i < N; i++)
            {
                _x[i] *= nVec2._x[i];
                _y[i] *= nVec2._y[i];
            }

            return *this;
        }

        [[nodiscard]] NVec2<T, N> operator/(const NVec2<T, N>& nVec2) const
        {
            NVec2<T, N> result = NVec2<T, N>();

            for (int i = 0; i < N; i++)
            {
                if (nVec2._x[i] == 0 || nVec2._y[i] == 0)
                {
                    throw DivisionByZeroException();
                }

                result._x[i] = _x[i] / nVec2._x[i];
                result._y[i] = _y[i] / nVec2._y[i];
            }

            return result;
        }

        NVec2<T, N>& operator/=(const NVec2<T, N>& nVec2)
        {
            for (int i = 0; i < N; i++)
            {
                if (nVec2._x[i] == 0 || nVec2._y[i] == 0)
                {
                    throw DivisionByZeroException();
                }

                _x[i] /= nVec2._x[i];
                _y[i] /= nVec2._y[i];
            }

            return *this;
        }

        [[nodiscard]] NOALIAS NVec2<T, N> operator*(const std::array<T, N> array1N) const noexcept
        {
            NVec2<T, N> result = NVec2<T, N>();

            for (int i = 0; i < N; i++)
            {
                result._x[i] = _x[i] * array1N[i];
                result._y[i] = _y[i] * array1N[i];
            }

            return result;
        }

        NVec2<T, N>& operator*=(const std::array<T, N> array1N) noexcept
        {
            for (int i = 0; i < N; i++)
            {
                _x[i] *= array1N[i];
                _y[i] *= array1N[i];
            }

            return *this;
        }

        [[nodiscard]] NVec2<T, N> operator/(const std::array<T, N> array1N) const
        {
            NVec2<T, N> result = NVec2<T, N>();

            for (int i = 0; i < N; i++)
            {
                if (array1N[i] == 0)
                {
                    throw DivisionByZeroException();
                }

                result._x[i] = _x[i] / array1N[i];
                result._y[i] = _y[i] / array1N[i];
            }

            return result;
        }

        NVec2<T, N>& operator/=(const std::array<T, N> array1N)
        {
            for (int i = 0; i < N; i++)
            {
                if (array1N[i] == 0)
                {
                    throw DivisionByZeroException();
                }

                _x[i] /= array1N[i];
                _y[i] /= array1N[i];
            }

            return *this;
        }

        static std::array<T, N> Dot(const NVec2<T, N>& nV1, const NVec2<T, N>& nV2) noexcept
        {
            std::array<T, N> result = std::array<T, N>();

            for (int i = 0; i < N; i++)
            {
                result[i] = nV1._x[i] * nV2._x[i] + nV1._y[i] * nV2._y[i];
            }

            return result;
        }

        [[nodiscard]] NOALIAS std::array<T, N> SquareMagnitude() const noexcept
        {
            return Dot(*this, *this);
        }

        [[nodiscard]] NOALIAS std::array<T, N> Magnitude() const noexcept
        {
            std::array<T, N> sqrtMagnitude = SquareMagnitude();

            for (int i = 0; i < N; i++)
            {
                sqrtMagnitude[i] = std::sqrt(sqrtMagnitude[i]);
            }

            return sqrtMagnitude;
        }

        [[nodiscard]] NOALIAS std::array<T, N> Normalized() const
        {
            const auto magnitude = Magnitude();

            for (int i = 0; i < N; i++)
            {
                if (magnitude[i] == 0)
                {
                    throw DivisionByZeroException();
                }
            }

            const auto array1N = SquareMagnitude();
            std::array<T, N> reciprocalSqrt = std::array<T, N>();

            for (int i = 0; i < N; i++)
            {
                reciprocalSqrt[i] = 1 / std::sqrt(array1N[i]);
            }

            return reciprocalSqrt;
        }

        [[nodiscard]] NOALIAS const auto& X() const noexcept
        { return _x; }

        [[nodiscard]] NOALIAS const auto& Y() const noexcept
        { return _y; }
    };

    using FourVec2F = NVec2<float, 4>;
    using FourVec2I = NVec2<int, 4>;

#ifdef __SSE__

#pragma region FourVec2F

    template<>
    [[nodiscard]] NOALIAS inline FourVec2F FourVec2F::operator+(const FourVec2F& nVec2) const noexcept
    {
        FourVec2F result = FourVec2F();

        __m128 x1 = _mm_loadu_ps(_x.data());
        __m128 x2 = _mm_loadu_ps(nVec2._x.data());
        __m128 y1 = _mm_loadu_ps(_y.data());
        __m128 y2 = _mm_loadu_ps(nVec2._y.data());

        __m128 x1x2 = _mm_add_ps(x1, x2);
        __m128 y1y2 = _mm_add_ps(y1, y2);

        _mm_storeu_ps(result._x.data(), x1x2);
        _mm_storeu_ps(result._y.data(), y1y2);

        return result;
    }

    template<>
    inline FourVec2F& FourVec2F::operator+=(const FourVec2F& nVec2) noexcept
    {
        __m128 x1 = _mm_loadu_ps(_x.data());
        __m128 x2 = _mm_loadu_ps(nVec2._x.data());
        __m128 y1 = _mm_loadu_ps(_y.data());
        __m128 y2 = _mm_loadu_ps(nVec2._y.data());

        __m128 x1x2 = _mm_add_ps(x1, x2);
        __m128 y1y2 = _mm_add_ps(y1, y2);

        _mm_storeu_ps(_x.data(), x1x2);
        _mm_storeu_ps(_y.data(), y1y2);

        return *this;
    }

    template<>
    [[nodiscard]] NOALIAS inline FourVec2F FourVec2F::operator-() const noexcept
    {
        FourVec2F result = FourVec2F();

        __m128 x1 = _mm_loadu_ps(_x.data());
        __m128 y1 = _mm_loadu_ps(_y.data());

        __m128 x1x2 = _mm_sub_ps(_mm_setzero_ps(), x1);
        __m128 y1y2 = _mm_sub_ps(_mm_setzero_ps(), y1);

        _mm_storeu_ps(result._x.data(), x1x2);
        _mm_storeu_ps(result._y.data(), y1y2);

        return result;
    }

    template<>
    [[nodiscard]] NOALIAS inline FourVec2F FourVec2F::operator-(const FourVec2F& nVec2) const noexcept
    {
        FourVec2F result = FourVec2F();

        __m128 x1 = _mm_loadu_ps(_x.data());
        __m128 x2 = _mm_loadu_ps(nVec2._x.data());
        __m128 y1 = _mm_loadu_ps(_y.data());
        __m128 y2 = _mm_loadu_ps(nVec2._y.data());

        __m128 x1x2 = _mm_sub_ps(x1, x2);
        __m128 y1y2 = _mm_sub_ps(y1, y2);

        _mm_storeu_ps(result._x.data(), x1x2);
        _mm_storeu_ps(result._y.data(), y1y2);

        return result;
    }

    template<>
    inline FourVec2F& FourVec2F::operator-=(const FourVec2F& nVec2) noexcept
    {
        __m128 x1 = _mm_loadu_ps(_x.data());
        __m128 x2 = _mm_loadu_ps(nVec2._x.data());
        __m128 y1 = _mm_loadu_ps(_y.data());
        __m128 y2 = _mm_loadu_ps(nVec2._y.data());

        __m128 x1x2 = _mm_sub_ps(x1, x2);
        __m128 y1y2 = _mm_sub_ps(y1, y2);

        _mm_storeu_ps(_x.data(), x1x2);
        _mm_storeu_ps(_y.data(), y1y2);

        return *this;
    }

    template<>
    [[nodiscard]] NOALIAS inline FourVec2F FourVec2F::operator*(const FourVec2F& nVec2) const noexcept
    {
        FourVec2F result = FourVec2F();

        __m128 x1 = _mm_loadu_ps(_x.data());
        __m128 x2 = _mm_loadu_ps(nVec2._x.data());
        __m128 y1 = _mm_loadu_ps(_y.data());
        __m128 y2 = _mm_loadu_ps(nVec2._y.data());

        __m128 x1x2 = _mm_mul_ps(x1, x2);
        __m128 y1y2 = _mm_mul_ps(y1, y2);

        _mm_storeu_ps(result._x.data(), x1x2);
        _mm_storeu_ps(result._y.data(), y1y2);

        return result;
    }

    template<>
    inline FourVec2F& FourVec2F::operator*=(const FourVec2F& nVec2) noexcept
    {
        __m128 x1 = _mm_loadu_ps(_x.data());
        __m128 x2 = _mm_loadu_ps(nVec2._x.data());
        __m128 y1 = _mm_loadu_ps(_y.data());
        __m128 y2 = _mm_loadu_ps(nVec2._y.data());

        __m128 x1x2 = _mm_mul_ps(x1, x2);
        __m128 y1y2 = _mm_mul_ps(y1, y2);

        _mm_storeu_ps(_x.data(), x1x2);
        _mm_storeu_ps(_y.data(), y1y2);

        return *this;
    }

    template<>
    [[nodiscard]] inline FourVec2F FourVec2F::operator/(const FourVec2F& nVec2) const
    {
        FourVec2F result = FourVec2F();

        for (int i = 0; i < 4; i++)
        {
            if (nVec2._x[i] == 0 || nVec2._y[i] == 0)
            {
                throw DivisionByZeroException();
            }
        }

        __m128 x1 = _mm_loadu_ps(_x.data());
        __m128 x2 = _mm_loadu_ps(nVec2._x.data());
        __m128 y1 = _mm_loadu_ps(_y.data());
        __m128 y2 = _mm_loadu_ps(nVec2._y.data());

        __m128 x1x2 = _mm_div_ps(x1, x2);
        __m128 y1y2 = _mm_div_ps(y1, y2);

        _mm_storeu_ps(result._x.data(), x1x2);
        _mm_storeu_ps(result._y.data(), y1y2);

        return result;
    }

    template<>
    inline FourVec2F& FourVec2F::operator/=(const FourVec2F& nVec2)
    {
        for (int i = 0; i < 4; i++)
        {
            if (nVec2._x[i] == 0 || nVec2._y[i] == 0)
            {
                throw DivisionByZeroException();
            }
        }

        __m128 x1 = _mm_loadu_ps(_x.data());
        __m128 x2 = _mm_loadu_ps(nVec2._x.data());
        __m128 y1 = _mm_loadu_ps(_y.data());
        __m128 y2 = _mm_loadu_ps(nVec2._y.data());

        __m128 x1x2 = _mm_div_ps(x1, x2);
        __m128 y1y2 = _mm_div_ps(y1, y2);

        _mm_storeu_ps(_x.data(), x1x2);
        _mm_storeu_ps(_y.data(), y1y2);

        return *this;
    }

    template<>
    [[nodiscard]] NOALIAS inline std::array<float, 4> FourVec2F::Dot(const NVec2<float, 4>& nV1, const NVec2<float, 4>& nV2) noexcept
    {
        std::array<float, 4> result = std::array<float, 4>();

        __m128 x1 = _mm_loadu_ps(nV1._x.data());
        __m128 x2 = _mm_loadu_ps(nV2._x.data());
        __m128 y1 = _mm_loadu_ps(nV1._y.data());
        __m128 y2 = _mm_loadu_ps(nV2._y.data());

        __m128 x1x2 = _mm_mul_ps(x1, x2);
        __m128 y1y2 = _mm_mul_ps(y1, y2);

        __m128 x1x2y1y2 = _mm_add_ps(x1x2, y1y2);

        _mm_storeu_ps(result.data(), x1x2y1y2);

        return result;
    }

    template<>
    [[nodiscard]] NOALIAS inline std::array<float, 4> FourVec2F::SquareMagnitude() const noexcept
    {
        std::array<float, 4> result = std::array<float, 4>();

        __m128 x1 = _mm_loadu_ps(_x.data());
        __m128 y1 = _mm_loadu_ps(_y.data());

        __m128 x1x2 = _mm_mul_ps(x1, x1);
        __m128 y1y2 = _mm_mul_ps(y1, y1);

        __m128 x1x2y1y2 = _mm_add_ps(x1x2, y1y2);

        _mm_storeu_ps(result.data(), x1x2y1y2);

        return result;
    }

    template<>
    [[nodiscard]] NOALIAS inline std::array<float, 4> FourVec2F::Magnitude() const noexcept
    {
        std::array<float, 4> result = std::array<float, 4>();

        __m128 x1 = _mm_loadu_ps(_x.data());
        __m128 y1 = _mm_loadu_ps(_y.data());

        __m128 x1x2 = _mm_mul_ps(x1, x1);
        __m128 y1y2 = _mm_mul_ps(y1, y1);

        __m128 x1x2y1y2 = _mm_add_ps(x1x2, y1y2);

        __m128 x1x2y1y2Sqrt = _mm_sqrt_ps(x1x2y1y2);

        _mm_storeu_ps(result.data(), x1x2y1y2Sqrt);

        return result;
    }

    template<>
    [[nodiscard]] NOALIAS inline std::array<float, 4> FourVec2F::Normalized() const
    {
        std::array<float, 4> result = std::array<float, 4>();

        // Check if magnitude is 0.
        const auto magnitude = Magnitude();

        for (int i = 0; i < 4; i++)
        {
            if (magnitude[i] == 0)
            {
                throw DivisionByZeroException();
            }
        }

        __m128 x1 = _mm_loadu_ps(_x.data());
        __m128 y1 = _mm_loadu_ps(_y.data());

        __m128 x1x2 = _mm_mul_ps(x1, x1);
        __m128 y1y2 = _mm_mul_ps(y1, y1);

        __m128 x1x2y1y2 = _mm_add_ps(x1x2, y1y2);

        __m128 x1x2y1y2Sqrt = _mm_sqrt_ps(x1x2y1y2);

        __m128 x1x2y1y2SqrtReciprocal = _mm_rcp_ps(x1x2y1y2Sqrt);

        _mm_storeu_ps(result.data(), x1x2y1y2SqrtReciprocal);

        return result;
    }

#pragma endregion

#pragma region FourVec2I

    template<>
    [[nodiscard]] NOALIAS inline FourVec2I FourVec2I::operator+(const FourVec2I& nVec2) const noexcept
    {
        FourVec2I result = FourVec2I();

        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
        __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._x.data()));
        __m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
        __m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._y.data()));

        __m128i x1x2 = _mm_add_epi32(x1, x2);
        __m128i y1y2 = _mm_add_epi32(y1, y2);

        _mm_storeu_si128(reinterpret_cast<__m128i*>(result._x.data()), x1x2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(result._y.data()), y1y2);

        return result;
    }

    template<>
    inline FourVec2I& FourVec2I::operator+=(const FourVec2I& nVec2) noexcept
    {
        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
        __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._x.data()));
        __m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
        __m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._y.data()));

        __m128i x1x2 = _mm_add_epi32(x1, x2);
        __m128i y1y2 = _mm_add_epi32(y1, y2);

        _mm_storeu_si128(reinterpret_cast<__m128i*>(_x.data()), x1x2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(_y.data()), y1y2);

        return *this;
    }

    template<>
    [[nodiscard]] NOALIAS inline FourVec2I FourVec2I::operator-() const noexcept
    {
        FourVec2I result = FourVec2I();

        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
        __m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));

        __m128i x1x2 = _mm_sub_epi32(_mm_setzero_si128(), x1);
        __m128i y1y2 = _mm_sub_epi32(_mm_setzero_si128(), y1);

        _mm_storeu_si128(reinterpret_cast<__m128i*>(result._x.data()), x1x2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(result._y.data()), y1y2);

        return result;
    }

    template<>
    [[nodiscard]] NOALIAS inline FourVec2I FourVec2I::operator-(const FourVec2I& nVec2) const noexcept
    {
        FourVec2I result = FourVec2I();

        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
        __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._x.data()));
        __m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
        __m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._y.data()));

        __m128i x1x2 = _mm_sub_epi32(x1, x2);
        __m128i y1y2 = _mm_sub_epi32(y1, y2);

        _mm_storeu_si128(reinterpret_cast<__m128i*>(result._x.data()), x1x2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(result._y.data()), y1y2);

        return result;
    }

    template<>
    inline FourVec2I& FourVec2I::operator-=(const FourVec2I& nVec2) noexcept
    {
        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
        __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._x.data()));
        __m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
        __m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._y.data()));

        __m128i x1x2 = _mm_sub_epi32(x1, x2);
        __m128i y1y2 = _mm_sub_epi32(y1, y2);

        _mm_storeu_si128(reinterpret_cast<__m128i*>(_x.data()), x1x2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(_y.data()), y1y2);

        return *this;
    }

    template<>
    inline FourVec2I FourVec2I::operator*(const FourVec2I& nVec2) const noexcept
    {
        FourVec2I result = FourVec2I();

        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
        __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._x.data()));
        __m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
        __m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._y.data()));

        __m128i x1x2 = _mm_mullo_epi32(x1, x2);
        __m128i y1y2 = _mm_mullo_epi32(y1, y2);

        _mm_storeu_si128(reinterpret_cast<__m128i*>(result._x.data()), x1x2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(result._y.data()), y1y2);

        return result;
    }

    template<>
    inline FourVec2I& FourVec2I::operator*=(const FourVec2I& nVec2) noexcept
    {
        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
        __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._x.data()));
        __m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
        __m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._y.data()));

        __m128i x1x2 = _mm_mullo_epi32(x1, x2);
        __m128i y1y2 = _mm_mullo_epi32(y1, y2);

        _mm_storeu_si128(reinterpret_cast<__m128i*>(_x.data()), x1x2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(_y.data()), y1y2);

        return *this;
    }

#ifdef _mm_div_epi32

    template <>
    [[nodiscard]] inline FourVec2I FourVec2I::operator/(const FourVec2I& nVec2) const
    {
        FourVec2I result = FourVec2I();

        for (int i = 0; i < 4; i++)
        {
            if (nVec2._x[i] == 0 || nVec2._y[i] == 0)
            {
                throw DivisionByZeroException();
            }
        }

        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
        __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._x.data()));
        __m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
        __m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._y.data()));

        __m128i x1x2 = _mm_div_epi32(x1, x2);
        __m128i y1y2 = _mm_div_epi32(y1, y2);

        _mm_storeu_si128(reinterpret_cast<__m128i*>(result._x.data()), x1x2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(result._y.data()), y1y2);

        return result;
    }

    template <>
    inline FourVec2I& FourVec2I::operator/=(const FourVec2I& nVec2)
    {
        for (int i = 0; i < 4; i++)
        {
            if (nVec2._x[i] == 0 || nVec2._y[i] == 0)
            {
                throw DivisionByZeroException();
            }
        }

        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
        __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._x.data()));
        __m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
        __m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._y.data()));

        __m128i x1x2 = _mm_div_epi32(x1, x2);
        __m128i y1y2 = _mm_div_epi32(y1, y2);

        _mm_storeu_si128(reinterpret_cast<__m128i*>(_x.data()), x1x2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(_y.data()), y1y2);

        return *this;
    }

#endif

    template<>
    [[nodiscard]] NOALIAS inline std::array<int, 4> FourVec2I::Dot(const NVec2<int, 4>& nV1, const NVec2<int, 4>& nV2) noexcept
    {
        std::array<int, 4> result = std::array<int, 4>();

        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nV1._x.data()));
        __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nV2._x.data()));
        __m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nV1._y.data()));
        __m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nV2._y.data()));

        __m128i x1x2 = _mm_mullo_epi32(x1, x2);
        __m128i y1y2 = _mm_mullo_epi32(y1, y2);

        __m128i x1x2y1y2 = _mm_add_epi32(x1x2, y1y2);

        _mm_storeu_si128(reinterpret_cast<__m128i*>(result.data()), x1x2y1y2);

        return result;
    }

    template<>
    [[nodiscard]] NOALIAS inline std::array<int, 4> FourVec2I::SquareMagnitude() const noexcept
    {
        std::array<int, 4> result = std::array<int, 4>();

        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
        __m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));

        __m128i x1x2 = _mm_mullo_epi32(x1, x1);
        __m128i y1y2 = _mm_mullo_epi32(y1, y1);

        __m128i x1x2y1y2 = _mm_add_epi32(x1x2, y1y2);

        _mm_storeu_si128(reinterpret_cast<__m128i*>(result.data()), x1x2y1y2);

        return result;
    }

#ifdef _mm_sqrt_epi32

    template <>
    [[nodiscard]] NOALIAS inline std::array<int, 4> FourVec2I::Magnitude() const noexcept
    {
        std::array<int, 4> result = std::array<int, 4>();

        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
        __m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));

        __m128i x1x2 = _mm_mullo_epi32(x1, x1);
        __m128i y1y2 = _mm_mullo_epi32(y1, y1);

        __m128i x1x2y1y2 = _mm_add_epi32(x1x2, y1y2);

        __m128i x1x2y1y2Sqrt = _mm_sqrt_epi32(x1x2y1y2);

        _mm_storeu_si128(reinterpret_cast<__m128i*>(result.data()), x1x2y1y2Sqrt);

        return result;
    }

    template <>
    inline std::array<int, 4> FourVec2I::Normalized() const
    {
        std::array<int, 4> result = std::array<int, 4>();

        // Check if magnitude is 0.
        const auto magnitude = Magnitude();

        for (int i = 0; i < 4; i++)
        {
            if (magnitude[i] == 0)
            {
                throw DivisionByZeroException();
            }
        }

        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
        __m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));

        __m128i x1x2 = _mm_mullo_epi32(x1, x1);
        __m128i y1y2 = _mm_mullo_epi32(y1, y1);

        __m128i x1x2y1y2 = _mm_add_epi32(x1x2, y1y2);

        __m128i x1x2y1y2Sqrt = _mm_sqrt_epi32(x1x2y1y2);

        __m128i x1x2y1y2SqrtReciprocal = _mm_rcp_epi32(x1x2y1y2Sqrt);

        _mm_storeu_si128(reinterpret_cast<__m128i*>(result.data()), x1x2y1y2SqrtReciprocal);

        return result;
    }

#endif

#pragma endregion

#endif

}