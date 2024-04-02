#include "Allocator.h"

#include <cstdlib>

void* HeapAllocator::Allocate(std::size_t allocationSize, std::size_t alignment)
{
    if (allocationSize == 0)
    {
        return nullptr;
    }

    // Calculate the correct size based on allocationSize alone
    const std::size_t size = allocationSize;

    auto* ptr = malloc(size);

#ifdef TRACY_ENABLE
        TracyAlloc(ptr, size);
#endif

    return ptr;
}

void HeapAllocator::Deallocate(void* ptr)
{
#ifdef TRACY_ENABLE
        TracyFree(ptr);
#endif

    free(ptr);
}