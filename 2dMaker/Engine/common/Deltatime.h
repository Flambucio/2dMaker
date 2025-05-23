#pragma once
#include <chrono>

namespace D2Maker
{
    static class DeltaTime
    {
    private:
        static std::chrono::high_resolution_clock::time_point lastTime;
        static double deltaTime;
    public:
        static void Update();
        static double Get();
    };
}
