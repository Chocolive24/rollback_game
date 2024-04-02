/**
 * @headerfile Metrics.h
 * This file defines a collection of metrics.
 *
 * @author Olivier
 */

#pragma once

#include "Vec2.h"

namespace Metrics
{
    /**
     * @brief The number of meters for 1 pixel.
     */
    static constexpr float PixelsToMetersRatio = 0.01f;
    /**
     * @brief The number of pixels for 1 meter.
     */
    static constexpr float MetersToPixelsRatio = 100.f;

    /**
     * @brief PixelsToMeters is a method that converts a value in pixels to its equivalent value in meters
     * based on the predefined pixels/meters ratio.
     * @param pixels The value in pixels that you want to convert to meters.
     * @return The converted value in meters of the input 'pixels'.
     */
    template<typename T>
    [[nodiscard]] constexpr T PixelsToMeters(const T pixels) noexcept
    {
        return pixels * PixelsToMetersRatio;
    }

    /**
     * @brief PixelsToMeters is a method that converts a position in pixels to its equivalent value in meters
     * based on the predefined pixels/meters ratio.
     * @param pixelPos The position in pixels that you want to convert to meters.
     * @return The converted position in meters of the input 'pixels'.
     */
    template <typename T>
    [[nodiscard]] constexpr Math::Vec2<T> PixelsToMeters(const Math::Vec2<T> pixelPos) noexcept
    {
        return Math::Vec2<T>(pixelPos.X * PixelsToMetersRatio, -pixelPos.Y * PixelsToMetersRatio);
    }

    /**
     * @brief MetersToPixels is a method that converts a value in meters to its equivalent value in pixels
     * based on the predefined pixels/meters ratio.
     * @param meters The value in pixels that you want to convert to meters.
     * @return The converted value in meters of the input 'pixels'.
     */
    template<typename T>
    [[nodiscard]] constexpr T MetersToPixels(const T meters) noexcept
    {
        return meters * MetersToPixelsRatio;
    }

    /**
     * @brief MetersToPixels is a method that converts a position in meters to its equivalent position in pixels
     * based on the predefined pixels/meters ratio.
     * @param meterPos The position in pixels that you want to convert to meters.
     * @return The converted position in meters of the input 'pixels'.
     */
    template <typename T>
    [[nodiscard]] constexpr Math::Vec2<T> MetersToPixels(const Math::Vec2<T> meterPos) noexcept
    {
        return Math::Vec2<T>(meterPos.X * MetersToPixelsRatio, -meterPos.Y * MetersToPixelsRatio);
    }
}