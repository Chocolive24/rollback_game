#pragma once

/**
 * @author Constantin
 */

#ifdef _MSC_VER
#define NOALIAS __declspec(noalias)
#define FORCE_INLINE __forceinline
#else
#define NOALIAS __attribute__((const))
#define FORCE_INLINE __attribute__((always_inline))
#endif