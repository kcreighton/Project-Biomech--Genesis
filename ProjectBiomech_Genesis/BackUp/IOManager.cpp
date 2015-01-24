#include "IOManager.h"

#include <fstream>

bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
{
	//open file to put binary version of contents
	std::ifstream file(filePath, std::ios::binary);
	if (file.fail())
	{
		perror(filePath.c_str());
		return false;
	}

	//seek to end of file
	file.seekg(0, std::ios::end);

	//get file size
	int fileSize = file.tellg();

	//seek back to beg of file
	file.seekg(0, std::ios::beg);

	//reduce fileSize by any headers
	fileSize =- file.tellg();

	//read into buffer
	buffer.resize(fileSize);
	file.read((char*)&(buffer[0]), fileSize);

	//close file
	file.close();
}