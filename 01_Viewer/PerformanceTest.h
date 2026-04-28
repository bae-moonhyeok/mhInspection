#pragma once
// 성능 측정을 위한 헬퍼 클래스
class PerformanceTest
{
	LARGE_INTEGER m_start;
	LARGE_INTEGER m_freq;

public:
	PerformanceTest() {
		QueryPerformanceFrequency(&m_freq);
		QueryPerformanceCounter(&m_start);
	}

	double GetElapsedSeconds() {
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);
		return static_cast<double>(end.QuadPart - m_start.QuadPart) / m_freq.QuadPart;
	}

	double GetElapsedMilliseconds() {
		return GetElapsedSeconds() * 1000.0;
	}
};
