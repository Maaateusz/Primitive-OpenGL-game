#version 150 core

in vec4 Coords_frag;
out vec4 outColor;

uniform samplerCube tex_skybox;

void main()
{
	outColor = texture(tex_skybox, Coords_frag.xyz);
}
