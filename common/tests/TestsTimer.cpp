#include "Timer.h"

#include "gtest/gtest.h"

TEST(Timer, DeltaTime)
{
    Timer timer;
    timer.Init();

    std::chrono::time_point<std::chrono::steady_clock> endOfFrame, startOfFrame;
    std::chrono::duration<float> frameTime{0.f};

    startOfFrame = std::chrono::high_resolution_clock::now();

    float epsilon = 0.00001f;

    int loopCounter = 0;

    while(loopCounter < 100000)
    {
        timer.Tick();

        endOfFrame = std::chrono::high_resolution_clock::now();

        frameTime = std::chrono::duration_cast<std::chrono::duration<float>>(
                endOfFrame - startOfFrame);

        startOfFrame = endOfFrame;

        loopCounter++;
    }

    EXPECT_NEAR(timer.DeltaTime(), frameTime.count(), epsilon);
}

TEST(Timer, TotalTime)
{
    Timer timer;
    timer.Init();

    auto startTime = std::chrono::high_resolution_clock::now();
    float totalTime;

    float epsilon = 0.00001f;

    // Sleep for a while to simulate the passage of time
    std::this_thread::sleep_for(std::chrono::seconds(2));

    totalTime = std::chrono::duration_cast<std::chrono::duration<float>>(
            std::chrono::high_resolution_clock::now() - startTime).count();

    EXPECT_NEAR(timer.TotalTime(), totalTime, epsilon);
}