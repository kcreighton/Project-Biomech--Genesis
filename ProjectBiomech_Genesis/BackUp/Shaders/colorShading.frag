#version 130
//The fragment shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
in vec4 fragmentColor;

//This is the 4 component float vector that gets outputted to the screen
//for each pixel.
out vec4 color;

uniform float time;

void main() {
    color = vec4(fragmentColor.r * (tan(fragmentPosition.x + time * 2) + 1.0) * 0.5,
				fragmentColor.g * (cos(fragmentPosition.y + time * 3) + 1.0) * 0.5,
				fragmentColor.b * (sin(fragmentPosition.x + time) + 1.0) * 0.5,
				fragmentColor.a);
}