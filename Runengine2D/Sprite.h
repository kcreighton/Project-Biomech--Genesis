#pragma once
#include <GL/glew.h>
#include "GLTexture.h"

#include <string>

enum SpriteCatagory { NONE };

namespace Runengine2D
{
	//A 2D quad that can be rendered to the screen
	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string texturePath);

		void draw();

	private:
		float _x, _y;
		float _width, _height;
		GLuint _vertexBufferID;
		GLTexture _texture;
	};

}