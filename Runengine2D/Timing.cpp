#include "Timing.h"

#include <SDL/SDL.h>

namespace Runengine2D {

	FpsLimiter::FpsLimiter() {
	}
	void FpsLimiter::init(float maxFPS) {
		setMaxFps(maxFPS);
	}

	void FpsLimiter::setMaxFps(float maxFPS) {
		_maxFps = maxFPS;
	}

	void FpsLimiter::begin() {
		_startTicks = SDL_GetTicks();
	}

	float FpsLimiter::end() {
		calculateFps();

		float frameTicks = SDL_GetTicks() - _startTicks;
		//Limit the FPS to the max FPS
		if (1000.0f / _maxFps > frameTicks) {
			SDL_Delay((Uint32)(1000.0f / _maxFps - frameTicks));
		}

		return _fps;
	}

	void FpsLimiter::calculateFps() {
		//The number of frames to average
		static const int NUM_SAMPLES = 10;
		//Stores all the frametimes for each frame that we will average
		static float frameTimes[NUM_SAMPLES];
		//The current frame we are on
		static int currentFrame = 0;
		//the ticks of the previous frame
		static float prevTicks = SDL_GetTicks();

		//Ticks for the current frame
		float currentTicks = SDL_GetTicks();

		//Calculate the number of ticks (ms) for this frame
		_frameTime = currentTicks - prevTicks;
		frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

		//current ticks is now previous ticks
		prevTicks = currentTicks;

		//The number of frames to average
		int count;

		currentFrame++;
		if (currentFrame < NUM_SAMPLES) {
			count = currentFrame;
		}
		else {
			count = NUM_SAMPLES;
		}

		//Average all the frame times
		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++) {
			frameTimeAverage += frameTimes[i];
		}
		frameTimeAverage /= count;

		//Calculate FPS
		if (frameTimeAverage > 0) {
			_fps = 1000.0f / frameTimeAverage;
		}
		else {
			_fps = 60.0f;
		}
	}

}