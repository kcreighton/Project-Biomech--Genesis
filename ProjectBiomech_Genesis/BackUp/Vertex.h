#pragma once

#include <GL/glew.h>

//8 bytes of 2 floats of 4 bytes
struct Position
{
	float x, y;
};

//4 bytes 0f 4 chars of 1 byte
struct Color
{
	char r, g, b, a;
};

struct Vertex
{
	//8 bytes of 2 floats of 4 bytes
	Position position;

	//4 bytes 0f 4 chars of 1 byte
	Color color;
	
	//12 bytes total
	//12 % 4 = 0
	//keep Vertex struct at multiples of 4
};