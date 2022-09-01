#ifndef __SHADER_STUFF
#define __SHADER_STUFF

// References:
// https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/loading.php

#include <fstream>
#include <map>

// --------------------------------------------------------------
// Camera view settings and variables
// --------------------------------------------------------------
glm::mat4 matProj;

GLfloat _wheel = 0.0f;
GLfloat _scene_rotate_x = 0.0f;
GLfloat _scene_rotate_y = 0.0f;
GLfloat _scene_translate_x = 0.0f;
GLfloat _scene_translate_y = 0.0f;
GLfloat _scene_translate_z = -10.0f;
GLfloat _camera_translate_x = 0.0f;
GLfloat _camera_translate_y = 0.0f;
GLfloat _camera_translate_z = -10.0f;

GLuint Width;
GLuint Height;

// ----------------------- PROGRAM -------------------------------
int _mouse_buttonState = GLUT_UP;
int _mouse_buttonX, _mouse_buttonY;
float tmp = 0.0f;
float move_x = 0.0f;
float move_y = 0.0f;
float move_z = 0.0f;
int texture_mode = 2;
bool viewMode = true;

// ----------------------- PLAYER -------------------------------
float player_move_rotation = 0.0f;
float moveSpeed = 0.25f;
float rotateSpeed = 0.06f;

// ----------------------- GAME -------------------------------
std::map<unsigned char, bool> state;
bool GAME_WIN = false;
bool GAMEOVER = false;
unsigned int LOSE_COUNT = 0;
unsigned int WIN_COUNT = 0;
unsigned int TIME_LIMIT = 400;
unsigned int TIME_LEFT = TIME_LIMIT;

// ------------------------------------------------------
glm::vec3 ExtractCameraPos(const glm::mat4 &a_modelView)
{
	glm::mat4 modelViewT = transpose(a_modelView);

	// Get plane normals
	glm::vec3 n1(modelViewT[0]);
	glm::vec3 n2(modelViewT[1]);
	glm::vec3 n3(modelViewT[2]);

	// Get plane distances
	float d1(modelViewT[0].w);
	float d2(modelViewT[1].w);
	float d3(modelViewT[2].w);

	// Get the intersection of these 3 planes
	glm::vec3 n2n3 = cross(n2, n3);
	glm::vec3 n3n1 = cross(n3, n1);
	glm::vec3 n1n2 = cross(n1, n2);

	glm::vec3 top = (n2n3 * d1) + (n3n1 * d2) + (n1n2 * d3);
	float denom = dot(n1, n2n3);

	return top / -denom;
}

// --------------------------------------------------------------
void SpecialKeys(int key, int x, int y)
{
	// switch (key)
	// {
	// case GLUT_KEY_LEFT:
	// 	_scene_translate_x += 0.1f;
	// 	break;
	// case GLUT_KEY_RIGHT:
	// 	_scene_translate_x -= 0.1f;
	// 	break;
	// case GLUT_KEY_UP:
	// 	_scene_translate_y -= 0.1f;
	// 	break;
	// case GLUT_KEY_DOWN:
	// 	_scene_translate_y += 0.1f;
	// 	break;
	// }

	glutPostRedisplay();
}

// --------------------------------------------------------------
void Keyboard_Down(unsigned char key, int x, int y)
{
	state[key] = true;

	switch (key)
	{
	case 27: // ESC key
		exit(0);
		break;

	case 'x':
		tmp += 0.1f;
		break;
	case 'z':
		tmp -= 0.1f;
		break;

	case 'i':
		move_x += 0.1f;
		break;
	case 'k':
		move_x -= 0.1f;
		break;
	case 'u':
		move_y += 0.1f;
		break;
	case 'o':
		move_y -= 0.1f;
		break;
	case 'j':
		move_z += 0.1f;
		break;
	case 'l':
		move_z -= 0.1f;
		break;

	case '1':
		texture_mode = 1;
		printf("Normals\n");
		break;
	case '2':
		texture_mode = 2;
		printf("Textures\n");
		break;
	case '3':
		texture_mode = 3;
		printf("Green\n");
		break;
	}

	// printf("(%f, %f, %f, %f)\n", move_x, move_y, move_z, tmp);
}

void Keyboard_Up(unsigned char key, int x, int y)
{
	state[key] = false;
}

// --------------------------------------------------------------
void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{

		_mouse_buttonState = state;

		if (state == GLUT_DOWN)
		{
			_mouse_buttonX = x;
			_mouse_buttonY = y;
		}
	}
}

// --------------------------------------------------------------
void MouseMotion(int x, int y)
{
	if (_mouse_buttonState == GLUT_DOWN)
	{
		_scene_rotate_y += 2 * (x - _mouse_buttonX) / (float)glutGet(GLUT_WINDOW_WIDTH);
		_mouse_buttonX = x;
		_scene_rotate_x -= 2 * (_mouse_buttonY - y) / (float)glutGet(GLUT_WINDOW_HEIGHT);
		_mouse_buttonY = y;
		glutPostRedisplay();
	}
}

// --------------------------------------------------------------
void MouseWheel(int button, int dir, int x, int y)
{
	if (dir == 1)
		_scene_translate_z += 1.5f;
	else
		_scene_translate_z -= 1.5f;
	// printf("MouseWheel(%d, %d, %d, %d, %f); \n", button, dir, x, y, _wheel);
	glutPostRedisplay();
}

// ---------------------------------------------
unsigned long getFileLength(std::ifstream &file)
{
	if (!file.good())
		return 0;

	unsigned long pos = file.tellg();
	file.seekg(0, std::ios::end);
	unsigned long len = file.tellg();
	file.seekg(std::ios::beg);

	return len;
}

// ---------------------------------------------
GLchar *LoadShaderFile(const char *filename)
{
	std::ifstream file;
	file.open(filename, std::ios::in); // opens as ASCII!
	if (!file)
	{
		printf("Can't open file %s !\n", filename);
		exit(1);
	}

	unsigned long len = getFileLength(file);

	if (len == 0)
	{
		printf("File %s is empty!\n", filename);
		exit(1);
	}; // Error: Empty File

	GLchar *ShaderSource = new GLchar[len + 1];
	if (ShaderSource == NULL)
	{
		printf("Can't reserve memory %d \n", len + 1);
		exit(1);
	} // can't reserve memory

	// len isn't always strlen cause some characters are stripped in ascii read...
	// it is important to 0-terminate the real length later, len is just max possible value...
	ShaderSource[len] = 0;

	unsigned int i = 0;
	while (file.good())
	{
		ShaderSource[i] = file.get();
		if (!file.eof())
			i++;
	}

	ShaderSource[i] = 0;
	file.close();
	return ShaderSource;
}

// ---------------------------------------
void CheckForErrors_Shader(GLuint shader)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		printf("Blad!\n");
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		char *log = new char[logLength];
		glGetShaderInfoLog(shader, logLength, NULL, log);
		printf("LOG: %s\n", log);
		delete[] log;
		exit(1);
	}
}

// ---------------------------------------
void CheckForErrors_Program(GLuint program, GLenum mode)
{
	GLint status;
	glGetProgramiv(program, mode, &status);
	if (status != GL_TRUE)
	{
		switch (mode)
		{
		case GL_LINK_STATUS:
			printf("Blad konsolidacji programu!\n");
			break;
		case GL_VALIDATE_STATUS:
			printf("Blad walidacji programu!\n");
			break;
		default:
			printf("Inny blad programu!\n");
		}
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		char *log = new char[logLength];
		glGetProgramInfoLog(program, logLength, NULL, log);
		printf("LOG: %s\n", log);
		delete[] log;
		exit(1);
	}
}

// ---------------------------------------
GLuint LoadShader(GLuint MODE, const char *filename)
{
	// utworzenie obiektu shadera
	GLuint shader = glCreateShader(MODE);

	// Wczytanie kodu shadera z pliku
	GLchar *code = LoadShaderFile(filename);

	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);
	CheckForErrors_Shader(shader);

	delete[] code;
	return shader;
}

// ---------------------------------------
void LinkAndValidateProgram(GLuint program)
{
	// Konsolidacja programu
	glLinkProgram(program);
	CheckForErrors_Program(program, GL_LINK_STATUS);

	// Walidacja programu
	glValidateProgram(program);
	CheckForErrors_Program(program, GL_VALIDATE_STATUS);
}

#endif
