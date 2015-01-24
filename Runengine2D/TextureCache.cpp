#include "TextureCache.h"
#include "ImageLoader.h"

#include <iostream>

namespace Runengine2D
{

	TextureCache::TextureCache() :
		_textureMap()
	{
	}


	TextureCache::~TextureCache()
	{
	}

	GLTexture TextureCache::getTexture(std::string texturePath)
	{
		//shearch map
		auto mapLocation = _textureMap.find(texturePath);

		//did it find it?
		if (mapLocation == _textureMap.end())
		{
			//load texture
			GLTexture newTexture;
			ImageLoader::loadPNG(texturePath, &newTexture); // assigns a PNG image to texture

			//insert texture
			_textureMap.insert(make_pair(texturePath, newTexture));

			std::cout << "loaded texture from TextureCache\n";
			return newTexture;
		}

		std::cout << "loaded cached texture from TextureCache\n";
		return mapLocation->second; // second element is string
	}

}