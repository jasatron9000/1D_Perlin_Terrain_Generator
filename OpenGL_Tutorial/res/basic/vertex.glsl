/*
+=======================================+
|										|
|			VERTEX SHADER				|
|										|
+=======================================+
*/
#version 330 core

// Vertex Attributes
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColour;

// Ins and Outs
out vec3 colour;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	colour = aColour;
}

