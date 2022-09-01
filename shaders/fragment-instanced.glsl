#version 150

// --- IN ---
// in vec4 Coords_frag;
in vec2 UV_frag;
// in vec3 Normals_frag;

// --- UNIFORM ---
uniform sampler2D Texture;
uniform float texture_scale;

// --- OUT ---
out vec4 outColor;


void main()
{
	vec4 fragColor;
	fragColor = texture(Texture, UV_frag * texture_scale);
	if(fragColor.a < 0.5) 
		discard;

	outColor = fragColor;
}
