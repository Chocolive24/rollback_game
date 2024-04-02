#pragma once

/**
 * @author Alexis
 */

#if defined(__x86_64__) || defined(_M_X64)
#include <immintrin.h>
#if !defined(__SSE__)
#define __SSE__
#endif
#endif

#if defined(__aarch64__)
#include <arm_neon.h>
#endif

#if defined(__GNUC__) || defined(__clang__)
typedef float v4sf __attribute__ ((vector_size (16)));
#endif