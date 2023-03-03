#include "Timer.h"

#include <SDL3/SDL.h>

namespace core
{
    uint64_t Timer::m_LastTick = 0;
    uint64_t Timer::m_DeltaTime = 0;
}

void core::Timer::Tick()
{
    uint64_t ticks = SDL_GetTicks();
    m_DeltaTime = ticks - m_LastTick / (uint64_t) 1000;
    m_LastTick = ticks;
}

uint64_t core::Timer::DeltaTime()
{
    return m_DeltaTime;
}