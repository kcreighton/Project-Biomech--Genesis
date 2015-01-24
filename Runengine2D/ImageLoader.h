#pragma once
#include "GLTexture.h"

#include <string>
#include <vector>

namespace Runengine2D
{

	//Loads images into GLTextures and or converts them into physical data
	class ImageLoader
	{
	public:
		static void loadPNG(std::string filePath, GLTexture*);
		static void loadPNG(std::string filePath, std::vector<bool>*);
	};

}