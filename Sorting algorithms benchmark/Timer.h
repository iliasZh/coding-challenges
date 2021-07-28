#pragma once

#include <chrono>

class Timer
{
public:
	Timer()
	{
		mark = std::chrono::steady_clock::now();
	}
	float Mark()
	{
		using namespace std::chrono;
		auto now = steady_clock::now();
		const duration<float> time = now - mark;
		mark = now;
		return time.count();
	}
	float Peek()
	{
		using namespace std::chrono;
		return duration<float>(steady_clock::now() - mark).count();
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> mark;
};