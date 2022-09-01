// Reference: https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows
#version 330 core

in vec4 FragPos;
in vec2 UV_frag;

uniform vec3  LightPosition;
uniform float pointLightDistanceFar;
uniform sampler2D Texture;
uniform float texture_scale;

void main()
{

    // get distance between fragment and light source
    float lightDistance = length(FragPos.xyz - LightPosition);

    // map to [0;1] range by dividing by FarPlane
    lightDistance = lightDistance / pointLightDistanceFar;

    // vec4 fragColor = texture(Texture, UV_frag);
    // if(fragColor.a < 0.5) 
    // {
    //     discard;
    //     // gl_FragDepth = 0;
    //     gl_FragDepth = lightDistance;
    // }
    // else 
    // {
    //     // write this as modified depth
    //     gl_FragDepth = lightDistance;
    // }
    gl_FragDepth = lightDistance;
}
