#ifndef __CRANDOMOBJECTS_H
#define __CRANDOMOBJECTS_H

// --------------- C++ ---------------
#include <vector>
#include <string>
#include <stdlib.h> /* srand, rand */
#include <time.h>	/* time */

// --------------- OpenGL ---------------
#include <GL/glew.h>

// --------------- Utility ---------------
#include "obj_loader.hpp"
#include "light_material.h"
#include "CTriangle.h"
// #include "C_Mesh.cpp"

// // --------------- ---------------

class C_RandomObjects
{
public:
	std::vector<glm::vec4> Positions;
	GLuint NUMBER_OF_INSTANCES = 1;
	std::vector<C_Mesh> Objects;

	GLuint idVAO;
	GLuint obiectNumber;

	GLuint idVBO_coord;
	GLuint idVBO_uv;
	GLuint idVBO_normal;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	GLuint texture_id;
	GLfloat texture_scale;

	// TODO
	GLfloat texture_x;
	GLfloat texture_y;
	GLfloat texture_z;

	MaterialParam myMaterial;
	GLuint reflection;

	glm::mat4 matObj;
	glm::mat3 matNormal;

	std::vector<CTriangle> triangles;

	glm::vec3 Direction;
	float Angle;
	float radius = 0.5f;
	float scale = 1.0f;

	void SetRandomPositions(int _number_of_instances, float _xpos_max, float _zpos_max, C_Mesh &ground, float _moveY = 0.0f)
	{
		NUMBER_OF_INSTANCES = _number_of_instances;

		float Min_x = -_xpos_max;
		float Max_x = _xpos_max;
		float Min_z = -_zpos_max;
		float Max_z = _zpos_max;
		float Min_scale = 0.75;
		float Max_scale = 1.99;
		float y;

		for (int i = 0; i < NUMBER_OF_INSTANCES; i++)
		{
			float x = 0.0;
			// dopóki x jest na "wodzie"
			while ((x < 13.0 && x > -13.0))
			{
				printf("############## Czekaj... ##############\n");
				x = Min_x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Max_x - Min_x)));
			}
			// x = Min_x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Max_x - Min_x)));

			float z = Min_z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Max_z - Min_z)));
			float angle = 0.0 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (360.0 - 0.0)));
			scale = Min_scale + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Max_scale - Min_scale)));

			if (_moveY != 0.0)
				y = ground.getY(glm::vec2(x, z)) - _moveY;
			else
				y = ground.getY(glm::vec2(x, z));

			Positions.push_back(glm::vec4(x, y, z, scale));

			C_Mesh Obj;
			Obj.idVAO = idVAO;
			Obj.obiectNumber = obiectNumber;
			Obj.idVBO_coord = idVBO_coord;
			Obj.idVBO_uv = idVBO_uv;
			Obj.idVBO_normal = idVBO_normal;
			Obj.vertices = vertices;
			Obj.uvs = uvs;
			Obj.normals = normals;
			Obj.texture_id = texture_id;
			Obj.texture_scale = texture_scale;
			Obj.texture_x = texture_x;
			Obj.texture_y = texture_y;
			Obj.myMaterial = myMaterial;
			Obj.reflection = reflection;
			Obj.matObj = matObj;
			Obj.matNormal = matNormal;
			Obj.triangles = triangles;
			Obj.Position = Positions[i];
			Obj.Direction = Direction;
			Obj.Angle = angle;
			Obj.radius = radius * scale;
			Obj.scale = scale;
			Objects.push_back(Obj);
		}
	}

	void DrawRandom(GLuint _idProgram, float _angle = 0.0, glm::vec3 _rotate = glm::vec3(0.0f))
	{
		for (int i = 0; i < NUMBER_OF_INSTANCES; i++)
		{
			float x = Positions[i].x;
			float y = Positions[i].y;
			float z = Positions[i].z;
			scale = Positions[i].w;

			Objects[i].Translate(glm::vec3(x, y, z));
			Objects[i].Rotate(Objects[i].Angle, glm::vec3(0.0f, 1.0f, 0.0f));
			if (_angle != 0.0)
				Objects[i].Rotate(_angle, _rotate);
			Objects[i].Scale(glm::vec3(scale, scale, scale));
			Objects[i].Draw(_idProgram);
		}
	}

	void CreateFromOBJ(const char *filename, GLuint _obiect_number, float collision_radius = 0.5f)
	{
		obiectNumber = _obiect_number;
		radius = collision_radius;

		glGenVertexArrays(1, &idVAO);
		glBindVertexArray(idVAO);

		printf("CreateObject(%d)...\n", idVAO);
		if (!loadOBJ(filename, vertices, uvs, normals))
		{
			printf("Object not loaded!\n");
			exit(1);
		}

		// Bufor na wspolrzedne wierzcholkow
		glGenBuffers(1, &idVBO_coord);
		glBindBuffer(GL_ARRAY_BUFFER, idVBO_coord);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		// Bufor na wspolrzedne textury UV
		glGenBuffers(1, &idVBO_uv);
		glBindBuffer(GL_ARRAY_BUFFER, idVBO_uv);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		// Bufor na normalne
		glGenBuffers(1, &idVBO_normal);
		glBindBuffer(GL_ARRAY_BUFFER, idVBO_normal);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}

	void SetTexture(GLint _texture_id, GLfloat _texture_scale)
	{
		texture_id = _texture_id;
		texture_scale = _texture_scale;
	}

	void TerminateObject()
	{
		glDeleteVertexArrays(1, &idVAO);

		glDeleteVertexArrays(1, &idVBO_coord);
		glDeleteVertexArrays(1, &idVBO_normal);
		glDeleteVertexArrays(1, &idVBO_uv);
	}

	void SetMaterial(MaterialParam mat, GLuint _reflection)
	{
		myMaterial = mat;
		reflection = _reflection;
	}

	std::vector<glm::vec3> getPositionsVec3()
	{
		std::vector<glm::vec3> _Positions;
		for (int i = 0; i < NUMBER_OF_INSTANCES; i++)
		{
			_Positions.push_back(glm::vec3(Positions[i].x, Positions[i].y, Positions[i].z));
		}
		return _Positions;
	}
};

#endif