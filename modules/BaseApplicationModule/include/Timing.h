#pragma once

class Timing
{
public:
	static Timing& Instance() {
		static Timing instance;
		return instance;
	}

	float totalTime = 0.0f;
	double CurrentFrame;
	double LastFrame;
	float  DeltaTime;

protected:
	Timing() = default;
};