/**
 * @headerfile Allocator.h
 * This file defines a collection of custom allocator that enables to profile
 * the data allocations.
 *
 * @author Olivier
 */

#pragma once

#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#endif // TRACY_ENABLE

#include <cstddef>
#include <memory>
#include <vector>

    /**
     * @brief Allocator is an abstract class the defines the fundamental elements shared between 
     * all different custom allocators. 
     * It also enabled to see and analyse the number of allocations made and the amount 
     * of memory used.
     */
    class Allocator
    {
    protected:
        void* _rootPtr = nullptr;
        std::size_t _size = 0;
        std::size_t _usedMemory = 0;
        std::size_t _allocationCount = 0;

    public:
        Allocator() = default;
        Allocator(void* rootPtr, std::size_t size) noexcept
        {
            _rootPtr = rootPtr;
            _size = size;
            _usedMemory = 0;
            _allocationCount = 0;
        }

        virtual ~Allocator() noexcept
        {
            _rootPtr = nullptr;
            _size = 0;
            _usedMemory = 0;
            _allocationCount = 0;
        }

        /**
         * @brief Allocate is a method that allocates a given amount of memory.
         * @param allocationSize The size of the allocation to do.
         * @param alignment The alignment in memory of the allocation.
         * @return A pointer pointing to the memory (aka a void*).
         */
        virtual void* Allocate(std::size_t allocationSize, std::size_t alignment) = 0;

        template<typename T>
        T* Allocate(std::size_t allocationSize)
        {
            return static_cast<T*>(Allocate(allocationSize, sizeof(T)));
        }

        /**
         * @brief Deallocate is a method that deallocates a block of memory given in parameter.
         * @param ptr The pointer to the memory block to deallocates.
         */
        virtual void Deallocate(void* ptr) = 0;

        /**
         * @brief RootPtr is a method that gives to root pointer of the allocator (aka the start of the memory block of
         * the allocator).
         * @return The root pointer of the allocator.
         */
        [[nodiscard]] void* RootPtr() const noexcept { return _rootPtr; }

        /**
         * @brief Size is a method that gives to size of the block of memory of the allocator.
         * @return The size of the block of memory of the allocator.
         */
        [[nodiscard]] std::size_t Size() const noexcept { return _size; }

        /**
         * @brief UseMemory is a method that gives the amount of memory used with the allocator.
         * @return The amount of memory used with the allocator.
         */
        [[nodiscard]] std::size_t UsedMemory() const noexcept { return _usedMemory; }

        /**
         * @brief AllocationCount is a method that gives the count of allocation made with the allocator.
         * @return The count of allocation made with the allocator..
         */
        [[nodiscard]] std::size_t AllocationCount() const noexcept { return _allocationCount; }
    };

    /*
    * @brief HeapAllocator is a custom allocator that simply trace the allocations made with 
    * std::malloc and std::free.
    */
    class HeapAllocator final : public Allocator
    {
    public:
        /**
         * @brief Allocate is a method that allocates a given amount of memory.
         * @param allocationSize The size of the allocation to do.
         * @param alignment The alignment in memory of the allocation.
         * @return A pointer pointing to the memory (aka a void*).
         */
        void* Allocate(std::size_t allocationSize, std::size_t alignment) override;

        /**
         * @brief Deallocate is a method that deallocates a block of memory given in parameter.
         * @param ptr The pointer to the memory block to deallocates.
         */
        void Deallocate(void* ptr) override;
    };

    /**
     * @brief StandardAllocator is an implementation of the allocator of the STL but used as a proxy
     * custom allocator in order to be able to trace allocations.
     */
    template<typename T>
    class StandardAllocator
    {
    protected:
        Allocator& _allocator;

    public:
        typedef T value_type;

        StandardAllocator(Allocator& allocator);

        StandardAllocator(StandardAllocator&& other) noexcept = default;
        StandardAllocator& operator=(StandardAllocator&& other) noexcept = default;
        StandardAllocator(const StandardAllocator& other) noexcept = default;
        StandardAllocator& operator=(const StandardAllocator& other) noexcept = default;
        ~StandardAllocator() noexcept = default;
             
        template <class U>
        StandardAllocator(const StandardAllocator<U>& allocator) noexcept : 
            _allocator(allocator.GetAllocator()) {}

        /**
         * @brief allocate is a method that calls the referenced allocator's Allocate method
         * that allocates a given amount of memory.
         * @param n The size of the allocation to do.
         * @return A pointer pointing to the memory (aka a T*).
         */
        T* allocate(std::size_t n);

        /**
         * @brief deallocate is a method that calls the referenced allocator's Deallocate method
         * that deallocates a block of memory given in parameter.
         * @param ptr The pointer to the memory block to deallocates.
         * * @param n The size of the pointer to deallocate.
         */
        void deallocate(T* ptr, std::size_t n);

        [[nodiscard]] Allocator& GetAllocator() const { return _allocator; }
    };

    template <class T, class U>
    constexpr bool operator== (const StandardAllocator<T>&, const StandardAllocator<U>&) noexcept
    {
        return true;
    }

    template <class T, class U>
    constexpr bool operator!= (const StandardAllocator<T>&, const StandardAllocator<U>&) noexcept
    {
        return false;
    }

    template <typename T>
    StandardAllocator<T>::StandardAllocator(Allocator& allocator) : _allocator(allocator)
    {
    }

    template <typename T>
    T* StandardAllocator<T>::allocate(std::size_t n)
    {
        return static_cast<T*>(_allocator.Allocate(n * sizeof(T), alignof(T)));
    }

    template <typename T>
    void StandardAllocator<T>::deallocate(T* ptr, [[maybe_unused]] std::size_t n)
    {
        _allocator.Deallocate(ptr);
    }

    template<typename T>
    using AllocVector = std::vector<T, StandardAllocator<T>>;