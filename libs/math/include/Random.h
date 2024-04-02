#pragma once

/**
* @headerfile Random number generator functions and classes
* @author Alexis
*/

#include <random>

/**
 * @brief A namespace for random number generator functions and classes
 */
namespace Math::Random
{
    [[nodiscard]] inline float Range(float min, float max) noexcept
    {
        if (min > max)
        {
            float temp = min;
            min = max;
            max = temp;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(min, max);

        return dis(gen);
    }

    [[nodiscard]] inline int Range(int min, int max) noexcept
    {
        if (min > max)
        {
            int temp = min;
            min = max;
            max = temp;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(min, max);

        return dis(gen);
    }
}