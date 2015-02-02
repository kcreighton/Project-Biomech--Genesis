#version 130
//The vertex shader operates on each vertex

//input data from the vertexBuffer. Each vertex is 2 floats
in vec2 vertexPosition;
in vec4 vertexColor;

out vec4 fragmentColor;

void main() {
    //Set the x,y position on the screen
    gl_Position.xy = vertexPosition;
    //the z position is zero since we are in 2D
    gl_Position.z = 0.0;
    
    //Indicate that the coordinates are normalized
    gl_Position.w = 1.0;
	
	fragmentColor = vertexColor;
}