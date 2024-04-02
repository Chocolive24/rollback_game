#include "Timer.h"

void Timer::Init() noexcept
{
    _startTime = std::chrono::high_resolution_clock::now();
}

void Timer::Tick() noexcept
{
    auto currentTime = std::chrono::high_resolution_clock::now();

    _deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - _startTime);

    _startTime = currentTime;
}

[[nodiscard]] float Timer::DeltaTime() const noexcept
{
    return _deltaTime.count();
}

[[nodiscard]] float Timer::TotalTime() const noexcept
{
    return std::chrono::duration_cast<std::chrono::duration<float>>(
            std::chrono::high_resolution_clock::now() - _startTime).count();
}