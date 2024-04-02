/**
 * @headerfile SharedPtr.h
 * This file defines the SharedPtr class which is a naive implementation of the
 * std::shared_ptr standard library class.
 *
 * @author Olivier
 */

#pragma once

#include <algorithm>

/*
*@brief SharedPtr is a naive implementation of the std::shared_ptr standard library class.
*/
template<typename T>
class SharedPtr
{
private:
    T* _ptr = nullptr;
    int* _count = nullptr;

public:
    constexpr explicit SharedPtr() noexcept = default;
    constexpr explicit SharedPtr(T* ptr) noexcept : _ptr(ptr), _count(new int(1)) {};

    constexpr SharedPtr(const SharedPtr<T>& other) noexcept : _ptr(other._ptr), _count(other._count)
    {
        (*_count)++;
    }

    constexpr SharedPtr(SharedPtr<T>&& other) noexcept
    {
        std::swap(_ptr, other._ptr);
        std::swap(_count, other._count);

        other._ptr = nullptr;
        other._count = nullptr;
    }

    ~SharedPtr() noexcept
    {
        (*_count)--;

        if ((*_count) == 0)
        {
            delete _ptr;
            delete _count;
        }
    }

    constexpr SharedPtr<T>& operator=(const SharedPtr<T>& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        _ptr = other._ptr;
        _count = other._count;

        (*_count)++;

        return *this;
    }

    constexpr SharedPtr<T>& operator=(SharedPtr<T>&& other) noexcept
    {
        std::swap(_ptr, other._ptr);
        std::swap(_count, other._count);

        other._ptr = nullptr;
        other._count = nullptr;

        return *this;
    }

    [[nodiscard]] constexpr T& operator*() const noexcept { return *_ptr; }
    [[nodiscard]] constexpr T* operator->() const noexcept { return _ptr; }

    template<typename U>
    explicit constexpr operator SharedPtr<U>() const noexcept
    {
        auto ptrToCast = _ptr;
        _ptr = nullptr;

        return SharedPtr<U>(ptrToCast);
    }

    /**
     * @brief Get is a method that gives the pointer inside the shared pointer object.
     * @return The pointer inside the shared pointer object.
     */
    [[nodiscard]] constexpr T* Get() const noexcept { return _ptr; }

    /**
     * @brief SharedCount is a method that gives the count of shared pointer pointing to the pointer inside.
     * @return The count of shared pointer pointing to the pointer inside.
     */
    [[nodiscard]] constexpr int SharedCount() const noexcept { return *_count; }
};

/**
* @brief MakeShared is a method that creates a shared pointer of the type of the value in parameter.
* @param value The value to point with the shared pointer.
* @return The shared pointer object that points the value.
*/
template<typename T>
[[nodiscard]] static constexpr SharedPtr<T> MakeShared(const T& value) noexcept
{
    return SharedPtr<T>(new T(value));
}

/**
* @brief MakeShared is a method that creates a shared pointer of the type of the value in parameter.
* @param value The value to point with the shared pointer.
* @return The shared pointer object that points the value.
*/
template<typename T, typename U>
[[nodiscard]] static constexpr SharedPtr<T> MakeShared(const U& value) noexcept
{
    return SharedPtr<T>(new U(value));
}