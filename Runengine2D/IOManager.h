#pragma once

#include <vector>

namespace Runengine2D
{

	class IOManager
	{
	public:
		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
	};

}