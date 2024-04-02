#pragma once

/**
 * @brief A collection of classes and functions to work with angles.
 * @author Alexis
 */

#include "Exception.h"
#include "Definition.h"
#include "Const.h"

#include <stdexcept>

namespace Math
{
    class Radian;

    class Degree
    {
    public:
        constexpr Degree() noexcept = default;

        constexpr explicit Degree(float value) noexcept
        {
            _value = value;
        }

    private:
        float _value{ 0.f };

    public:
        NOALIAS constexpr explicit operator float() const noexcept
        {
            return _value;
        }

        NOALIAS operator Radian() const noexcept;

        NOALIAS Degree(Radian radian) noexcept;

        [[nodiscard]] NOALIAS constexpr Degree operator+(const Degree degree) const noexcept
        {
            return Degree(_value + degree._value);
        }

        [[nodiscard]] NOALIAS constexpr Degree operator-(const Degree degree) const noexcept
        {
            return Degree(_value - degree._value);
        }

        [[nodiscard]] NOALIAS constexpr Degree operator-() const noexcept
        {
            return Degree(-_value);
        }

        constexpr Degree operator+=(const Degree degree) noexcept
        {
            _value += degree._value;

            return *this;
        }

        constexpr Degree operator-=(const Degree degree) noexcept
        {
            _value -= degree._value;

            return *this;
        }

        [[nodiscard]] NOALIAS constexpr Degree operator*(const float scalar) const noexcept
        {
            return Degree(_value * scalar);
        }

        [[nodiscard]] NOALIAS constexpr Degree operator/(const float scalar) const
        {
            if (scalar == 0)
            {
                throw DivisionByZeroException();
            }

            return Degree(_value / scalar);
        }

        constexpr Degree operator*=(const float scalar) noexcept
        {
            _value *= scalar;

            return *this;
        }

        constexpr Degree operator/=(const float scalar)
        {
            if (scalar == 0)
            {
                throw DivisionByZeroException();
            }

            _value /= scalar;

            return *this;
        }

        NOALIAS constexpr bool operator==(const Degree degree) const noexcept
        {
            return _value == degree._value;
        }

        NOALIAS constexpr bool operator!=(const Degree degree) const noexcept
        {
            return !operator==(degree);
        }

        NOALIAS constexpr bool operator<(const Degree degree) const noexcept
        {
            return _value < degree._value;
        }

        NOALIAS constexpr bool operator>(const Degree degree) const noexcept
        {
            return _value > degree._value;
        }

        NOALIAS constexpr bool operator<=(const Degree degree) const noexcept
        {
            return !operator>(degree);
        }

        NOALIAS constexpr bool operator>=(const Degree degree) const noexcept
        {
            return !operator<(degree);
        }
    };

    class Radian
    {
    public:
        constexpr Radian() noexcept = default;

        constexpr explicit Radian(float value) noexcept
        {
            _value = value;
        }

    private:
        float _value{ 0 };

    public:
        constexpr Radian(Degree degree) noexcept
        {
            _value = static_cast<float>(degree) * 3.14159265358979323846f / 180.0f;
        }

        NOALIAS constexpr explicit operator float() const noexcept
        {
            return _value;
        }

        NOALIAS constexpr Radian operator+(const Radian radian) const noexcept
        {
            return Radian(_value + radian._value);
        }

        NOALIAS constexpr Radian operator-(const Radian radian) const noexcept
        {
            return Radian(_value - radian._value);
        }

        NOALIAS constexpr Radian operator-() const noexcept
        {
            return Radian(-_value);
        }

        constexpr Radian operator+=(const Radian radian) noexcept
        {
            _value += radian._value;

            return *this;
        }

        constexpr Radian operator-=(const Radian radian) noexcept
        {
            _value -= radian._value;

            return *this;
        }

        NOALIAS constexpr Radian operator*(const float scalar) const noexcept
        {
            return Radian(_value * scalar);
        }

        NOALIAS friend Radian operator*(const float scalar, const Radian radian) noexcept
        {
            return radian * scalar;
        }

        NOALIAS constexpr Radian operator/(const float scalar) const
        {
            if (scalar == 0)
            {
                throw DivisionByZeroException();
            }

            return Radian(_value / scalar);
        }

        constexpr Radian operator*=(const float scalar) noexcept
        {
            _value *= scalar;

            return *this;
        }

        constexpr Radian operator/=(const float scalar)
        {
            if (scalar == 0)
            {
                throw DivisionByZeroException();
            }

            _value /= scalar;

            return *this;
        }

        NOALIAS constexpr bool operator==(const Radian radian) const noexcept
        {
            return _value == radian._value;
        }

        NOALIAS constexpr bool operator!=(const Radian radian) const noexcept
        {
            return !operator==(radian);
        }

        NOALIAS constexpr bool operator<(const Radian radian) const noexcept
        {
            return _value < radian._value;
        }

        NOALIAS constexpr bool operator>(const Radian radian) const noexcept
        {
            return _value > radian._value;
        }

        NOALIAS constexpr bool operator<=(const Radian radian) const noexcept
        {
            return !operator>(radian);
        }

        NOALIAS constexpr bool operator>=(const Radian radian) const noexcept
        {
            return !operator<(radian);
        }
    };

    inline Degree::operator Radian() const noexcept
    {
        return Radian(_value);
    }

    inline Degree::Degree(Radian radian) noexcept
    {
        _value = static_cast<float>(radian) * 180.0f / Math::Pi;
    }
}