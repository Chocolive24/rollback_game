/**
 * @headerfile UniquePtr.h
 * This file defines the UniquePtr class which is a naive implementation of the
 * std::unique_ptr standard library class.
 *
 * @author Olivier Pachoud
 */

#pragma once

#include "Allocator.h"

#include <algorithm>

/*
* @brief UniquePtr is a naive implementation of the std::unique_ptr standard library class. 
*/
template<typename T>
class UniquePtr
{
private:
    T* _ptr = nullptr;   

public:
    constexpr explicit UniquePtr() noexcept = default;

    constexpr explicit UniquePtr(T* ptr) noexcept : _ptr(ptr) {};

    constexpr UniquePtr(const UniquePtr<T>& other) noexcept = delete;

    constexpr UniquePtr(UniquePtr<T>&& other) noexcept
    {
        std::swap(_ptr, other._ptr);
    }

    ~UniquePtr() noexcept 
    {
    #ifdef TRACY_ENABLE
            TracyFree(_ptr);
    #endif
        delete _ptr;
    }

    constexpr UniquePtr& operator=(const UniquePtr<T>& other) const noexcept = delete;

    constexpr UniquePtr& operator=(UniquePtr<T>&& other) noexcept
    {
        std::swap(_ptr, other._ptr);
        return *this;
    }

    [[nodiscard]] constexpr T& operator*() const noexcept { return *_ptr; }
    [[nodiscard]] constexpr T* operator->() const noexcept { return _ptr; }

    template<typename U>
    explicit constexpr operator UniquePtr<U>() noexcept
    {
        auto ptrToCast = _ptr;
        _ptr = nullptr;

        return UniquePtr<U>(ptrToCast);
    }

    /**
     * @brief Get is a method that gives the pointer inside the unique pointer object.
     * @return The pointer inside the unique pointer object.
     */
    [[nodiscard]] constexpr T* Get() const noexcept { return _ptr; }
};

/**
* @brief MakeUnique is a method that creates a unique pointer of the type of the value in parameter.
* @param value The value to point with the unique pointer.
* @return The unique pointer object that points the value.
*/
template<typename T, typename... Args>
[[nodiscard]] UniquePtr<T> MakeUnique(Allocator& allocator, Args&&... args) noexcept
{
    T* allocatedMemory = static_cast<T*>(allocator.Allocate(sizeof(T), alignof(T)));

    return UniquePtr<T>(new (allocatedMemory) T(std::forward<Args>(args)...));
}


/**
 * @brief MakeUnique is a method that creates a unique pointer of the type of the value in parameter.
 * @param value The value to point with the unique pointer.
 * @return The unique pointer object that points the value.
 */
template<typename T, typename U, typename... Args>
[[nodiscard]] UniquePtr<T> MakeUnique(Allocator& allocator, Args&&... args) noexcept
{
    U* allocatedMemory = static_cast<U*>(allocator.Allocate(sizeof(U), alignof(U)));

    return UniquePtr<T>(new (allocatedMemory) U(std::forward<Args>(args)...));
}