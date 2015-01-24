#pragma once

namespace Runengine2D {

	///Calculates FPS and also limits FPS
	class FpsLimiter {
	public:
		FpsLimiter();

		// Initializes the FPS limiter. For now, this is
		// analogous to setMaxFPS
		void init(float maxFps);

		// Sets the desired max FPS
		void setMaxFps(float maxFps);

		void begin();

		// end() will return the current FPS as a float
		float end();
	private:
		// Calculates the current FPS
		void calculateFps();

		// Variables
		float _fps, _maxFps, _frameTime;
		unsigned int _startTicks;
	};

}