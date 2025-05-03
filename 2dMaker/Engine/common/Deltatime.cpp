#include "Deltatime.h"

using namespace D2Maker;

std::chrono::high_resolution_clock::time_point DeltaTime::lastTime = std::chrono::high_resolution_clock::now();
double DeltaTime::deltaTime = 0.0;

void DeltaTime::Update()
{
    auto now = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<double>(now - lastTime).count();
    lastTime = now;

    if (deltaTime < 0.001f) deltaTime = 0.001f;
    if (deltaTime > 0.033f) deltaTime = 0.033f;
}

double DeltaTime::Get()
{
    return deltaTime;
}