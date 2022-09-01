#version 150

// Liczba świateł punktowych
#define Number_of_lights 3  

// --- IN ---
in vec4 Coords_frag;
in vec2 UV_frag;
in vec3 Normals_frag;

// --- STRUCT ---
struct LightPoint
{
	vec3 Ambient; //kolor
	vec3 Diffuse; //cień
	vec3 Specular; //odbicie
	vec3 Att; //Tłumienie światła; (constant, linear, quadratic)
	float Shininess; //Połysk
	vec3 Position; //pozycja
};

struct MaterialParam
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Shininess;
};

// --- UNIFORM ---
// uniform int obiectID;
uniform sampler2D Texture;
uniform samplerCube textureSkyBox;
uniform samplerCube shadowMapPoint[Number_of_lights];
uniform LightPoint myLightPoint[Number_of_lights];
uniform float pointLightDistanceFar;

uniform int light_mode;
uniform int texture_mode;
uniform int reflection;
uniform int obiectNumber;
uniform vec3 eyePosition;
uniform float texture_scale;
uniform float texture_x;
uniform float tmp;
uniform MaterialParam myMaterial;


// --- OUT ---
out vec4 outColor;

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

// Obliczanie jasnosci dla oswietlenia punktowego
vec3 calculatePointLight(LightPoint myLight)
{
	vec3 ambientPart = myLight.Ambient * myMaterial.Ambient;

	// Składowa tłumienia
	float DLV = length(myLight.Position - Coords_frag.xyz); //distance
	float Latt = 1.0 / (myLight.Att.x + myLight.Att.y * DLV + myLight.Att.z * DLV * DLV);

	//Diffuse part
	vec3 L = normalize(myLight.Position - Coords_frag.xyz);
	vec3 diffusePart = max(dot(L, Normals_frag), 0.0) * myLight.Diffuse * myMaterial.Diffuse;

	// Specular part
	vec3 E = normalize(eyePosition - Coords_frag.xyz);
	vec3 R = reflect(-E, Normals_frag);
	vec3 specularPart = pow(max(dot(R, L), 0.0), myLight.Shininess/10.0) * myLight.Specular * myMaterial.Specular;

	//światło wyjściowe 
	return Latt * (ambientPart + diffusePart + specularPart);
	// return (ambientPart/10) + Latt * (diffusePart + specularPart);
	// return Latt * (diffusePart + specularPart);
	// return ambientPart + Latt * (diffusePart + specularPart);
}

float calculatePointShadow(vec3 _lightPosition, samplerCube _shadowMapPoint)
{
	// get vector between fragment position and light position
    vec3 fragToLight = Coords_frag.xyz - _lightPosition;

	// now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);

    // use the light to fragment vector to sample from the depth map
    float closestDepth = texture(_shadowMapPoint, fragToLight).r * pointLightDistanceFar;

	float bias = 0.001;
	float shadow = 0.0;

	//  simple test
	// shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
	// return shadow;


	// filtering
    int samples = 15;
    float viewDistance = length(eyePosition - Coords_frag.xyz);
    float diskRadius = (1.0 + (viewDistance / pointLightDistanceFar)) / 25.0;

    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(_shadowMapPoint, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= pointLightDistanceFar;   // undo mapping [0;1]

        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }

    shadow /= float(samples);

    return shadow;
}

void main()
{
	// ---------- główne składowe ----------
	vec4 fragColor;

	vec4 lightPart;
	float shadowPart;
	vec4  finalColor;

	// ---------- Reflection & Refraction ----------
	float fragRatio = 2.5;
	float reflRatio = 15.0;
	float refrRatio = 15.0;

	vec3 D = normalize(Coords_frag.xyz - eyePosition);

	// Światło odbite
	vec3 Refl = reflect(D, Normals_frag);
	vec4 ReflColor = texture(textureSkyBox, Refl);

	// Światło załamane
	vec3 Refr = refract(D, Normals_frag, 1.0/1.52);
	vec4 RefrColor = texture(textureSkyBox, Refr);

	// ---------- Texture ------------------------------------------------------------
	if(texture_mode == 1)
	{
		fragColor = vec4(Normals_frag, 1.0);
	}
	else if(texture_mode == 2)
	{
		vec2 UV = UV_frag;
		UV.x = UV_frag.x + texture_x;
		fragColor = texture(Texture, UV * texture_scale);
		if(fragColor.a < 0.5) 
			discard;
	}
	else if(texture_mode == 3)
	{
		fragColor = vec4(0.1, 0.9, 0.1, 1.0);
	}

	// ---------- Light ------------------------------------------------------------
	// No light
	if(light_mode == 1)
	{
		outColor = vec4(fragColor);
	}
	// Point Light
	else if(light_mode == 2)
	{
		lightPart = vec4(0.0, 0.0, 0.0, 1.0);
		for( int i = 0; i < Number_of_lights; i ++)
		{
			lightPart += vec4(calculatePointLight(myLightPoint[i]), 1.0);
		}
		
		if(reflection == 0) 
			outColor = vec4(lightPart * fragColor);
		else 
			outColor = lightPart * fragColor/fragRatio + ReflColor/reflRatio + RefrColor/refrRatio;
	}
	// Point Light + Shadow
	else if(light_mode == 3)
	{
		lightPart = vec4(0.0);
		shadowPart = 0.0;
		for( int i = 0; i < Number_of_lights; i ++)
		{
			lightPart += vec4(calculatePointLight(myLightPoint[i]), 1.0);
			shadowPart += calculatePointShadow(myLightPoint[i].Position, shadowMapPoint[i]);
		}

		float ambientExtra = 0.1;


		if(reflection == 0) 
			finalColor = vec4((ambientExtra + (1 - shadowPart) * lightPart) *  fragColor);
		else 
			finalColor = vec4((ambientExtra + (1 - shadowPart) * lightPart) * fragColor/fragRatio + ReflColor/reflRatio + RefrColor/refrRatio);



		// if(reflection == 0) 
		// {
		// 	finalColor = vec4((ambientExtra + (1 - shadowPart) * lightPart) * fragColor);
		// }
		// else 
		// {
		// 	vec4  withReflColor = lightPart * fragColor/fragRatio + ReflColor/reflRatio + RefrColor/refrRatio;
		// 	finalColor = vec4((ambientExtra + (1 - shadowPart) * withReflColor) * fragColor);
		// }

		outColor = finalColor;
	}
}
