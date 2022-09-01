#version 330 core

layout( location = 0 ) in vec4 inPosition;

uniform mat4 matProjView;
out vec4 Coords_frag;

void main()
{
	gl_Position = matProjView * inPosition;
	Coords_frag = inPosition;
}
