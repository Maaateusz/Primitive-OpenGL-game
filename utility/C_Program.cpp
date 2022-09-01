// --------------- C++ ---------------
#include <string>

// --------------- OpenGL ---------------
#include <GL/glew.h>

// --------------- Utility ---------------
#include "light_material.h"

// --------------- ---------------

class C_Program
{
	GLuint idProgram;
	LightPoint myLightPoint;
	LightDirect myLightDirect;

	glm::mat4 matProj;
	glm::mat4 matView;
	glm::mat4 matProjView;
	glm::vec3 eyePosition;

public:
	// Metoda tworzaca program, jego identyfikator
	void CreateProgram()
	{
		idProgram = glCreateProgram();
		printf("CreateProgram(%d)\n", idProgram);
	}

	// Metoda ladujaca shadery:
	void LoadShaders(const char *vertex_filename, const char *fragment_filename)
	{
		glAttachShader(idProgram, LoadShader(GL_VERTEX_SHADER, vertex_filename));
		glAttachShader(idProgram, LoadShader(GL_FRAGMENT_SHADER, fragment_filename));
		LinkAndValidateProgram(idProgram);
	}

	void SendUniformFloat(GLfloat dataToSend, const char *nameData)
	{
		glUniform1f(glGetUniformLocation(idProgram, nameData), dataToSend);
	}

	void SendUniformInt(GLint dataToSend, const char *nameData)
	{
		glUniform1i(glGetUniformLocation(idProgram, nameData), dataToSend);
	}

	void SendObjectMatrix(glm::mat4 matMVP)
	{
		glUniformMatrix4fv(glGetUniformLocation(idProgram, "matObj"), 1, GL_FALSE, &matMVP[0][0]);
	}

	void SendMat4(glm::mat4 mat, const char *nameData)
	{
		glUniformMatrix4fv(glGetUniformLocation(idProgram, nameData), 1, GL_FALSE, &mat[0][0]);
	}

	void SendMat3(glm::mat3 mat, const char *nameData)
	{
		glUniformMatrix3fv(glGetUniformLocation(idProgram, nameData), 1, GL_FALSE, &mat[0][0]);
	}

	void SendVec3(glm::vec3 vec, const char *nameData)
	{
		glUniform3fv(glGetUniformLocation(idProgram, nameData), 1, &vec[0]);
	}

	void SendCameraPosition(glm::vec3 eyePosition)
	{
		glUniform3fv(glGetUniformLocation(idProgram, "eyePosition"), 1, &eyePosition[0]);
	}

	// void SetMVP(matMVP)
	void SendViewMatrix(glm::mat4 matMVP)
	{
		glUniformMatrix4fv(glGetUniformLocation(idProgram, "matProjView"), 1, GL_FALSE, &matMVP[0][0]);
	}

	// aktywuje program
	void Use()
	{
		glUseProgram(idProgram);
	}

	void sendPointLight(LightPoint myLightPoint, GLuint i)
	{
		std::string number = std::to_string(i);
		glUniform3fv(glGetUniformLocation(idProgram, ("myLightPoint[" + number + "].Ambient").c_str()), 1, &myLightPoint.Ambient[0]);
		glUniform3fv(glGetUniformLocation(idProgram, ("myLightPoint[" + number + "].Diffuse").c_str()), 1, &myLightPoint.Diffuse[0]);
		glUniform3fv(glGetUniformLocation(idProgram, ("myLightPoint[" + number + "].Specular").c_str()), 1, &myLightPoint.Specular[0]);
		glUniform3fv(glGetUniformLocation(idProgram, ("myLightPoint[" + number + "].Att").c_str()), 1, &myLightPoint.Att[0]);
		glUniform1f(glGetUniformLocation(idProgram, ("myLightPoint[" + number + "].Shininess").c_str()), myLightPoint.Shininess);
		glUniform3fv(glGetUniformLocation(idProgram, ("myLightPoint[" + number + "].Position").c_str()), 1, &myLightPoint.Position[0]);
	}

	void TerminateProgram()
	{
		glDeleteProgram(idProgram);
	}

	void SetLightDirect(LightDirect lightDirect)
	{
		myLightDirect = lightDirect;
	}

	LightDirect GetLightDirect()
	{
		return myLightDirect;
	}

	void SetLightPoint(LightPoint lightPoint)
	{
		myLightPoint = lightPoint;
	}

	LightPoint GetLightPoint()
	{
		return myLightPoint;
	}

	GLuint getId()
	{
		return idProgram;
	}
};