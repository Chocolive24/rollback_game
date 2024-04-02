/**
 * @author Olivier
 */

#pragma once

#include "Intrinsics.h"
#include "Definition.h"
#include "Vec4.h"

#include <array>
#include <cmath>

namespace Math
{
	template<typename T, int N>
	class NVec4
	{
	public:
		constexpr NVec4() = default;
		constexpr NVec4(const NVec4<T, N>& vec) = default;

		constexpr explicit NVec4(const std::array<Vec4<T>, N> vecs) noexcept
		{
			for (int i = 0; i < N; i++)
			{
				_x[i] = vecs[i].X;
				_y[i] = vecs[i].Y;
				_z[i] = vecs[i].Z;
				_w[i] = vecs[i].W;
			}
		}

		constexpr explicit NVec4(const Vec4<T> vec) noexcept
		{
			for (int i = 0; i < N; i++)
			{
				_x[i] = vec.X;
				_y[i] = vec.Y;
				_z[i] = vec.Z;
				_w[i] = vec.W;
			}
		}

	private:
		std::array<T, N> _x;
		std::array<T, N> _y;
		std::array<T, N> _z;
		std::array<T, N> _w;

	public:
		[[nodiscard]] NOALIAS const auto& X() const noexcept { return _x; }

		[[nodiscard]] NOALIAS const auto& Y() const noexcept { return _y; }

		[[nodiscard]] NOALIAS const auto& Z() const noexcept { return _z; }

		[[nodiscard]] NOALIAS const auto& W() const noexcept { return _w; }

		[[nodiscard]] NOALIAS NVec4<T, N> operator+(const NVec4<T, N>& nVec4) const noexcept
		{
			NVec4<T, N> result = NVec4<T, N>();

			for (int i = 0; i < N; i++)
			{
				result._x[i] = _x[i] + nVec4._x[i];
				result._y[i] = _y[i] + nVec4._y[i];
				result._z[i] = _z[i] + nVec4._z[i];
				result._w[i] = _w[i] + nVec4._w[i];
			}

			return result;
		}

		NVec4<T, N>& operator+=(const NVec4<T, N>& nVec4) noexcept
		{
			return *this = *this + nVec4;
		}

		[[nodiscard]] NOALIAS NVec4<T, N> operator-() const noexcept
		{
			NVec4<T, N> result = NVec4<T, N>();

			for (int i = 0; i < N; i++)
			{
				result._x[i] = -_x[i];
				result._y[i] = -_y[i];
				result._z[i] = -_z[i];
				result._w[i] = -_w[i];
			}

			return result;
		}

		[[nodiscard]] NOALIAS NVec4<T, N> operator-(const NVec4<T, N>& nVec4) const noexcept
		{
			NVec4<T, N> result = NVec4<T, N>();

			for (int i = 0; i < N; i++)
			{
				result._x[i] = _x[i] - nVec4._x[i];
				result._y[i] = _y[i] - nVec4._y[i];
				result._z[i] = _z[i] - nVec4._z[i];
				result._w[i] = _w[i] - nVec4._w[i];
			}

			return result;
		}

		NVec4<T, N>& operator-=(const NVec4<T, N>& nVec4) noexcept
		{
			return *this = *this - nVec4;
		}

		[[nodiscard]] NOALIAS NVec4<T, N> operator*(const NVec4<T, N>& nVec4) const noexcept
		{
			NVec4<T, N> result = NVec4<T, N>();

			for (int i = 0; i < N; i++)
			{
				result._x[i] = _x[i] * nVec4._x[i];
				result._y[i] = _y[i] * nVec4._y[i];
				result._z[i] = _z[i] * nVec4._z[i];
				result._w[i] = _w[i] * nVec4._w[i];
			}

			return result;
		}

		NVec4<T, N>& operator*=(const NVec4<T, N>& nVec4) noexcept
		{
			return *this = *this * nVec4;
		}

		[[nodiscard]] NVec4<T, N> operator/(const NVec4<T, N>& nVec4) const
		{
			NVec4<T, N> result = NVec4<T, N>();

			for (int i = 0; i < N; i++)
			{
				if (nVec4._x[i] == 0 || nVec4._y[i] == 0 || nVec4._z[i] == 0 || nVec4._w[i] == 0)
				{
					throw DivisionByZeroException();
				}

				result._x[i] = _x[i] / nVec4._x[i];
				result._y[i] = _y[i] / nVec4._y[i];
				result._z[i] = _z[i] / nVec4._z[i];
				result._w[i] = _w[i] / nVec4._w[i];
			}

			return result;
		}

		NVec4<T, N>& operator/=(const NVec4<T, N>& nVec4)
		{
			return *this = *this / nVec4;
		}

		[[nodiscard]] NOALIAS NVec4<T, N> operator*(const std::array<T, N> array1N) const noexcept
		{
			NVec4<T, N> result = NVec4<T, N>();

			for (int i = 0; i < N; i++)
			{
				result._x[i] = _x[i] * array1N[i];
				result._y[i] = _y[i] * array1N[i];
				result._z[i] = _z[i] * array1N[i];
				result._w[i] = _w[i] * array1N[i];
			}

			return result;
		}

		NVec4<T, N>& operator*=(const std::array<T, N> array1N) noexcept
		{
			return *this = *this * array1N;
		}

		[[nodiscard]] NVec4<T, N> operator/(const std::array<T, N> array1N) const
		{
			NVec4<T, N> result = NVec4<T, N>();

			for (int i = 0; i < N; i++)
			{
				if (array1N[i] == 0)
				{
					throw DivisionByZeroException();
				}

				result._x[i] = _x[i] / array1N[i];
				result._y[i] = _y[i] / array1N[i];
				result._z[i] = _z[i] / array1N[i];
				result._w[i] = _w[i] / array1N[i];
			}

			return result;
		}

		NVec4<T, N>& operator/=(const std::array<T, N> array1N)
		{
			return *this = *this / array1N;
		}

		static std::array<T, N> Dot(const NVec4<T, N>& nV1, const NVec4<T, N>& nV2) noexcept
		{
			std::array<T, N> result = std::array<T, N>();

			for (int i = 0; i < N; i++)
			{
				result[i] = nV1._x[i] * nV2._x[i] + nV1._y[i] * nV2._y[i] +
				            nV1._z[i] * nV2._z[i] + nV1._w[i] * nV2._w[i];
			}

			return result;
		}

		[[nodiscard]] NOALIAS std::array<T, N> SquareMagnitude() const noexcept
		{
			std::array<T, N> result;

			for (int i = 0; i < N; i++)
			{
				result[i] = _x[i] * _x[i] + _y[i] * _y[i] + _z[i] * _z[i] + _w[i] * _w[i];
			}

			return result;
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
	};

	using FourVec4F = NVec4<float, 4>;
	using FourVec4I = NVec4<int, 4>;

#ifdef __SSE__

#pragma region FourVec4F

	template<>
	[[nodiscard]] NOALIAS inline FourVec4F FourVec4F::operator+(const FourVec4F& nVec4) const noexcept
	{
		FourVec4F result = FourVec4F();

		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 x2 = _mm_loadu_ps(nVec4._x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 y2 = _mm_loadu_ps(nVec4._y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 z2 = _mm_loadu_ps(nVec4._z.data());
		__m128 w1 = _mm_loadu_ps(_w.data());
		__m128 w2 = _mm_loadu_ps(nVec4._w.data());

		__m128 x1x2 = _mm_add_ps(x1, x2);
		__m128 y1y2 = _mm_add_ps(y1, y2);
		__m128 z1z2 = _mm_add_ps(z1, z2);
		__m128 w1w2 = _mm_add_ps(w1, w2);

		_mm_storeu_ps(result._x.data(), x1x2);
		_mm_storeu_ps(result._y.data(), y1y2);
		_mm_storeu_ps(result._z.data(), z1z2);
		_mm_storeu_ps(result._w.data(), w1w2);

		return result;
	}

	template<>
	inline FourVec4F& FourVec4F::operator+=(const FourVec4F& nVec4) noexcept
	{
		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 x2 = _mm_loadu_ps(nVec4._x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 y2 = _mm_loadu_ps(nVec4._y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 z2 = _mm_loadu_ps(nVec4._z.data());
		__m128 w1 = _mm_loadu_ps(_w.data());
		__m128 w2 = _mm_loadu_ps(nVec4._w.data());

		__m128 x1x2 = _mm_add_ps(x1, x2);
		__m128 y1y2 = _mm_add_ps(y1, y2);
		__m128 z1z2 = _mm_add_ps(z1, z2);
		__m128 w1w2 = _mm_add_ps(w1, w2);

		_mm_storeu_ps(_x.data(), x1x2);
		_mm_storeu_ps(_y.data(), y1y2);
		_mm_storeu_ps(_z.data(), z1z2);
		_mm_storeu_ps(_w.data(), w1w2);

		return *this;
	}

	template<>
	[[nodiscard]] NOALIAS inline FourVec4F FourVec4F::operator-() const noexcept
	{
		FourVec4F result = FourVec4F();

		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 w1 = _mm_loadu_ps(_w.data());

		__m128 x1x2 = _mm_sub_ps(_mm_setzero_ps(), x1);
		__m128 y1y2 = _mm_sub_ps(_mm_setzero_ps(), y1);
		__m128 z1z2 = _mm_sub_ps(_mm_setzero_ps(), z1);
		__m128 w1w2 = _mm_sub_ps(_mm_setzero_ps(), w1);

		_mm_storeu_ps(result._x.data(), x1x2);
		_mm_storeu_ps(result._y.data(), y1y2);
		_mm_storeu_ps(result._z.data(), z1z2);
		_mm_storeu_ps(result._w.data(), w1w2);

		return result;
	}

	template<>
	[[nodiscard]] NOALIAS inline FourVec4F FourVec4F::operator-(const FourVec4F& nVec4) const noexcept
	{
		FourVec4F result = FourVec4F();

		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 x2 = _mm_loadu_ps(nVec4._x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 y2 = _mm_loadu_ps(nVec4._y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 z2 = _mm_loadu_ps(nVec4._z.data());
		__m128 w1 = _mm_loadu_ps(_w.data());
		__m128 w2 = _mm_loadu_ps(nVec4._w.data());

		__m128 x1x2 = _mm_sub_ps(x1, x2);
		__m128 y1y2 = _mm_sub_ps(y1, y2);
		__m128 z1z2 = _mm_sub_ps(z1, z2);
		__m128 w1w2 = _mm_sub_ps(w1, w2);

		_mm_storeu_ps(result._x.data(), x1x2);
		_mm_storeu_ps(result._y.data(), y1y2);
		_mm_storeu_ps(result._z.data(), z1z2);
		_mm_storeu_ps(result._w.data(), w1w2);

		return result;
	}

	template<>
	inline FourVec4F& FourVec4F::operator-=(const FourVec4F& nVec4) noexcept
	{
		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 x2 = _mm_loadu_ps(nVec4._x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 y2 = _mm_loadu_ps(nVec4._y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 z2 = _mm_loadu_ps(nVec4._z.data());
		__m128 w1 = _mm_loadu_ps(_w.data());
		__m128 w2 = _mm_loadu_ps(nVec4._w.data());

		__m128 x1x2 = _mm_sub_ps(x1, x2);
		__m128 y1y2 = _mm_sub_ps(y1, y2);
		__m128 z1z2 = _mm_sub_ps(z1, z2);
		__m128 w1w2 = _mm_sub_ps(w1, w2);

		_mm_storeu_ps(_x.data(), x1x2);
		_mm_storeu_ps(_y.data(), y1y2);
		_mm_storeu_ps(_z.data(), z1z2);
		_mm_storeu_ps(_w.data(), w1w2);

		return *this;
	}

	template<>
	[[nodiscard]] NOALIAS inline FourVec4F FourVec4F::operator*(const FourVec4F& nVec4) const noexcept
	{
		FourVec4F result = FourVec4F();

		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 x2 = _mm_loadu_ps(nVec4._x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 y2 = _mm_loadu_ps(nVec4._y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 z2 = _mm_loadu_ps(nVec4._z.data());
		__m128 w1 = _mm_loadu_ps(_w.data());
		__m128 w2 = _mm_loadu_ps(nVec4._w.data());

		__m128 x1x2 = _mm_mul_ps(x1, x2);
		__m128 y1y2 = _mm_mul_ps(y1, y2);
		__m128 z1z2 = _mm_mul_ps(z1, z2);
		__m128 w1w2 = _mm_mul_ps(w1, w2);

		_mm_storeu_ps(result._x.data(), x1x2);
		_mm_storeu_ps(result._y.data(), y1y2);
		_mm_storeu_ps(result._z.data(), z1z2);
		_mm_storeu_ps(result._w.data(), w1w2);

		return result;
	}

	template<>
	inline FourVec4F& FourVec4F::operator*=(const FourVec4F& nVec4) noexcept
	{
		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 x2 = _mm_loadu_ps(nVec4._x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 y2 = _mm_loadu_ps(nVec4._y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 z2 = _mm_loadu_ps(nVec4._z.data());
		__m128 w1 = _mm_loadu_ps(_w.data());
		__m128 w2 = _mm_loadu_ps(nVec4._w.data());

		__m128 x1x2 = _mm_mul_ps(x1, x2);
		__m128 y1y2 = _mm_mul_ps(y1, y2);
		__m128 z1z2 = _mm_mul_ps(z1, z2);
		__m128 w1w2 = _mm_mul_ps(w1, w2);

		_mm_storeu_ps(_x.data(), x1x2);
		_mm_storeu_ps(_y.data(), y1y2);
		_mm_storeu_ps(_z.data(), z1z2);
		_mm_storeu_ps(_w.data(), w1w2);

		return *this;
	}

	template<>
	[[nodiscard]] inline FourVec4F FourVec4F::operator/(const FourVec4F& nVec4) const
	{
		FourVec4F result = FourVec4F();

		for (int i = 0; i < 4; i++)
		{
			if (nVec4._x[i] == 0 || nVec4._y[i] == 0 || nVec4._z[i] == 0 || nVec4._w[i] == 0)
			{
				throw DivisionByZeroException();
			}
		}

		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 x2 = _mm_loadu_ps(nVec4._x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 y2 = _mm_loadu_ps(nVec4._y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 z2 = _mm_loadu_ps(nVec4._z.data());
		__m128 w1 = _mm_loadu_ps(_w.data());
		__m128 w2 = _mm_loadu_ps(nVec4._w.data());

		__m128 x1x2 = _mm_div_ps(x1, x2);
		__m128 y1y2 = _mm_div_ps(y1, y2);
		__m128 z1z2 = _mm_div_ps(z1, z2);
		__m128 w1w2 = _mm_div_ps(w1, w2);

		_mm_storeu_ps(result._x.data(), x1x2);
		_mm_storeu_ps(result._y.data(), y1y2);
		_mm_storeu_ps(result._z.data(), z1z2);
		_mm_storeu_ps(result._w.data(), w1w2);

		return result;
	}

	template<>
	inline FourVec4F& FourVec4F::operator/=(const FourVec4F& nVec4)
	{
		for (int i = 0; i < 4; i++)
		{
			if (nVec4._x[i] == 0 || nVec4._y[i] == 0 || nVec4._z[i] == 0 || nVec4._w[i] == 0)
			{
				throw DivisionByZeroException();
			}
		}

		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 x2 = _mm_loadu_ps(nVec4._x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 y2 = _mm_loadu_ps(nVec4._y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 z2 = _mm_loadu_ps(nVec4._z.data());
		__m128 w1 = _mm_loadu_ps(_w.data());
		__m128 w2 = _mm_loadu_ps(nVec4._w.data());

		__m128 x1x2 = _mm_div_ps(x1, x2);
		__m128 y1y2 = _mm_div_ps(y1, y2);
		__m128 z1z2 = _mm_div_ps(z1, z2);
		__m128 w1w2 = _mm_div_ps(w1, w2);

		_mm_storeu_ps(_x.data(), x1x2);
		_mm_storeu_ps(_y.data(), y1y2);
		_mm_storeu_ps(_z.data(), z1z2);
		_mm_storeu_ps(_w.data(), w1w2);

		return *this;
	}

	template<>
	[[nodiscard]] NOALIAS inline std::array<float, 4> FourVec4F::Dot(const NVec4<float, 4>& nV1, const NVec4<float, 4>& nV2) noexcept
	{
		std::array<float, 4> result = std::array<float, 4>();

		__m128 x1 = _mm_loadu_ps(nV1._x.data());
		__m128 x2 = _mm_loadu_ps(nV2._x.data());
		__m128 y1 = _mm_loadu_ps(nV1._y.data());
		__m128 y2 = _mm_loadu_ps(nV2._y.data());
		__m128 z1 = _mm_loadu_ps(nV1._z.data());
		__m128 z2 = _mm_loadu_ps(nV2._z.data());
		__m128 w1 = _mm_loadu_ps(nV1._w.data());
		__m128 w2 = _mm_loadu_ps(nV2._w.data());

		__m128 x1x2 = _mm_mul_ps(x1, x2);
		__m128 y1y2 = _mm_mul_ps(y1, y2);
		__m128 z1z2 = _mm_mul_ps(z1, z2);
		__m128 w1w2 = _mm_mul_ps(w1, w2);

		__m128 x1x2y1y2 = _mm_add_ps(x1x2, y1y2);
		__m128 z1z2w1w2 = _mm_add_ps(z1z2, w1w2);

		__m128 dot = _mm_add_ps(x1x2y1y2, z1z2w1w2);

		_mm_storeu_ps(result.data(), dot);

		return result;
	}

	template<>
	[[nodiscard]] NOALIAS inline std::array<float, 4> FourVec4F::SquareMagnitude() const noexcept
	{
		std::array<float, 4> result = std::array<float, 4>();

		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 w1 = _mm_loadu_ps(_w.data());

		__m128 x1x2 = _mm_mul_ps(x1, x1);
		__m128 y1y2 = _mm_mul_ps(y1, y1);
		__m128 z1z2 = _mm_mul_ps(z1, z1);
		__m128 w1w2 = _mm_mul_ps(w1, w1);

		__m128 x1x2y1y2 = _mm_add_ps(x1x2, y1y2);
		__m128 z1z2w1w2 = _mm_add_ps(z1z2, w1w2);

		__m128 squareMagnitude = _mm_add_ps(x1x2y1y2, z1z2w1w2);

		_mm_storeu_ps(result.data(), squareMagnitude);

		return result;
	}

	template<>
	[[nodiscard]] NOALIAS inline std::array<float, 4> FourVec4F::Magnitude() const noexcept
	{
		std::array<float, 4> result = std::array<float, 4>();

		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 w1 = _mm_loadu_ps(_w.data());

		__m128 x1x2 = _mm_mul_ps(x1, x1);
		__m128 y1y2 = _mm_mul_ps(y1, y1);
		__m128 z1z2 = _mm_mul_ps(z1, z1);
		__m128 w1w2 = _mm_mul_ps(w1, w1);

		__m128 x1x2y1y2 = _mm_add_ps(x1x2, y1y2);
		__m128 z1z2w1w2 = _mm_add_ps(z1z2, w1w2);

		__m128 squareMagnitude = _mm_add_ps(x1x2y1y2, z1z2w1w2);

		__m128 magnitude = _mm_sqrt_ps(squareMagnitude);

		_mm_storeu_ps(result.data(), magnitude);

		return result;
	}

	template<>
	[[nodiscard]] NOALIAS inline std::array<float, 4> FourVec4F::Normalized() const
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
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 w1 = _mm_loadu_ps(_w.data());

		__m128 x1x2 = _mm_mul_ps(x1, x1);
		__m128 y1y2 = _mm_mul_ps(y1, y1);
		__m128 z1z2 = _mm_mul_ps(z1, z1);
		__m128 w1w2 = _mm_mul_ps(w1, w1);

		__m128 x1x2y1y2 = _mm_add_ps(x1x2, y1y2);
		__m128 z1z2w1w2 = _mm_add_ps(z1z2, w1w2);

		__m128 squareMagnitude = _mm_add_ps(x1x2y1y2, z1z2w1w2);

		__m128 magn = _mm_sqrt_ps(squareMagnitude);

		__m128 x1x2y1y2SqrtReciprocal = _mm_rcp_ps(magn);

		_mm_storeu_ps(result.data(), x1x2y1y2SqrtReciprocal);

		return result;
	}

#pragma endregion FourVec4F

#pragma region FourVec4I

	template<>
	[[nodiscard]] NOALIAS inline FourVec4I FourVec4I::operator+(const FourVec4I& nVec4) const noexcept
	{
		FourVec4I result = FourVec4I();

		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._z.data()));
		__m128i w1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_w.data()));
		__m128i w2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._w.data()));

		__m128i x1x2 = _mm_add_epi32(x1, x2);
		__m128i y1y2 = _mm_add_epi32(y1, y2);
		__m128i z1z2 = _mm_add_epi32(z1, z2);
		__m128i w1w2 = _mm_add_epi32(w1, w2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._x.data()), x1x2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._y.data()), y1y2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._z.data()), z1z2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._w.data()), w1w2);

		return result;
	}

	template<>
	inline FourVec4I& FourVec4I::operator+=(const FourVec4I& nVec4) noexcept
	{
		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._z.data()));
		__m128i w1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_w.data()));
		__m128i w2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._w.data()));

		__m128i x1x2 = _mm_add_epi32(x1, x2);
		__m128i y1y2 = _mm_add_epi32(y1, y2);
		__m128i z1z2 = _mm_add_epi32(z1, z2);
		__m128i w1w2 = _mm_add_epi32(w1, w2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(_x.data()), x1x2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_y.data()), y1y2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_z.data()), z1z2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_w.data()), w1w2);

		return *this;
	}

	template<>
	[[nodiscard]] NOALIAS inline FourVec4I FourVec4I::operator-() const noexcept
	{
		FourVec4I result = FourVec4I();

		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i w1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_w.data()));

		__m128 x1x2 = _mm_sub_epi32(_mm_setzero_ps(), x1);
		__m128 y1y2 = _mm_sub_epi32(_mm_setzero_ps(), y1);
		__m128 z1z2 = _mm_sub_epi32(_mm_setzero_ps(), z1);
		__m128 w1w2 = _mm_sub_epi32(_mm_setzero_ps(), w1);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._x.data()), x1x2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._y.data()), y1y2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._z.data()), z1z2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._w.data()), w1w2);

		return result;
	}

	template<>
	[[nodiscard]] NOALIAS inline FourVec4I FourVec4I::operator-(const FourVec4I& nVec4) const noexcept
	{
		FourVec4I result = FourVec4I();

		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._z.data()));
		__m128i w1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_w.data()));
		__m128i w2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._w.data()));

		__m128i x1x2 = _mm_sub_epi32(x1, x2);
		__m128i y1y2 = _mm_sub_epi32(y1, y2);
		__m128i z1z2 = _mm_sub_epi32(z1, z2);
		__m128i w1w2 = _mm_sub_epi32(w1, w2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._x.data()), x1x2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._y.data()), y1y2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._z.data()), z1z2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._w.data()), w1w2);

		return result;
	}

	template<>
	inline FourVec4I& FourVec4I::operator-=(const FourVec4I& nVec4) noexcept
	{
		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._z.data()));
		__m128i w1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_w.data()));
		__m128i w2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._w.data()));

		__m128i x1x2 = _mm_sub_epi32(x1, x2);
		__m128i y1y2 = _mm_sub_epi32(y1, y2);
		__m128i z1z2 = _mm_sub_epi32(z1, z2);
		__m128i w1w2 = _mm_sub_epi32(w1, w2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(_x.data()), x1x2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_y.data()), y1y2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_z.data()), z1z2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_w.data()), w1w2);

		return *this;
	}

	template<>
	[[nodiscard]] NOALIAS inline FourVec4I FourVec4I::operator*(const FourVec4I& nVec4) const noexcept
	{
		FourVec4I result = FourVec4I();

		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._z.data()));
		__m128i w1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_w.data()));
		__m128i w2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._w.data()));

		__m128i x1x2 = _mm_mullo_epi32(x1, x2);
		__m128i y1y2 = _mm_mullo_epi32(y1, y2);
		__m128i z1z2 = _mm_mullo_epi32(z1, z2);
		__m128i w1w2 = _mm_mullo_epi32(w1, w2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._x.data()), x1x2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._y.data()), y1y2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._z.data()), z1z2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._w.data()), w1w2);

		return result;
	}

	template<>
	inline FourVec4I& FourVec4I::operator*=(const FourVec4I& nVec4) noexcept
	{
		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._z.data()));
		__m128i w1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_w.data()));
		__m128i w2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._w.data()));

		__m128i x1x2 = _mm_mullo_epi32(x1, x2);
		__m128i y1y2 = _mm_mullo_epi32(y1, y2);
		__m128i z1z2 = _mm_mullo_epi32(z1, z2);
		__m128i w1w2 = _mm_mullo_epi32(w1, w2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(_x.data()), x1x2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_y.data()), y1y2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_z.data()), z1z2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_w.data()), w1w2);

		return *this;
	}

#ifdef _mm_div_epi32

	template<>
	[[nodiscard]] inline FourVec4I FourVec4I::operator/(const FourVec4I& nVec4) const
	{
		FourVec4I result = FourVec4I();

		for (int i = 0; i < 4; i++)
		{
			if (nVec4._x[i] == 0 || nVec4._y[i] == 0 || nVec4._z[i] == 0 || nVec4._w[i] == 0)
			{
				throw DivisionByZeroException();
			}
		}

		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._z.data()));
		__m128i w1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_w.data()));
		__m128i w2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._w.data()));

		__m128 x1x2 = _mm_div_epi32(x1, x2);
		__m128 y1y2 = _mm_div_epi32(y1, y2);
		__m128 z1z2 = _mm_div_epi32(z1, z2);
		__m128 w1w2 = _mm_div_epi32(w1, w2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._x.data()), x1x2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._y.data()), y1y2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._z.data()), z1z2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._w.data()), w1w2);

		return result;
	}

	template<>
	inline FourVec4I& FourVec4I::operator/=(const FourVec4I& nVec4)
	{
		for (int i = 0; i < 4; i++)
		{
			if (nVec4._x[i] == 0 || nVec4._y[i] == 0 || nVec4._z[i] == 0 || nVec4._w[i] == 0)
			{
				throw DivisionByZeroException();
			}
		}

		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._z.data()));
		__m128i w1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_w.data()));
		__m128i w2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec4._w.data()));

		__m128 x1x2 = _mm_div_epi32(x1, x2);
		__m128 y1y2 = _mm_div_epi32(y1, y2);
		__m128 z1z2 = _mm_div_epi32(z1, z2);
		__m128 w1w2 = _mm_div_epi32(w1, w2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(_x.data()), x1x2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_y.data()), y1y2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_z.data()), z1z2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_w.data()), w1w2);

		return *this;
	}

#endif

	template<>
	[[nodiscard]] NOALIAS inline std::array<int, 4> FourVec4I::Dot(const NVec4<int, 4>& nV1, const NVec4<int, 4>& nV2) noexcept
	{
		std::array<int, 4> result = std::array<int, 4>();

		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nV1._x.data()));
		__m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nV2._x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nV1._y.data()));
		__m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nV2._y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nV1._z.data()));
		__m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nV2._z.data()));
		__m128i w1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nV1._w.data()));
		__m128i w2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nV2._w.data()));

		__m128i x1x2 = _mm_mullo_epi32(x1, x2);
		__m128i y1y2 = _mm_mullo_epi32(y1, y2);
		__m128i z1z2 = _mm_mullo_epi32(z1, z2);
		__m128i w1w2 = _mm_mullo_epi32(w1, w2);

		__m128i x1x2y1y2 = _mm_add_epi32(x1x2, y1y2);
		__m128i z1z2w1w2 = _mm_add_epi32(z1z2, w1w2);

		__m128i dot = _mm_add_epi32(x1x2y1y2, z1z2w1w2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(result.data()), dot);

		return result;
	}

	template<>
	[[nodiscard]] NOALIAS inline std::array<int, 4> FourVec4I::SquareMagnitude() const noexcept
	{
		std::array<int, 4> result = std::array<int, 4>();

		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i w1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_w.data()));

		__m128i x1x2 = _mm_mullo_epi32(x1, x1);
		__m128i y1y2 = _mm_mullo_epi32(y1, y1);
		__m128i z1z2 = _mm_mullo_epi32(z1, z1);
		__m128i w1w2 = _mm_mullo_epi32(w1, w1);

		__m128i x1x2y1y2 = _mm_add_epi32(x1x2, y1y2);
		__m128i z1z2w1w2 = _mm_add_epi32(z1z2, w1w2);

		__m128i squareMagnitude = _mm_add_epi32(x1x2y1y2, z1z2w1w2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(result.data()), squareMagnitude);

		return result;
	}

#ifdef _mm_sqrt_epi32

	template<>
	[[nodiscard]] NOALIAS inline std::array<int, 4> FourVec4I::Magnitude() const noexcept
	{
		std::array<int, 4> result = std::array<int, 4>();

		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i w1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_w.data()));

		__m128i x1x2 = _mm_mullo_epi32(x1, x1);
		__m128i y1y2 = _mm_mullo_epi32(y1, y1);
		__m128i z1z2 = _mm_mullo_epi32(z1, z1);
		__m128i w1w2 = _mm_mullo_epi32(w1, w1);

		__m128i x1x2y1y2 = _mm_add_epi32(x1x2, y1y2);
		__m128i z1z2w1w2 = _mm_add_epi32(z1z2, w1w2);

		__m128i squareMagnitude = _mm_add_epi32(x1x2y1y2, z1z2w1w2);

		__m128i magnitude = _mm_sqrt_epi32(squareMagnitude)

		_mm_storeu_si128(reinterpret_cast<__m128i*>(result.data()), magnitude);

		return result;
	}

	template <>
	[[nodiscard]] NOALIAS inline std::array<int, 4> FourVec4I::Normalized() const
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
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i w1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_w.data()));

		__m128i x1x2 = _mm_mullo_epi32(x1, x1);
		__m128i y1y2 = _mm_mullo_epi32(y1, y1);
		__m128i z1z2 = _mm_mullo_epi32(z1, z1);
		__m128i w1w2 = _mm_mullo_epi32(w1, w1);

		__m128i x1x2y1y2 = _mm_add_epi32(x1x2, y1y2);
		__m128i z1z2w1w2 = _mm_add_epi32(z1z2, w1w2);

		__m128i squareMagnitude = _mm_add_epi32(x1x2y1y2, z1z2w1w2);

		__m128i magn = _mm_sqrt_epi32(squareMagnitude)

		__m128i x1x2y1y2SqrtReciprocal = _mm_rcp_epi32(magn);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(result.data()), x1x2y1y2SqrtReciprocal);

		return result;
	}

#endif

#pragma endregion FourVec4I

#endif

}