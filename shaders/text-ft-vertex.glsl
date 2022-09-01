// https://learnopengl.com/In-Practice/Text-Rendering
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;
uniform float zpos;
uniform mat4 matProjView;
uniform mat4 matObj;

void main()
{
    // gl_Position = matProjView * vec4(vertex.xy, zpos, 1.0);
    // gl_Position = projection * matObj * vec4(vertex.xy, zpos, 1.0);
    gl_Position = projection * vec4(vertex.xy, zpos, 1.0);
    TexCoords = vertex.zw;
}