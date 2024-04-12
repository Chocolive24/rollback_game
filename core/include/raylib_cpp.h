#pragma once

/**
 * \brief raylib is a namespace that wraps the raylib headers include because
 * some functions have a name conflict with functions from the windows headers
 * included in the photon library.
 * It also recreates the raylib color macros to constexpr variables.
 */
namespace raylib {

#include <raylib.h>

constexpr Color kRed{230, 41, 55, 255};
constexpr Color kBlue{0, 121, 241, 255};
constexpr Color kWhite{255, 255, 255, 255};
constexpr Color kBlack{0, 0, 0, 255};

}  // namespace raylib