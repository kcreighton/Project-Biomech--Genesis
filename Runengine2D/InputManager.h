#pragma once

#include <unordered_map>
#include <GLM/glm.hpp>

namespace Runengine2D
{

	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void keyDown(unsigned int keyID);
		void keyUp(unsigned int keyID);
		bool isKeyPressed(unsigned int keyID);

		//setters
		void setMouseCoords(float x, float y);

		//getters
		glm::vec2 getMouseCoords() const { return _mouseCoords; }

	private:
		std::unordered_map<unsigned int, bool> _keyMap;
		glm::vec2 _mouseCoords;
	};

}