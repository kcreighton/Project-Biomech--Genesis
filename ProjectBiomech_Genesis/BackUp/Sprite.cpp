#include "Sprite.h"
#include "Vertex.h"

#include <cstddef>

Sprite::Sprite() : _vertexBufferID(0)
{
	
}


Sprite::~Sprite()
{
    //Always remember to delete your buffers when
    if (_vertexBufferID != 0) {
        glDeleteBuffers(1, &_vertexBufferID);
    }
}

//Initializes the sprite verex buffer. x, y, width, and height are in the normalized device coordinate space. so, [-1, 1]
void Sprite::init(float x, float y, float width, float height)
{
    //Set up our private vars
    _x = x;
    _y = y;
    _width = width;
    _height = height;

    //Generate the buffer if it hasn't already been generated
    if (_vertexBufferID == 0)
	{
        glGenBuffers(1, &_vertexBufferID);
    }

    //This array will hold our vertex data.
    //We need 6 vertices, and each vertex has 2
    //floats for X and Y
	Vertex vertexData[6];

    //First Triangle

	//TopRight
    vertexData[0].position.x = x + width;
	vertexData[0].position.y = y + height;
	vertexData[0].color.r = 0;
	vertexData[0].color.g = 255;
	vertexData[0].color.b = 0;
	vertexData[0].color.a = 255;

	//TopLeft
	vertexData[1].position.x = x;
	vertexData[1].position.y = y + height;
	vertexData[1].color.r = 255;
	vertexData[1].color.g = 0;
	vertexData[1].color.b = 0;
	vertexData[1].color.a = 255;

	//BottomLeft
	vertexData[2].position.x = x;
	vertexData[2].position.y = y;
	vertexData[2].color.r = 255;
	vertexData[2].color.g = 0;
	vertexData[2].color.b = 255;
	vertexData[2].color.a = 255;

    //Second Triangle

	//BottomLeft
	vertexData[3].position.x = x;
	vertexData[3].position.y = y;
	vertexData[3].color.r = 255;
	vertexData[3].color.g = 0;
	vertexData[3].color.b = 255;
	vertexData[3].color.a = 255;

	//BottomRight
	vertexData[4].position.x = x + width;
	vertexData[4].position.y = y;
	vertexData[4].color.r = 0;
	vertexData[4].color.g = 255;
	vertexData[4].color.b = 255;
	vertexData[4].color.a = 255;

	//TopRight
	vertexData[5].position.x = x + width;
	vertexData[5].position.y = y + height;
	vertexData[5].color.r = 0;
	vertexData[5].color.g = 255;
	vertexData[5].color.b = 0;
	vertexData[5].color.a = 255;

    //Tell opengl to bind our vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);

    //Upload the data to the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    //Unbind the vrtex buffer (optional)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Draws the sprite to the screen
void Sprite::draw()
{

    //bind the vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);

    //Tell opengl that we want to use the first attribute array.
	//We only need one array right now since we are only using position.
    glEnableVertexAttribArray(0);

    //Point opengl to the data in our vertex buffer
	//This it position attribute pointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	//This it color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    //Draw the 6 vertices to the screen
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //Disable the vertex attrib array. This is not optional.
    glDisableVertexAttribArray(0);

    //Unbind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}