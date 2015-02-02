#include "ImageLoader.h"
#include "PicoPNG.h"
#include "IOManager.h"
#include "Errors.h"

GLTexture ImageLoader::loadPNG(std::string filePath)
{
	//create texture object
	GLTexture texture = {};

	std::vector<unsigned char> inBuffer, outTexture;
	unsigned long outWidth, outHeight;

	//load PNG to buffer
	if (IOManager::readFileToBuffer(filePath, inBuffer) == false)
	{
		fatalError("failed to load PNG file to buffer");
	}

	//decode
	int errorCode = decodePNG(outTexture, outWidth, outHeight, &(inBuffer[0]), inBuffer.size());
	if (errorCode != 0)
	{
		fatalError("decodePNG failed with error: " + std::to_string(errorCode));
	}

	//create texture
	glGenTextures(1, &(texture.id));

	//bind texture
	glBindTexture(GL_TEXTURE_2D, texture.id);

	//put at image on it
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, outWidth, outHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(outTexture));

	//
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);


	//modify return texture
	texture.width = outWidth;
	texture.height = outHeight;

	//return copy of texture data
	return texture;
}