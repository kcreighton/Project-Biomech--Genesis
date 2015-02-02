#include "Level.h"

#include <fstream>
#include <iostream>

#include <Runengine2D/Errors.h>

Level::Level(const std::string& fileName)
{
	//open file
	std::ifstream file;
	file.open(fileName);

	//error check
	if (file.fail())
	{
		Runengine2D::fatalError("failed to load " + fileName);
	}

	//get number of humans from first line
	std::string temp;
	file >> temp >> _numHumans;

	//read file
	while (std::getline(file, temp))
	{
		_levelData.push_back(temp);

		/* temp print test
			std::cout << temp << std::endl;
		// temp print test */
	}

	/* _levelData print test
	for (int i = 0; i < _levelData.size(); i++)
	{
		std::cout << _levelData[i] << std::endl;
	}
	// _levelData print test */
}


Level::~Level()
{
}
