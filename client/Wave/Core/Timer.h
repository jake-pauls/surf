#pragma once

namespace core 
{
	class Timer
	{
	public:
		static void Tick();

		static uint64_t DeltaTime();

	private:
		static uint64_t m_LastTick;
		static uint64_t m_DeltaTime;
	};
}