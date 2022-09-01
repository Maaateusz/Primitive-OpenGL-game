#ifndef Light_Material
#define Light_Material

// --------------- OpenGL ---------------
#include <GL/glew.h>

// Oswietlenie punktowe
struct LightPoint
{
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	glm::vec3 Att;
	float Shininess;
	glm::vec3 Position;
};

// Oswietlenie kierunkowe
struct LightDirect
{
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	glm::vec3 Direction;
	glm::vec3 Position;
};

// MATERIALY
struct MaterialParam
{
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	float Shininess;
};

// --------------- Materiały ---------------
MaterialParam material_1 = {
	glm::vec3(0.6f, 0.66f, 0.9f),
	glm::vec3(0.99f, 0.99f, 0.99f),
	glm::vec3(0.727811f, 0.626959f, 0.626959f),
	90.0f};

MaterialParam material_2 = {
	glm::vec3(0.05f, 0.05f, 0.05f),
	glm::vec3(0.99f, 0.99f, 0.99f),
	glm::vec3(0.07f, 0.07f, 0.07f),
	10.0f};

MaterialParam material_3 = {
	glm::vec3(0.05f, 0.05f, 0.05f),
	glm::vec3(0.99f, 0.99f, 0.99f),
	glm::vec3(0.07f, 0.07f, 0.07f),
	20.0f};

MaterialParam material_4 = {
	glm::vec3(0.6f, 0.66f, 0.9f),
	glm::vec3(0.99f, 0.99f, 0.99f),
	glm::vec3(0.727811f, 0.626959f, 0.626959f),
	100.0f};

// --------------- Światła ---------------
LightDirect myLightDirect = {
	glm::vec3(0.3, 0.3, 0.3),
	glm::vec3(2.0, 2.0, 2.0),
	glm::vec3(0.7, 0.7, 0.3),
	glm::vec3(-1.0, -1.0, -1.0),
	glm::vec3(0.0, 5.0, 0.0)};



	// vec3 Ambient; //kolor
	// vec3 Diffuse; //cień, kolor
	// vec3 Specular; //odbicie
	// vec3 Att; //Tłumienie światła; (constant, linear, quadratic)
	// float Shininess; //Połysk
	// vec3 Position; //pozycja

LightPoint myLightPoint[] = {
	{
		glm::vec3(1.0, 1.0, 1.0),
		glm::vec3(1.0, 1.0, 1.0),
		glm::vec3(1.0, 1.0, 1.0),
		glm::vec3(0.01, 0.01, 0.01),
		64.0,
		glm::vec3(0.0, 4.0, 0.0)},
	{
		glm::vec3(1.0, 0.0, 0.0),
		glm::vec3(1.0, 0.0, 0.0),
		glm::vec3(1.0, 0.1, 0.1),
		glm::vec3(0.01, 0.01, 0.01),
		64.0,
		glm::vec3(-15.0, 4.0f, -15.0)
	 },
	{
		glm::vec3(0.0, 1.0, 0.0),
		glm::vec3(0.0, 1.0, 0.0),
		glm::vec3(0.1, 1.0, 0.1),
		glm::vec3(0.01, 0.01, 0.01),
		64.0,
		glm::vec3(15.0, 4.0f, -15.0)}
	 };

// LightPoint myLightPoint[] = {
// 	{glm::vec3(0.3, 0.3, 0.3),
// 	 glm::vec3(1.0, 1.0, 1.0),
// 	 glm::vec3(0.7, 0.7, 0.3),
// 	 glm::vec3(0.7, 0.0, 0.0),
// 	 32.0,
// 	 glm::vec3(0.0, 4.0, 0.0)},

// 	{glm::vec3(0.3, 0.3, 0.3),
// 	 glm::vec3(1.0, 0.0, 0.0),
// 	 glm::vec3(1.0, 0.3, 0.3),
// 	 glm::vec3(0.7, 0.01, 0.01),
// 	 64.0,
// 	 glm::vec3(-15.0, 4.0f, -15.0)},

// 	{glm::vec3(0.3, 0.3, 0.3),
// 	 glm::vec3(0.0, 1.0, 0.0),
// 	 glm::vec3(0.3, 1.0, 0.3),
// 	 glm::vec3(0.7, 0.01, 0.01),
// 	 64.0,
// 	 glm::vec3(15.0, 4.0f, -15.0)}};

int Number_of_lights = (sizeof(myLightPoint) / sizeof(LightPoint));

#endif