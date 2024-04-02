/**
 * @author Constantin
 */

#pragma once

#include "Intrinsics.h"
#include "Definition.h"
#include "Vec3.h"

#include <array>

namespace Math
{
	template<typename T, int N>
	class NVec3
	{
	public:
		constexpr NVec3() = default;

		constexpr NVec3(const NVec3<T, N>& vec) = default;

		constexpr explicit NVec3(const std::array<Math::Vec3<T>, N> vecs) noexcept
		{
			for (int i = 0; i < N; i++)
			{
				_x[i] = vecs[i].X;
				_y[i] = vecs[i].Y;
				_z[i] = vecs[i].Z;
			}
		}

		constexpr explicit NVec3(const Math::Vec3<T> vec) noexcept
		{
			for (int i = 0; i < N; i++)
			{
				_x[i] = vec.X;
				_y[i] = vec.Y;
				_z[i] = vec.Z;
			}
		}

	private:
		std::array<T, N> _x = std::array<T, N>();
		std::array<T, N> _y = std::array<T, N>();
		std::array<T, N> _z = std::array<T, N>();

	public:
		[[nodiscard]] NOALIAS const auto& X() const noexcept
		{ return _x; }

		[[nodiscard]] NOALIAS const auto& Y() const noexcept
		{ return _y; }

		[[nodiscard]] NOALIAS const auto& Z() const noexcept
		{ return _z; }

		[[nodiscard]] NOALIAS NVec3<T, N> operator+(const NVec3<T, N>& nVec3) const noexcept
		{
			NVec3<T, N> result = NVec3<T, N>();

			for (int i = 0; i < N; i++)
			{
				result._x[i] = _x[i] + nVec3._x[i];
				result._y[i] = _y[i] + nVec3._y[i];
				result._z[i] = _z[i] + nVec3._z[i];
			}

			return result;
		}

		NVec3<T, N>& operator+=(const NVec3<T, N>& nVec3) noexcept
		{
			for (int i = 0; i < N; i++)
			{
				_x[i] += nVec3._x[i];
				_y[i] += nVec3._y[i];
				_z[i] += nVec3._z[i];
			}

			return *this;
		}

		[[nodiscard]] NOALIAS NVec3<T, N> operator-() const noexcept
		{
			NVec3<T, N> result = NVec3<T, N>();

			for (int i = 0; i < N; i++)
			{
				result._x[i] = -_x[i];
				result._y[i] = -_y[i];
				result._z[i] = -_z[i];
			}

			return result;
		}

		[[nodiscard]] NOALIAS NVec3<T, N> operator-(const NVec3<T, N>& nVec3) const noexcept
		{
			NVec3<T, N> result = NVec3<T, N>();

			for (int i = 0; i < N; i++)
			{
				result._x[i] = _x[i] - nVec3._x[i];
				result._y[i] = _y[i] - nVec3._y[i];
				result._z[i] = _z[i] - nVec3._z[i];
			}

			return result;
		}

		NVec3<T, N>& operator-=(const NVec3<T, N>& nVec3) noexcept
		{
			for (int i = 0; i < N; i++)
			{
				_x[i] -= nVec3._x[i];
				_y[i] -= nVec3._y[i];
				_z[i] -= nVec3._z[i];
			}

			return *this;
		}

		[[nodiscard]] NOALIAS NVec3<T, N> operator*(const NVec3<T, N>& nVec3) const noexcept
		{
			NVec3<T, N> result = NVec3<T, N>();

			for (int i = 0; i < N; i++)
			{
				result._x[i] = _x[i] * nVec3._x[i];
				result._y[i] = _y[i] * nVec3._y[i];
				result._z[i] = _z[i] * nVec3._z[i];
			}

			return result;
		}

		NVec3<T, N>& operator*=(const NVec3<T, N>& nVec3) noexcept
		{
			for (int i = 0; i < N; i++)
			{
				_x[i] *= nVec3._x[i];
				_y[i] *= nVec3._y[i];
				_z[i] *= nVec3._z[i];
			}

			return *this;
		}

		[[nodiscard]] NVec3<T, N> operator/(const NVec3<T, N>& nVec3) const
		{
			NVec3<T, N> result = NVec3<T, N>();

			for (int i = 0; i < N; i++)
			{
				if (nVec3._x[i] == 0 || nVec3._y[i] == 0 || nVec3._z[i] == 0)
				{
					throw DivisionByZeroException();
				}

				result._x[i] = _x[i] / nVec3._x[i];
				result._y[i] = _y[i] / nVec3._y[i];
				result._z[i] = _z[i] / nVec3._z[i];
			}

			return result;
		}

		NVec3<T, N>& operator/=(const NVec3<T, N>& nVec3)
		{
			for (int i = 0; i < N; i++)
			{
				if (nVec3._x[i] == 0 || nVec3._y[i] == 0 || nVec3._z[i] == 0)
				{
					throw DivisionByZeroException();
				}

				_x[i] /= nVec3._x[i];
				_y[i] /= nVec3._y[i];
				_z[i] /= nVec3._z[i];
			}

			return *this;
		}

		[[nodiscard]] NOALIAS NVec3<T, N> operator*(const T* array1N) const noexcept
		{
			NVec3<T, N> result = NVec3<T, N>();

			for (int i = 0; i < N; i++)
			{
				result._x[i] = _x[i] * array1N[i];
				result._y[i] = _y[i] * array1N[i];
				result._z[i] = _z[i] * array1N[i];
			}

			return result;
		}

		NVec3<T, N>& operator*=(const T* array1N) noexcept
		{
			for (int i = 0; i < N; i++)
			{
				_x[i] *= array1N[i];
				_y[i] *= array1N[i];
				_z[i] *= array1N[i];
			}

			return *this;
		}

		[[nodiscard]] NVec3<T, N> operator/(const T* array1N) const
		{
			NVec3<T, N> result = NVec3<T, N>();

			for (int i = 0; i < N; i++)
			{
				if (array1N[i] == 0)
				{
					throw DivisionByZeroException();
				}

				result._x[i] = _x[i] / array1N[i];
				result._y[i] = _y[i] / array1N[i];
				result._z[i] = _z[i] / array1N[i];
			}

			return result;
		}

		NVec3<T, N>& operator/=(const T* array1N)
		{
			for (int i = 0; i < N; i++)
			{
				if (array1N[i] == 0)
				{
					throw DivisionByZeroException();
				}

				_x[i] /= array1N[i];
				_y[i] /= array1N[i];
				_z[i] / array1N[i];
			}

			return *this;
		}

		static std::array<T, N> Dot(const NVec3<T, N>& nV1, const NVec3<T, N>& nV2) noexcept
		{
			std::array<T, N> result = std::array<T, N>();

			for (int i = 0; i < N; i++)
			{
				result[i] = nV1._x[i] * nV2._x[i] + nV1._y[i] * nV2._y[i] + nV1._z[i] * nV2._z[i];
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
	};

	using FourVec3F = NVec3<float, 4>;
	using FourVec3I = NVec3<int, 4>;

#ifdef __SSE__

#pragma region FourVec3F

	template<>
	[[nodiscard]] NOALIAS inline FourVec3F FourVec3F::operator+(const FourVec3F& nVec2) const noexcept
	{
		FourVec3F result = FourVec3F();

		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 x2 = _mm_loadu_ps(nVec2._x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 y2 = _mm_loadu_ps(nVec2._y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 z2 = _mm_loadu_ps(nVec2._z.data());

		__m128 x1x2 = _mm_add_ps(x1, x2);
		__m128 y1y2 = _mm_add_ps(y1, y2);
		__m128 z1z2 = _mm_add_ps(z1, z2);

		_mm_storeu_ps(result._x.data(), x1x2);
		_mm_storeu_ps(result._y.data(), y1y2);
		_mm_storeu_ps(result._z.data(), z1z2);

		return result;
	}

	template<>
	inline FourVec3F& FourVec3F::operator+=(const FourVec3F& nVec2) noexcept
	{
		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 x2 = _mm_loadu_ps(nVec2._x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 y2 = _mm_loadu_ps(nVec2._y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 z2 = _mm_loadu_ps(nVec2._z.data());


		__m128 x1x2 = _mm_add_ps(x1, x2);
		__m128 y1y2 = _mm_add_ps(y1, y2);
		__m128 z1z2 = _mm_add_ps(z1, z2);

		_mm_storeu_ps(_x.data(), x1x2);
		_mm_storeu_ps(_y.data(), y1y2);
		_mm_storeu_ps(_z.data(), z1z2);

		return *this;
	}

	template<>
	[[nodiscard]] NOALIAS inline FourVec3F FourVec3F::operator-() const noexcept
	{
		FourVec3F result = FourVec3F();

		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());

		__m128 x1x2 = _mm_sub_ps(_mm_setzero_ps(), x1);
		__m128 y1y2 = _mm_sub_ps(_mm_setzero_ps(), y1);
		__m128 z1z2 = _mm_sub_ps(_mm_setzero_ps(), z1);

		_mm_storeu_ps(result._x.data(), x1x2);
		_mm_storeu_ps(result._y.data(), y1y2);
		_mm_storeu_ps(result._z.data(), z1z2);

		return result;
	}

	template<>
	[[nodiscard]] NOALIAS inline FourVec3F FourVec3F::operator-(const FourVec3F& nVec2) const noexcept
	{
		FourVec3F result = FourVec3F();

		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 x2 = _mm_loadu_ps(nVec2._x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 y2 = _mm_loadu_ps(nVec2._y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 z2 = _mm_loadu_ps(nVec2._z.data());

		__m128 x1x2 = _mm_sub_ps(x1, x2);
		__m128 y1y2 = _mm_sub_ps(y1, y2);
		__m128 z1z2 = _mm_sub_ps(z1, z2);

		_mm_storeu_ps(result._x.data(), x1x2);
		_mm_storeu_ps(result._y.data(), y1y2);
		_mm_storeu_ps(result._z.data(), z1z2);

		return result;
	}

	template<>
	inline FourVec3F& FourVec3F::operator-=(const FourVec3F& nVec2) noexcept
	{
		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 x2 = _mm_loadu_ps(nVec2._x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 y2 = _mm_loadu_ps(nVec2._y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 z2 = _mm_loadu_ps(nVec2._z.data());

		__m128 x1x2 = _mm_sub_ps(x1, x2);
		__m128 y1y2 = _mm_sub_ps(y1, y2);
		__m128 z1z2 = _mm_sub_ps(z1, z2);

		_mm_storeu_ps(_x.data(), x1x2);
		_mm_storeu_ps(_y.data(), y1y2);
		_mm_storeu_ps(_z.data(), z1z2);

		return *this;
	}

	template<>
	[[nodiscard]] NOALIAS inline FourVec3F FourVec3F::operator*(const FourVec3F& nVec2) const noexcept
	{
		FourVec3F result = FourVec3F();

		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 x2 = _mm_loadu_ps(nVec2._x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 y2 = _mm_loadu_ps(nVec2._y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 z2 = _mm_loadu_ps(nVec2._z.data());

		__m128 x1x2 = _mm_mul_ps(x1, x2);
		__m128 y1y2 = _mm_mul_ps(y1, y2);
		__m128 z1z2 = _mm_mul_ps(z1, z2);

		_mm_storeu_ps(result._x.data(), x1x2);
		_mm_storeu_ps(result._y.data(), y1y2);
		_mm_storeu_ps(result._z.data(), z1z2);

		return result;
	}

	template<>
	inline FourVec3F& FourVec3F::operator*=(const FourVec3F& nVec2) noexcept
	{
		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 x2 = _mm_loadu_ps(nVec2._x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 y2 = _mm_loadu_ps(nVec2._y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 z2 = _mm_loadu_ps(nVec2._z.data());

		__m128 x1x2 = _mm_mul_ps(x1, x2);
		__m128 y1y2 = _mm_mul_ps(y1, y2);
		__m128 z1z2 = _mm_mul_ps(z1, z2);

		_mm_storeu_ps(_x.data(), x1x2);
		_mm_storeu_ps(_y.data(), y1y2);
		_mm_storeu_ps(_z.data(), z1z2);

		return *this;
	}

	template<>
	[[nodiscard]] inline FourVec3F FourVec3F::operator/(const FourVec3F& nVec2) const
	{
		FourVec3F result = FourVec3F();

		for (int i = 0; i < 4; i++)
		{
			if (nVec2._x[i] == 0 || nVec2._y[i] == 0 || nVec2._z[i] == 0)
			{
				throw DivisionByZeroException();
			}
		}

		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 x2 = _mm_loadu_ps(nVec2._x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 y2 = _mm_loadu_ps(nVec2._y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 z2 = _mm_loadu_ps(nVec2._z.data());

		__m128 x1x2 = _mm_div_ps(x1, x2);
		__m128 y1y2 = _mm_div_ps(y1, y2);
		__m128 z1z2 = _mm_div_ps(z1, z2);

		_mm_storeu_ps(result._x.data(), x1x2);
		_mm_storeu_ps(result._y.data(), y1y2);
		_mm_storeu_ps(result._z.data(), z1z2);

		return result;
	}

	template<>
	inline FourVec3F& FourVec3F::operator/=(const FourVec3F& nVec2)
	{
		for (int i = 0; i < 4; i++)
		{
			if (nVec2._x[i] == 0 || nVec2._y[i] == 0 || nVec2._z[i] == 0)
			{
				throw DivisionByZeroException();
			}
		}

		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 x2 = _mm_loadu_ps(nVec2._x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 y2 = _mm_loadu_ps(nVec2._y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());
		__m128 z2 = _mm_loadu_ps(nVec2._z.data());

		__m128 x1x2 = _mm_div_ps(x1, x2);
		__m128 y1y2 = _mm_div_ps(y1, y2);
		__m128 z1z2 = _mm_div_ps(z1, z2);

		_mm_storeu_ps(_x.data(), x1x2);
		_mm_storeu_ps(_y.data(), y1y2);
		_mm_storeu_ps(_z.data(), z1z2);

		return *this;
	}

	template<>
	[[nodiscard]] NOALIAS inline std::array<float, 4> FourVec3F::Dot(const NVec3<float, 4>& nV1, const NVec3<float, 4>& nV2) noexcept
	{
		std::array<float, 4> result = std::array<float, 4>();

		__m128 x1 = _mm_loadu_ps(nV1._x.data());
		__m128 y1 = _mm_loadu_ps(nV1._y.data());
		__m128 z1 = _mm_loadu_ps(nV1._z.data());

		__m128 x2 = _mm_loadu_ps(nV2._x.data());
		__m128 y2 = _mm_loadu_ps(nV2._y.data());
		__m128 z2 = _mm_loadu_ps(nV2._z.data());

		__m128 x1x2 = _mm_mul_ps(x1, x2);
		__m128 y1y2 = _mm_mul_ps(y1, y2);
		__m128 z1z2 = _mm_mul_ps(z1, z2);

		__m128 resultPartial = _mm_add_ps(x1x2, y1y2);
		resultPartial = _mm_add_ps(resultPartial, z1z2);

		_mm_storeu_ps(result.data(), resultPartial);

		return result;
	}

	template<>
	[[nodiscard]] NOALIAS inline std::array<float, 4> FourVec3F::SquareMagnitude() const noexcept
	{
		std::array<float, 4> result = std::array<float, 4>();

		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());

		__m128 x1x2 = _mm_mul_ps(x1, x1);
		__m128 y1y2 = _mm_mul_ps(y1, y1);
		__m128 z1z2 = _mm_mul_ps(z1, z1);

		__m128 resultPartial = _mm_add_ps(x1x2, y1y2);
		resultPartial = _mm_add_ps(resultPartial, z1z2);

		_mm_storeu_ps(result.data(), resultPartial);

		return result;
	}

	template<>
	[[nodiscard]] NOALIAS inline std::array<float, 4> FourVec3F::Magnitude() const noexcept
	{
		std::array<float, 4> result = std::array<float, 4>();

		__m128 x1 = _mm_loadu_ps(_x.data());
		__m128 y1 = _mm_loadu_ps(_y.data());
		__m128 z1 = _mm_loadu_ps(_z.data());

		__m128 x1x2 = _mm_mul_ps(x1, x1);
		__m128 y1y2 = _mm_mul_ps(y1, y1);
		__m128 z1z2 = _mm_mul_ps(z1, z1);

		__m128 resultPartial = _mm_add_ps(x1x2, y1y2);
		resultPartial = _mm_add_ps(resultPartial, z1z2);

		__m128 resultSqrt = _mm_sqrt_ps(resultPartial);

		_mm_storeu_ps(result.data(), resultSqrt);

		return result;
	}

	template<>
	[[nodiscard]] NOALIAS inline std::array<float, 4> FourVec3F::Normalized() const
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

		__m128 x1x2 = _mm_mul_ps(x1, x1);
		__m128 y1y2 = _mm_mul_ps(y1, y1);
		__m128 z1z2 = _mm_mul_ps(z1, z1);

		__m128 resultPartial = _mm_add_ps(x1x2, y1y2);
		resultPartial = _mm_add_ps(resultPartial, z1z2);

		__m128 resultSqrt = _mm_sqrt_ps(resultPartial);

		__m128 resultSqrtReciprocal = _mm_rcp_ps(resultSqrt);

		_mm_storeu_ps(result.data(), resultSqrtReciprocal);

		return result;
	}

#pragma endregion

#pragma region FourVec3I

	template<>
	[[nodiscard]] NOALIAS inline FourVec3I FourVec3I::operator+(const FourVec3I& nVec2) const noexcept
	{
		FourVec3I result = FourVec3I();

		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._z.data()));

		__m128i x1x2 = _mm_add_epi32(x1, x2);
		__m128i y1y2 = _mm_add_epi32(y1, y2);
		__m128i z1z2 = _mm_add_epi32(z1, z2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._x.data()), x1x2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._y.data()), y1y2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._z.data()), z1z2);

		return result;
	}

	template<>
	inline FourVec3I& FourVec3I::operator+=(const FourVec3I& nVec2) noexcept
	{
		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._z.data()));

		__m128i x1x2 = _mm_add_epi32(x1, x2);
		__m128i y1y2 = _mm_add_epi32(y1, y2);
		__m128i z1z2 = _mm_add_epi32(z1, z2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(_x.data()), x1x2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_y.data()), y1y2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_z.data()), z1z2);

		return *this;
	}

	template<>
	[[nodiscard]] NOALIAS inline FourVec3I FourVec3I::operator-() const noexcept
	{
		FourVec3I result = FourVec3I();

		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));

		__m128i x1x2 = _mm_sub_epi32(_mm_setzero_si128(), x1);
		__m128i y1y2 = _mm_sub_epi32(_mm_setzero_si128(), y1);
		__m128i z1z2 = _mm_sub_epi32(_mm_setzero_si128(), z1);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._x.data()), x1x2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._y.data()), y1y2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._z.data()), z1z2);

		return result;
	}

	template<>
	[[nodiscard]] NOALIAS inline FourVec3I FourVec3I::operator-(const FourVec3I& nVec2) const noexcept
	{
		FourVec3I result = FourVec3I();

		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._z.data()));

		__m128i x1x2 = _mm_sub_epi32(x1, x2);
		__m128i y1y2 = _mm_sub_epi32(y1, y2);
		__m128i z1z2 = _mm_sub_epi32(z1, z2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._x.data()), x1x2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._y.data()), y1y2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._z.data()), z1z2);

		return result;
	}

	template<>
	inline FourVec3I& FourVec3I::operator-=(const FourVec3I& nVec2) noexcept
	{
		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._z.data()));

		__m128i x1x2 = _mm_sub_epi32(x1, x2);
		__m128i y1y2 = _mm_sub_epi32(y1, y2);
		__m128i z1z2 = _mm_sub_epi32(z1, z2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(_x.data()), x1x2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_y.data()), y1y2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_z.data()), z1z2);

		return *this;
	}

	template<>
	[[nodiscard]] NOALIAS inline FourVec3I FourVec3I::operator*(const FourVec3I& nVec2) const noexcept
	{
		FourVec3I result = FourVec3I();

		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._z.data()));

		__m128i x1x2 = _mm_mullo_epi32(x1, x2);
		__m128i y1y2 = _mm_mullo_epi32(y1, y2);
		__m128i z1z2 = _mm_mullo_epi32(z1, z2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._x.data()), x1x2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._y.data()), y1y2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._z.data()), z1z2);

		return result;
	}

	template<>
	inline FourVec3I& FourVec3I::operator*=(const FourVec3I& nVec2) noexcept
	{
		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._z.data()));

		__m128i x1x2 = _mm_mullo_epi32(x1, x2);
		__m128i y1y2 = _mm_mullo_epi32(y1, y2);
		__m128i z1z2 = _mm_mullo_epi32(z1, z2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(_x.data()), x1x2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_y.data()), y1y2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_z.data()), z1z2);

		return *this;
	}

#ifdef _mm_div_epi32

	template <>
	[[nodiscard]] inline FourVec3I FourVec3I::operator/(const FourVec3I& nVec2) const
	{
		FourVec3I result = FourVec3I();

		for (int i = 0; i < 4; i++)
		{
			if (nVec2._x[i] == 0 || nVec2._y[i] == 0 || nVec2._z[i] == 0)
			{
				throw DivisionByZeroException();
			}
		}

		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._z.data()));

		__m128i x1x2 = _mm_div_epi32(x1, x2);
		__m128i y1y2 = _mm_div_epi32(y1, y2);
		__m128i z1z2 = _mm_div_epi32(z1, z2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._x.data()), x1x2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._y.data()), y1y2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(result._z.data()), z1z2);

		return result;
	}

	template <>
	inline FourVec3I& FourVec3I::operator/=(const FourVec3I& nVec2)
	{
		for (int i = 0; i < 4; i++)
		{
			if (nVec2._x[i] == 0 || nVec2._y[i] == 0 || nVec2._z[i] == 0)
			{
				throw DivisionByZeroException();
			}
		}

		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));
		__m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nVec2._z.data()));

		__m128i x1x2 = _mm_div_epi32(x1, x2);
		__m128i y1y2 = _mm_div_epi32(y1, y2);
		__m128i z1z2 = _mm_div_epi32(z1, z2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(_x.data()), x1x2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_y.data()), y1y2);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(_z.data()), z1z2);

		return *this;
	}

#endif

	template<>
	[[nodiscard]] NOALIAS inline std::array<int, 4> FourVec3I::Dot(const NVec3<int, 4>& nV1, const NVec3<int, 4>& nV2) noexcept
	{
		std::array<int, 4> result = std::array<int, 4>();

		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nV1._x.data()));
		__m128i x2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nV2._x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nV1._y.data()));
		__m128i y2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nV2._y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nV1._z.data()));
		__m128i z2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(nV2._z.data()));

		__m128i x1x2 = _mm_mullo_epi32(x1, x2);
		__m128i y1y2 = _mm_mullo_epi32(y1, y2);
		__m128i z1z2 = _mm_mullo_epi32(z1, z2);

		__m128i resultPartial = _mm_add_epi32(x1x2, y1y2);
		resultPartial = _mm_add_epi32(resultPartial, z1z2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(result.data()), resultPartial);

		return result;
	}

	template<>
	[[nodiscard]] NOALIAS inline std::array<int, 4> FourVec3I::SquareMagnitude() const noexcept
	{
		std::array<int, 4> result = std::array<int, 4>();

		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));

		__m128i x1x2 = _mm_mullo_epi32(x1, x1);
		__m128i y1y2 = _mm_mullo_epi32(y1, y1);
		__m128i z1z2 = _mm_mullo_epi32(z1, z1);

		__m128i resultPartial = _mm_add_epi32(x1x2, y1y2);
		resultPartial = _mm_add_epi32(resultPartial, z1z2);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(result.data()), resultPartial);

		return result;
	}

#ifdef _mm_sqrt_epi32

	template <>
	[[nodiscard]] NOALIAS inline std::array<int, 4> FourVec3I::Magnitude() const noexcept
	{
		std::array<int, 4> result = std::array<int, 4>();

		__m128i x1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_x.data()));
		__m128i y1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_y.data()));
		__m128i z1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_z.data()));

		__m128i x1x2 = _mm_mullo_epi32(x1, x1);
		__m128i y1y2 = _mm_mullo_epi32(y1, y1);
		__m128i z1z2 = _mm_mullo_epi32(z1, z1);

		__m128i resultPartial = _mm_add_epi32(x1x2, y1y2);
		resultPartial = _mm_add_epi32(resultPartial, z1z2);

		__m128i resultSqrt = _mm_sqrt_epi32(resultPartial);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(result.data()), resultSqrt);

		return result;
	}

	template <>
	[[nodiscard]] NOALIAS inline std::array<int, 4> FourVec3I::Normalized() const
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

		__m128i x1x2 = _mm_mullo_epi32(x1, x1);
		__m128i y1y2 = _mm_mullo_epi32(y1, y1);
		__m128i z1z2 = _mm_mullo_epi32(y1, y1);

		__m128i resultPartial = _mm_add_epi32(x1x2, y1y2);
		resultPartial = _mm_add_epi32(resultPartial, z1z2);

		__m128i resultSqrt = _mm_sqrt_epi32(resultPartial);

		__m128i resultSqrtReciprocal = _mm_rcp_epi32(resultSqrt);

		_mm_storeu_si128(reinterpret_cast<__m128i*>(result.data()), resultSqrtReciprocal);

		return result;
	}

#endif

#pragma endregion

#endif

}