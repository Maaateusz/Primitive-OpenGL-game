#version 330

// --- IN ---
layout( location = 0 ) in vec4 inCoords;
layout( location = 1 ) in vec2 inUV;
layout( location = 2 ) in vec3 inNormals;

// macierz modelu wspolna dla calej instancji
// layout ( location = 3 ) in mat4 matModelInst;

// --- UNIFORM ---
uniform int obiectNumber;
uniform mat4 matProjView;
uniform mat4 matObj;
uniform mat3 matNormal;
uniform int light_mode;
// uniform vec3 eyePosition;
uniform float tmp;
uniform float flowers_anim;

uniform mat4 lightProj;
uniform mat4 lightView;

// --- OUT ---
out vec4 Coords_frag;
out vec2 UV_frag;
out vec3 Normals_frag;

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

	// ---------- OUT ----------
	// Coords_frag = matObj * inCoords;
	Coords_frag = matObj * newPosition;

	UV_frag = inUV;

	Normals_frag = matNormal * inNormals;

	// gl_Position = matProjView * matObj * inCoords;
	gl_Position = matProjView * matObj * newPosition;
}
