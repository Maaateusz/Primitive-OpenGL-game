#version 330

// --- IN ---
layout( location = 0 ) in vec4 Coords;
layout( location = 1 ) in vec2 UV;
layout( location = 3 ) in mat4 matModelInst;

// --- UNIFORM ---
uniform mat4 matProjView;
uniform float anim;

// --- OUT ---
out vec2 UV_frag;

void main()
{
	// ---------- OUT ----------

	UV_frag = UV;

	vec4 newPosition = Coords;
	newPosition.x -= cos(anim);
	newPosition.y -= sin(anim);

	gl_Position = matProjView * matModelInst * newPosition;
}
