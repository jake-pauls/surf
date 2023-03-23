#include "Timer.h"

#include <SDL3/SDL.h>

namespace core
{
    uint64_t Timer::m_Now = SDL_GetPerformanceCounter();
    uint64_t Timer::m_Last = 0;
    double Timer::m_DeltaTime = 0;
}

void core::Timer::Tick()
{
    m_Last = m_Now;
    m_Now = SDL_GetPerformanceCounter();
    m_DeltaTime = (double)((m_Now - m_Last) * 1000 / (double)SDL_GetPerformanceFrequency());
}

double core::Timer::DeltaTime()
{
    return m_DeltaTime;
}

float core::Timer::DeltaTimeF()
{
    return static_cast<float>(m_DeltaTime);
}