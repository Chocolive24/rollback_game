#pragma once

/**
 * @author Rémy
 */

#include "Intrinsics.h"
#include "Exception.h"
#include "Definition.h"

#include <array>

namespace Math
{
    template<typename T, int N>
    class NScalar
    {

    public:
        constexpr NScalar() noexcept = default;

        explicit constexpr NScalar(const std::array<T, N> scalars) noexcept
        {
            for (int i = 0; i < N; i++)
            {
                _scalars[i] = scalars[i];
            }
        }

        explicit constexpr NScalar(const T scalar) noexcept
        {
            for (int i = 0; i < N; i++)
            {
                _scalars[i] = scalar;
            }
        }

    private:
        std::array<T, N> _scalars {};

    public:
        [[nodiscard]] NOALIAS NScalar<T, N> operator+(const NScalar<T, N> nScalar) const noexcept
        {
            NScalar<T, N> result = NScalar<T, N>();

            for (int i = 0; i < N; i++)
            {
                result._scalars[i] = _scalars[i] + nScalar._scalars[i];
            }
            return result;
        }

        NScalar<T, N>& operator+=(const NScalar<T, N> nScalar) noexcept
        {
            for (int i = 0; i < N; i++)
            {
                _scalars[i] += nScalar._scalars[i];
            }

            return *this;
        }

        [[nodiscard]] NOALIAS NScalar<T, N> operator-() const noexcept
        {
            NScalar<T, N> result = NScalar<T, N>();
            for (int i = 0; i < N; i++)
            {
                result._scalars[i] = -_scalars[i];
            }
            return result;
        }

        [[nodiscard]] NOALIAS NScalar<T, N> operator-(const NScalar<T, N> nScalar) const noexcept
        {
            NScalar<T, N> result = NScalar<T, N>();
            for (int i = 0; i < N; i++)
            {
                result._scalars[i] = _scalars[i] - nScalar._scalars[i];
            }
            return result;
        }

        NScalar<T, N>& operator-=(const NScalar<T, N> nScalar) noexcept
        {
            for (int i = 0; i < N; i++)
            {
                _scalars -= nScalar._scalars[i];
            }
            return *this;
        }

        [[nodiscard]] NOALIAS NScalar<T, N> operator*(const NScalar<T, N> nScalar) const noexcept
        {
            NScalar<T, N> result = NScalar<T, N>();
            for (int i = 0; i < N; i++)
            {
                result._scalars[i] = _scalars[i] * nScalar._scalars[i];
            }
            return result;
        }

        NScalar<T, N>& operator*=(const NScalar<T, N> nScalar) noexcept
        {
            for (int i = 0; i < N; i++)
            {
                _scalars[i] *= nScalar._scalars[i];
            }
            return *this;
        }

        [[nodiscard]] NScalar<T, N> operator/(const NScalar<T, N> nScalar) const
        {
            NScalar<T, N> result = NScalar<T, N>();

            for (int i = 0; i < N; i++)
            {
                if (nScalar._scalars[i] == 0)
                {
                    throw DivisionByZeroException();
                }

                result._scalars[i] = _scalars[i] / nScalar._scalars[i];
            }
            return result;
        }

        NScalar<T, N>& operator/=(const NScalar<T, N> nScalar)
        {
            for (int i = 0; i < N; i++)
            {
                if (nScalar._scalars[i] == 0)
                {
                    throw DivisionByZeroException();
                }

                _scalars[i] /= nScalar._scalars[i];
            }

            return *this;
        }

        [[nodiscard]] NOALIAS NScalar<T, N> operator*(const std::array<T, N> array1N) const noexcept
        {
            NScalar<T, N> result = NScalar<T, N>();
            for (int i = 0; i < N; i++)
            {
                result._scalars[i] = _scalars[i] * array1N[i];
            }
            return result;
        }

        NScalar<T, N>& operator*=(const std::array<T, N> array1N) noexcept
        {
            for (int i = 0; i < N; i++)
            {
                _scalars[i] *= array1N[i];
            }
            return *this;
        }

        [[nodiscard]] NScalar<T, N> operator/(const std::array<T, N> array1N) const
        {
            NScalar<T, N> result = NScalar<T, N>();

            for (int i = 0; i < N; i++)
            {
                if (array1N[i] == 0)
                {
                    throw DivisionByZeroException();
                }

                result._scalars[i] = _scalars[i] / array1N[i];
            }
            return result;
        }

        NScalar<T, N>& operator/=(const std::array<T, N> array1N)
        {
            for (int i = 0; i < N; i++)
            {
                if (array1N[i] == 0)
                {
                    throw DivisionByZeroException();
                }
                _scalars[i] /= array1N[i];
            }
            return *this;
        }

        [[nodiscard]] NOALIAS T operator[](const int index) const
        {
            return _scalars[index];
        }
    };

    using FourScalarF = NScalar<float, 4>;
    using FourScalarI = NScalar<int, 4>;

#ifdef __SSE__

    template<>
    [[nodiscard]] NOALIAS inline FourScalarF FourScalarF::operator+(const FourScalarF nScalar) const noexcept
    {
        FourScalarF result = FourScalarF();

        __m128 x1 = _mm_loadu_ps(_scalars.data());
        __m128 x2 = _mm_loadu_ps(nScalar._scalars.data());
        __m128 x1x2 = _mm_add_ps(x1, x2);
        _mm_storeu_ps(result._scalars.data(), x1x2);
        return result;
    }

    template<>
    inline FourScalarF& FourScalarF::operator+=(const FourScalarF nScalar) noexcept
    {
        __m128 x1 = _mm_loadu_ps(_scalars.data());
        __m128 x2 = _mm_loadu_ps(nScalar._scalars.data());
        __m128 x1x2 = _mm_add_ps(x1, x2);
        _mm_storeu_ps(_scalars.data(), x1x2);
        return *this;
    }

    template<>
    [[nodiscard]] NOALIAS inline FourScalarF FourScalarF::operator-() const noexcept
    {
        FourScalarF result = FourScalarF();
        __m128 x1 = _mm_loadu_ps(_scalars.data());
        __m128 x1x2 = _mm_sub_ps(_mm_setzero_ps(), x1);
        _mm_storeu_ps(result._scalars.data(), x1x2);
        return result;
    }

    template<>
    [[nodiscard]] NOALIAS inline FourScalarF FourScalarF::operator-(const FourScalarF nScalar) const noexcept
    {
        FourScalarF result = FourScalarF();

        __m128 x1 = _mm_loadu_ps(_scalars.data());
        __m128 x2 = _mm_loadu_ps(nScalar._scalars.data());
        __m128 x1x2 = _mm_sub_ps(x1, x2);
        _mm_storeu_ps(result._scalars.data(), x1x2);
        return result;
    }

    template<>
    inline FourScalarF& FourScalarF::operator-=(const FourScalarF nScalar) noexcept
    {
        __m128 x1 = _mm_loadu_ps(_scalars.data());
        __m128 x2 = _mm_loadu_ps(nScalar._scalars.data());
        __m128 x1x2 = _mm_sub_ps(x1, x2);
        _mm_storeu_ps(_scalars.data(), x1x2);

        return *this;
    }

    template<>
    [[nodiscard]] NOALIAS inline FourScalarF FourScalarF::operator*(const FourScalarF nScalar) const noexcept
    {
        FourScalarF result = FourScalarF();

        __m128 x1 = _mm_loadu_ps(_scalars.data());
        __m128 x2 = _mm_loadu_ps(nScalar._scalars.data());
        __m128 x1x2 = _mm_mul_ps(x1, x2);
        _mm_storeu_ps(result._scalars.data(), x1x2);
        return result;
    }

    template<>
    inline FourScalarF& FourScalarF::operator*=(const FourScalarF nScalar) noexcept
    {
        __m128 x1 = _mm_loadu_ps(_scalars.data());
        __m128 x2 = _mm_loadu_ps(nScalar._scalars.data());
        __m128 x1x2 = _mm_mul_ps(x1, x2);
        _mm_storeu_ps(_scalars.data(), x1x2);
        return *this;
    }

    template<>
    [[nodiscard]] inline FourScalarF FourScalarF::operator/(const FourScalarF nScalar) const
    {
        FourScalarF result = FourScalarF();

        for (int i = 0; i < 4; i++)
        {
            if (nScalar._scalars[i] == 0)
            {
                throw DivisionByZeroException();
            }
        }

        __m128 x1 = _mm_loadu_ps(_scalars.data());
        __m128 x2 = _mm_loadu_ps(nScalar._scalars.data());
        __m128 x1x2 = _mm_div_ps(x1, x2);

        _mm_storeu_ps(result._scalars.data(), x1x2);

        return result;
    }

    template<>
    inline FourScalarF& FourScalarF::operator/=(const FourScalarF nScalar)
    {
        for (int i = 0; i < 4; i++)
        {
            if (nScalar._scalars[i] == 0)
            {
                throw DivisionByZeroException();
            }
        }

        __m128 x1 = _mm_loadu_ps(_scalars.data());
        __m128 x2 = _mm_loadu_ps(nScalar._scalars.data());
        __m128 x1x2 = _mm_div_ps(x1, x2);
        _mm_storeu_ps(_scalars.data(), x1x2);
        return *this;
    }

    template<>
    inline FourScalarI FourScalarI::operator+(const FourScalarI nScalar) const noexcept
    {
        FourScalarI result = FourScalarI();

        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_scalars.data()));
        __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nScalar._scalars.data()));
        __m128i x1x2 = _mm_add_epi32(x1, x2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(result._scalars.data()), x1x2);
        return result;
    }

    template<>
    inline FourScalarI& FourScalarI::operator+=(const FourScalarI nScalar) noexcept
    {
        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_scalars.data()));
        __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nScalar._scalars.data()));
        __m128i x1x2 = _mm_add_epi32(x1, x2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(_scalars.data()), x1x2);

        return *this;
    }

    template<>
    [[nodiscard]] NOALIAS inline FourScalarI FourScalarI::operator-() const noexcept
    {
        FourScalarI result = FourScalarI();

        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_scalars.data()));
        __m128i x1x2 = _mm_sub_epi32(_mm_setzero_si128(), x1);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(result._scalars.data()), x1x2);

        return result;
    }

    template<>
    [[nodiscard]] NOALIAS inline FourScalarI FourScalarI::operator-(const FourScalarI nScalar) const noexcept
    {
        FourScalarI result = FourScalarI();

        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_scalars.data()));
        __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nScalar._scalars.data()));
        __m128i x1x2 = _mm_sub_epi32(x1, x2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(result._scalars.data()), x1x2);

        return result;
    }

    template<>
    inline FourScalarI& FourScalarI::operator-=(const FourScalarI nScalar) noexcept
    {
        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_scalars.data()));
        __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nScalar._scalars.data()));
        __m128i x1x2 = _mm_sub_epi32(x1, x2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(_scalars.data()), x1x2);
        return *this;
    }

    template<>
    [[nodiscard]] NOALIAS inline FourScalarI FourScalarI::operator*(const FourScalarI nScalar) const noexcept
    {
        FourScalarI result = FourScalarI();

        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_scalars.data()));
        __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nScalar._scalars.data()));
        __m128i x1x2 = _mm_mullo_epi32(x1, x2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(result._scalars.data()), x1x2);
        return result;
    }

    template<>
    inline FourScalarI& FourScalarI::operator*=(const FourScalarI nScalar) noexcept
    {
        __m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_scalars.data()));
        __m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nScalar._scalars.data()));
        __m128i x1x2 = _mm_mullo_epi32(x1, x2);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(_scalars.data()), x1x2);
        return *this;
    }
#endif
}