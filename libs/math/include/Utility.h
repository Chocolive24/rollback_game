#pragma once
/**
 * @headerfile A collection of useful math constants and functions.
 * @author Alexis, Olivier, Constantin, Remy
 */

#include "Angle.h"
#include "TrigoLUT.h"
#include "Definition.h"
#include "Const.h"

constexpr float CalculateLut(Math::Radian radian, std::array<float, Size> table, float step, float rangeStart = 0.f);

namespace Math
{
    constexpr static const float TanMargin = 0.001f;

    constexpr static float SinCosStep = Pi * 2 / Size;
    constexpr static float TanStep = ((Pi / 2.f - TanMargin) + (Pi / 2 + TanMargin)) / Size;
    constexpr static float CotStep = (Pi - TanMargin - (0 + TanMargin)) / Size;

    template<typename T>
    [[nodiscard]] NOALIAS constexpr T Abs(T nbr) noexcept
    {
        return nbr < 0 ? -nbr : nbr;
    }

    template<typename T>
    [[nodiscard]] NOALIAS constexpr T Clamp(T x, T min, T max) noexcept
    {
        if (min > max)
        {
            T tmp = min;
            min = max;
            max = tmp;
        }

        if (x < min)
        {
            x = min;
        }
        else if (x > max)
        {
            x = max;
        }

        return x;
    }

    template<typename T>
    [[nodiscard]] NOALIAS constexpr T Min(const T x, const T y) noexcept
    {
        return x < y ? x : y;
    }

    template<typename T>
    [[nodiscard]] NOALIAS constexpr T Max(const T x, const T y) noexcept
    {
        return x > y ? x : y;
    }

    template<typename T>
    [[nodiscard]] NOALIAS constexpr T Fact(const T n) noexcept
    {
        T result = 1;

        for (int i = 2; i <= Abs(n); i++)
        {
            result *= i;
        }

        if (n < 0)
        {
            result = -result;
        }

        return result;
    }

    template<typename T>
    [[nodiscard]] NOALIAS constexpr T Pow(const T x, const unsigned int power)
    {
        if (x == 0)
        {
            return 0;
        }

        T result = 1;

        for (int i = 1; i <= power; i++)
        {
            result *= x;
        }

        return result;
    }

    [[nodiscard]] NOALIAS constexpr float Sin(const Radian radian) noexcept
    {
        return CalculateLut(radian, SinLUT, SinCosStep);
    }

    [[nodiscard]] NOALIAS constexpr float Cos(const Radian radian) noexcept
    {
        return CalculateLut(radian, CosLUT, SinCosStep);
    }

    /**
     * @brief Get tangent value of an angle inside the LookUpTableTan.
     * @note Angle values in the degree range [-55, 55] are accurate to 5 decimal places.
     * Then the precision is lower and lower down to -90 and 90 degrees, which are indeterminate
     * @param radian The angle in radian.
     * @return An approximate value of the tangent function.
     */
    [[nodiscard]] NOALIAS constexpr float Tan(const Radian radian) noexcept
    {
        return CalculateLut(radian, TanLUT, TanStep, -Pi / 2 + TanMargin);
    }

    [[nodiscard]] NOALIAS constexpr float Cot(const Radian radian) noexcept
    {
        return CalculateLut(radian, CotLUT, CotStep, TanMargin);
    }
}

NOALIAS constexpr float CalculateLut(Math::Radian radian, std::array<float, Size> table, float step, float rangeStart)
{
    const auto angle = static_cast<float>(radian);
	int index = Math::Abs(static_cast<int>((angle - rangeStart) / step));

	while (index >= Size)
	{
		index -= Size;
	}

    if (index == Size - 1) return table[index];

    const float indexValue = table[index];
    const float nextValue = table[index + 1];
    const float ratio = (angle - (rangeStart + index * step)) / step;

    return indexValue + (nextValue - indexValue) * ratio;
}