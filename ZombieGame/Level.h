#pragma once

#include <string>
#include <vector>

class Level
{
public:
	//load level
	Level(const std::string& fileName);
	~Level();
private:
	std::vector<std::string> _levelData;
	int _numHumans;
	int _numZombies;
};

