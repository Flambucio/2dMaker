#include "Deltatime.h"

using namespace D2Maker;
using Clock = std::chrono::high_resolution_clock;
template <typename T>
using Duration = std::chrono::duration<T>;


Clock::time_point DeltaTime::lastTime = Clock::now();
double DeltaTime::deltaTime = 0.0;

void DeltaTime::Update()
{
    auto now = Clock::now();
    deltaTime = Duration<double>(now - lastTime).count();
    lastTime = now;

    if (deltaTime < 0.001f) deltaTime = 0.001f;
    if (deltaTime > 0.033f) deltaTime = 0.033f;
}

double DeltaTime::Get()
{
    return deltaTime;
}