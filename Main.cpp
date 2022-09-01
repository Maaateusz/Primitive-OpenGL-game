// --------------- C++ ---------------
#include <vector>
#include <stdio.h>
#include <string>

// --------------- OpenGL ---------------
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// --------------- Utility ---------------
#include "utility/shader_stuff.hpp"
#include "utility/obj_loader.hpp"
#include "utility/C_Mesh.cpp"
#include "utility/C_RandomObjects.cpp"
#include "utility/C_Program.cpp"
#include "utility/error.cpp"
#include "utility/light_material.h"
#include "utility/Functions.cpp"
#include "utility/C_Font.cpp"
#include "utility/ShadowPoint.h"
#include "utility/SkyBox.h"
#include "utility/Camera.h"
#include "utility/CameraAbove.h"

// --------------- Program ---------------
C_Program MainProgram;
C_Program FontProgram;
C_Program InstancedRenderingProgram;
C_SkyBox SkyBoxProgram;

// --------------- Textury ---------------
GLuint Land_txture;
GLuint Grass_txture;
GLuint Cat_txture;
GLuint Wall_txture;
GLuint Tree_txture;
GLuint Water_txture;
GLuint Player_txture;
GLuint Obstacle_txture;
GLuint Particle_txture;

// --------------- Obiekty ---------------
C_Mesh Land;
C_Mesh Wall;
C_Mesh Water;
C_RandomObjects Obstacle;
C_RandomObjects Tree;
C_RandomObjects Grass;
C_Mesh Player;
C_Mesh Cat;
C_Mesh Particles;

// --------------- Fonts ---------------
C_Font Font_1;
C_Font Font_2;

// --------------- Lights ---------------
C_ShadowPointLight myLight[3];

// --------------- Camera ---------------
CFPSCamera camera;
CameraAbove cameraAbove;

// ##############################################################################

void RenderMainScene(GLuint _program)
{
	Water.texture_x = water_anim;
	Water.Draw(_program);

	Wall.Draw(_program);

	Land.Draw(_program);

	Cat.Translate(Cat.Position);
	Cat.Rotate(glm::sin(cat_rotation) * 10, glm::vec3(0.0, 1.0, 0.0));
	Cat.Draw(_program);

	Tree.DrawRandom(_program);

	Obstacle.DrawRandom(_program, tree_rotate_anim * 50.0, glm::vec3(0.0, 1.0, 0.0));

	Grass.DrawRandom(_program, glm::sin(tree_rotate_anim) * 8.0, glm::vec3(0.0, 1.0, 0.0));

	// ---------------------- Player ----------------------
	Player.UpdateY(Land.getY(glm::vec2(Player.getPosX(), Player.getPosZ())) + 1.0f);

	Player.nextMoveSpeed = moveSpeed;

	if (state['a'])
	{
		Player.Rotate(rotateSpeed);
	}
	else if (state['d'])
	{
		Player.Rotate(-rotateSpeed);
	}

	if (!state['w'])
	{
		camera.SetFov(90.0f);
	}

	if (state['w'])
	{
		Player.CheckCollisionVec3_GAMEOVER(Obstacle.getPositionsVec3(), Obstacle.radius);
		Player.CheckCollisionVec3(Tree.getPositionsVec3(), Tree.radius);
		Player.CheckCollisionObj_WIN(Cat);
		camera.SetFov(110.0f);

		Player.MoveForward();

		Player.Rotate(player_move_rotation += 4.0, glm::vec3(0.0, 0.0, -1.0));
	}
	else if (state['s'])
	{
		Player.MoveBackward();
	}

	Player.Draw(_program);
	// --------------------------------------------
}

void DrawToShadowMap()
{
	for (GLuint i = 0; i < Number_of_lights; i++)
	{
		myLight[i].UpdatePV();
		myLight[i].GenBegin();

		RenderMainScene(myLight[i].idProgram);

		myLight[i].GenEnd();
	}
}

void DrawOnScreen()
{
	// 2. Renderowanie z pozycji kamery na ekran
	glViewport(0, 0, Width, Height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ####################################################################
	MainProgram.Use();

	// Shadow map textura na slot 2 5+id
	for (GLuint i = 0; i < Number_of_lights; i++)
	{
		myLight[i].SendUniform(MainProgram.getId(), 5 + i, i);
		MainProgram.sendPointLight(myLight[i].my_Light_Point, i);
	}
	// ####################################################################

	// Światło odbite i załamane, SkyBox; na slot 3
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxProgram.getTextureId());
	MainProgram.SendUniformInt(3, "textureSkyBox");

	MainProgram.SendUniformFloat(tmp, "tmp");
	MainProgram.SendUniformInt(light_mode, "light_mode");
	MainProgram.SendUniformInt(texture_mode, "texture_mode");
	MainProgram.SendUniformFloat(flowers_anim, "flowers_anim");

	if (state['v'])
		cameraAbove.Update(MainProgram.getId(), Player.getPosition(), Player.getAngle());
	else
		camera.Update(MainProgram.getId(), Player.getPosition(), Player.getAngle());

	// ----------------------------------------------------------------------
	RenderMainScene(MainProgram.getId());

	// ------------- SKyBox -----------------------
	SkyBoxProgram.Config(camera.getMatProjView());

	// ---------------- Instanced --------------------
	InstancedRenderingProgram.Use();
	InstancedRenderingProgram.SendUniformFloat(particles_anim, "anim");
	camera.SendPV(InstancedRenderingProgram.getId());
	Particles.DrawInstanced(InstancedRenderingProgram.getId());

	// ---------------- Fonts --------------------
	Font_1.RenderText("FPS: " + std::to_string(fps), 0, Height - 28, 1.0f, glm::vec3(0.1f, 0.1f, 1.0f)); //Lewo góra
	Font_2.RenderText("by Mateusz", Width - 180, 1, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));					 //Prawo dół

	Font_2.RenderText("Time left: " + std::to_string(TIME_LEFT), 3, 80, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	Font_2.RenderText("Rounds lost: " + std::to_string(LOSE_COUNT), 3, 50, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	Font_2.RenderText("Rounds win: " + std::to_string(WIN_COUNT), 3, 20, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	Font_1.RenderText("+", Width / 2, Height / 2, 1.0f, glm::vec3(0.0f, 0.8f, 0.0f));
	// if (!GAMEOVER)
	// {
	// 	Font_1.RenderText("+", Width / 2, Height / 2, 1.0f, glm::vec3(0.0f, 0.8f, 0.0f));
	// }
	// if (GAMEOVER)
	// {
	// 	Font_1.RenderText("GAMEOVER", Width / 2, Height / 2, 1.0f, glm::vec3(0.0f, 0.8f, 0.0f));
	// 	Sleep(500);
	// }
	// if (GAME_WIN)
	// {
	// 	Font_1.RenderText("WIN", Width / 2, Height / 2, 1.0f, glm::vec3(0.0f, 0.8f, 0.0f));
	// 	Sleep(500);
	// }
	// ##############################################################################

	glUseProgram(0);
}

void DisplayScene()
{
	__CHECK_FOR_ERRORS;

	Width = glutGet(GLUT_WINDOW_WIDTH);
	Height = glutGet(GLUT_WINDOW_HEIGHT);

	CalculateFrameRate();

	// ustawianie świateł punktowych
	myLight[0].my_Light_Point.Position = Player.Position;
	myLight[0].my_Light_Point.Position.y += 15.0;

	myLight[1].my_Light_Point.Position = Cat.Position;
	myLight[1].my_Light_Point.Position.y += 15.0;

	myLight[2].my_Light_Point.Position = Matrix_rotate_Y(light_rotate_anim) * glm::vec4(25.0, 20.0f, -25.0, 1.0f);

	DrawToShadowMap();
	DrawOnScreen();
	TIME_LEFT--;

	// ----------------- LOGIKA ------------------
	if (TIME_LEFT <= 0)
	{
		GAMEOVER = true;
	}
	if (GAMEOVER)
	{
		printf("You Lose\n");
		Font_1.RenderText("GAMEOVER", Width / 2, Height / 2, 1.0f, glm::vec3(0.0f, 0.8f, 0.0f));
		Sleep(500);
		GAMEOVER = false;
		GAME_WIN = false;
		TIME_LEFT = TIME_LIMIT;
		LOSE_COUNT++;
		Player.Reset();
	}
	if (GAME_WIN)
	{
		printf("You WIN\n");
		Font_1.RenderText("WIN", Width / 2, Height / 2, 1.0f, glm::vec3(0.0f, 0.8f, 0.0f));
		Sleep(500);
		GAME_WIN = false;
		GAMEOVER = false;
		TIME_LEFT = TIME_LIMIT;
		WIN_COUNT++;
		Player.Reset();
		Cat.SetRandomPosition(Land);
	}

	// -----------------------------------
	glutSwapBuffers();

	// ?? ??
	Sleep(50);
}

// ##############################################################################

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	camera.SetPerspective(width, height, 80.0f);
	cameraAbove.SetPerspective(width, height, 80.0f);
}

// ##############################################################################

void Initialize()
{
	printf("Initialize...\n");
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	srand((unsigned)time(NULL));

	// ---------- SkyBox ----------
	SkyBoxProgram.CreateSkyBox();

	// ---------- Programs ----------
	MainProgram.CreateProgram();
	MainProgram.LoadShaders("shaders/Main.vert", "shaders/Main.frag");

	SkyBoxProgram.CreateProgram();
	SkyBoxProgram.LoadShaders("shaders/SkyBox.vert", "shaders/SkyBox.frag");

	FontProgram.CreateProgram();
	FontProgram.LoadShaders("shaders/text-ft-vertex.glsl", "shaders/text-ft-fragment.glsl");

	InstancedRenderingProgram.CreateProgram();
	InstancedRenderingProgram.LoadShaders("shaders/vertex-instanced.glsl", "shaders/fragment-instanced.glsl");

	// ---------- Fonts ----------
	Font_1.InitText(FontProgram.getId(), "fonts/DS-DIGIT.TTF", 28);
	Font_2.InitText(FontProgram.getId(), "fonts/space age.ttf", 20);

	// ---------- Textures ----------
	Land_txture = createTexture("textures/Land.jpg");
	Cat_txture = createTexture("textures/Cat.png");
	Grass_txture = createTexture("textures/Grass.png");
	Wall_txture = createTexture("textures/Wall.jpg");
	Player_txture = createTexture("textures/Player.jpg");
	Water_txture = createTexture("textures/Water.jpg");
	Tree_txture = createTexture("textures/Tree.jpg");
	Obstacle_txture = createTexture("textures/Wall.jpg");
	Particle_txture = createTexture("textures/Particle.png");

	// ---------- Objects ----------
	Land.CreateFromOBJ("objects/Land.obj", 0);
	Land.SetTexture(Land_txture, 5.0);
	Land.SetMaterial(material_1, 0);
	Land.SetAsGround();

	Player.CreateFromOBJ("objects/Player.obj", 2, 0.7f);
	Player.SetTexture(Player_txture, 1.0);
	Player.SetMaterial(material_4, 1);
	float pX = -25.0;
	float pZ = -25.0;
	Player.SetAsPlayer(glm::vec3(pX, Land.getY(glm::vec2(pX, pZ)), pZ), glm::vec3(1.0, 0.0, 1.0));
	Player.triangles = Land.triangles;

	Cat.CreateFromOBJ("objects/Cat.obj", 3, 0.8f);
	Cat.SetTexture(Cat_txture, 1.0);
	Cat.SetMaterial(material_4, 0);
	Cat.SetRandomPosition(Land);

	Wall.CreateFromOBJ("objects/Wall.obj", 4);
	Wall.SetTexture(Wall_txture, 10.0);
	Wall.SetMaterial(material_4, 0);

	Water.CreateFromOBJ("objects/Water.obj", 5);
	Water.SetTexture(Water_txture, 5.0);
	Water.SetMaterial(material_4, 1);

	Tree.CreateFromOBJ("objects/Tree.obj", 6, 1.0f);
	Tree.SetTexture(Tree_txture, 1.0);
	Tree.SetMaterial(material_3, 0);
	Tree.SetRandomPositions(100, 70, 70, Land);

	Obstacle.CreateFromOBJ("objects/Obstacle.obj", 7, 1.1f);
	Obstacle.SetTexture(Obstacle_txture, 1.0);
	Obstacle.SetMaterial(material_2, 0);
	Obstacle.SetRandomPositions(100, 70, 70, Land);

	Grass.CreateFromOBJ("objects/Grass.obj", 8);
	Grass.SetTexture(Grass_txture, -1.0);
	Grass.SetMaterial(material_4, 0);
	Grass.SetRandomPositions(250, 70, 70, Land);

	Particles.CreateFromOBJInstanced("objects/Particle.obj", 1500, 9);
	Particles.SetTexture(Particle_txture, 1.0);
	Particles.SetMaterial(material_4, 0);

	// ---------- Camera ----------
	camera.Init();
	cameraAbove.Init();

	// ---------- Point Shadow ----------
	for (GLuint i = 0; i < Number_of_lights; i++)
	{
		myLight[i].Init(myLightPoint[i]);
	}
}

// ##############################################################################

int main(GLuint argc, char *argv[])
{
	GlutAppInit(argc, argv);
	GlewAppInit();
	OpenGlAppInit();
	MenuInit();

	Initialize();

	// Funkcje Glut
	glutDisplayFunc(DisplayScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutMouseWheelFunc(MouseWheel);
	glutKeyboardFunc(Keyboard_Down);
	glutKeyboardUpFunc(Keyboard_Up);
	glutSpecialFunc(SpecialKeys);

	//Animacje
	glutTimerFunc(100, Animation, 0);
	glutMainLoop();

	// Cleaning
	MainProgram.TerminateProgram();
	SkyBoxProgram.TerminateProgram();
	FontProgram.TerminateProgram();
	InstancedRenderingProgram.TerminateProgram();

	Land.TerminateObject();
	Obstacle.TerminateObject();
	Tree.TerminateObject();
	Grass.TerminateObject();
	Water.TerminateObject();
	Wall.TerminateObject();
	Particles.TerminateObject();

	return 0;
}
