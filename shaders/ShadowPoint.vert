#version 330 core

layout (location = 0) in vec4 inCoords;
layout( location = 1 ) in vec2 inUV;

uniform mat4 matObj;
uniform float flowers_anim;
uniform int obiectNumber;

out vec2 UV_frag;

void main()
{
	vec4 newPosition = inCoords;
	// grass
	if(obiectNumber == 8)
	{
		newPosition.x += (inCoords.y) * (cos(flowers_anim))/10;
		newPosition.z += (inCoords.y) * (sin(flowers_anim))/10;
	}
	// tree
	if(obiectNumber == 6)
	{
		newPosition.x += (inCoords.y) * (cos(flowers_anim))/30;
		newPosition.z += (inCoords.y) * (sin(flowers_anim))/30;
	}

    UV_frag = inUV;
    // gl_Position = inCoords;
    // gl_Position = matObj * newPosition;
    gl_Position = newPosition;
}
