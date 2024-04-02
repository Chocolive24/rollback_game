/**
 * @headerfile Timer.h
 * This file defines the Timer class which is a naive timer using the <chrono> standard library.
 *
 * @author Olivier
 */

#pragma once

#include <chrono>

/*
* @brief Timer is a simple timer using the <chrono> library which calculates the time 
* elapsing between each frame and since the timer was created.
*/
class Timer
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
    std::chrono::duration<float> _deltaTime = std::chrono::duration<float>(0);

public:
    /**
    * @brief Init is a method that tells the timer to start counting down.
    */
    void Init() noexcept;

    /**
    * @brief Tick is a method that updates the timer to measure the time elapsed between frames.
    */
    void Tick() noexcept;

    /**
    * @brief DeltaTime is a method that calculates the time elapsed between two consecutive frames.
    * @return The time elapsed between two consecutive frames in seconds.
    */
    [[nodiscard]] float DeltaTime() const noexcept;

    /**
    * @brief TotalTime is a method that calculates the time since the Init() function was called.
    * @return The time since the Init() function was called in seconds.
    */
    [[nodiscard]] float TotalTime() const noexcept;
};