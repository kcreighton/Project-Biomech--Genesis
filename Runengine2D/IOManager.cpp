#include "IOManager.h"

#include <fstream>

namespace Runengine2D
{

	//
	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
	{
		//open file
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail())
		{
			perror(filePath.c_str());
			return false;
		}

		//seek to the end
		file.seekg(0, std::ios::end);

		//Get the file size
		int fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		//Reduce the fileSize variable by any header bytes that might be present
		fileSize -= file.tellg();
		buffer.resize(fileSize);

		//load file into buffer
		file.read((char *)&(buffer[0]), fileSize);

		//close file
		file.close();

		return true;
	}

}