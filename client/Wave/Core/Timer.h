#pragma once

namespace core 
{
	class Timer
	{
	public:
		static void Tick();

		static double DeltaTime();

		static float DeltaTimeF();

	private:
		static uint64_t m_Now;
		static uint64_t m_Last;
		static double m_DeltaTime;
	};
}