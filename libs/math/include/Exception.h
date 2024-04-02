#pragma once

/**
 * @author Alexis
 */

#include <exception>

class DivisionByZeroException : public std::exception
{
public:
	[[nodiscard]] const char* what() const noexcept override
	{
		return "Division by zero";
	}
};

class OutOfRangeException : public std::exception
{
public:
	[[nodiscard]] const char* what() const noexcept override
	{
		return "Out of range";
	}
};

class NaNException : public std::exception
{
public:
	[[nodiscard]] const char* what() const noexcept override
	{
		return "NaN";
	}
};