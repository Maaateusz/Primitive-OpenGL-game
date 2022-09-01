#ifndef __CMESH_H
#define __CMESH_H

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
// #include "C_RandomObjects.cpp"

// // --------------- ---------------

class C_Mesh
{
public:
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
	GLfloat texture_x = 1.0f;
	GLfloat texture_y = 1.0f;
	MaterialParam myMaterial;
	GLuint reflection;

	glm::mat4 matObj;
	glm::mat3 matNormal;

	std::vector<glm::vec4> multiPos;

	GLuint NUMBER_OF_INSTANCES = 1;

	std::vector<CTriangle> triangles;
	C_Mesh *ground = nullptr;

	glm::vec3 Position;
	glm::vec3 StartPosition;
	glm::vec3 Direction;
	glm::vec3 StartDirection;
	float Angle;
	float radius = 0.5f;
	float scale = 1.0f;

	bool canMove = true;
	float nextMoveSpeed = 0.678f;

	// ------------------------ CREATE ------------------------
	// Metody tworzaca VAO i VBO z pliku OBJ
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

	void CreateFromOBJInstanced(const char *filename, GLuint _number, GLuint _obiect_number)
	{
		obiectNumber = _obiect_number;

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

		NUMBER_OF_INSTANCES = _number;
		glm ::mat4 matModels[NUMBER_OF_INSTANCES];

		float Min_x = -70.0;
		float Max_x = 70.0;

		float Min_y = -8.0;
		float Max_y = 30.0;

		float Min_z = -70.0;
		float Max_z = 70.0;

		float Min_scale = 0.2;
		float Max_scale = 0.8;

		for (int i = 0; i < NUMBER_OF_INSTANCES; i++)
		{
			float x = Min_x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Max_x - Min_x)));
			float z = Min_z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Max_z - Min_z)));
			float y = Min_y + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Max_y - Min_y)));
			float scale = Min_scale + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Max_scale - Min_scale)));
			float rot = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (360.f)));

			matModels[i] = glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));
			matModels[i] = glm::rotate(matModels[i], glm::radians(rot), glm::vec3(0.0, 1.0, 0.0));
			matModels[i] = glm::scale(matModels[i], glm::vec3(scale));
		}

		// Wektory normalne
		GLuint vBuffer_inst;
		glGenBuffers(1, &vBuffer_inst);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer_inst);
		glBufferData(GL_ARRAY_BUFFER, NUMBER_OF_INSTANCES * sizeof(glm::mat4), &matModels[0], GL_STATIC_DRAW);

		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm ::mat4), (void *)0);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm ::mat4), (void *)(sizeof(glm ::vec4)));
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm ::mat4), (void *)(2 * sizeof(glm ::vec4)));
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm ::mat4), (void *)(3 * sizeof(glm ::vec4)));

		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	// ------------------------ DRAW ------------------------
	// Metoda generujaca obraz na ekranie
	void Draw(GLuint _idProgram)
	{
		glUseProgram(_idProgram);

		// Wysyłanie wszystkich parametrów do programu
		// -- Textura skala
		glUniform1f(glGetUniformLocation(_idProgram, "texture_scale"), texture_scale);
		glUniform1f(glGetUniformLocation(_idProgram, "texture_x"), texture_x);
		// -- macierz normalnych
		matNormal = glm::mat3(transpose(inverse(matObj)));
		glUniformMatrix3fv(glGetUniformLocation(_idProgram, "matNormal"), 1, GL_FALSE, &matNormal[0][0]);
		// -- macierz obiektu
		glUniformMatrix4fv(glGetUniformLocation(_idProgram, "matObj"), 1, GL_FALSE, &matObj[0][0]);
		// -- Id
		// glUniform1i(glGetUniformLocation(_idProgram, "obiectID"), idVAO);
		glUniform1i(glGetUniformLocation(_idProgram, "obiectNumber"), obiectNumber);
		// -- Materiał
		glUniform3fv(glGetUniformLocation(_idProgram, "myMaterial.Ambient"), 1, &myMaterial.Ambient[0]);
		glUniform3fv(glGetUniformLocation(_idProgram, "myMaterial.Diffuse"), 1, &myMaterial.Diffuse[0]);
		glUniform3fv(glGetUniformLocation(_idProgram, "myMaterial.Specular"), 1, &myMaterial.Specular[0]);
		glUniform1f(glGetUniformLocation(_idProgram, "myMaterial.Shininess"), myMaterial.Shininess);
		glUniform1i(glGetUniformLocation(_idProgram, "reflection"), reflection);

		// Rysowanie
		glBindVertexArray(idVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);

		glUniform1i(glGetUniformLocation(_idProgram, "Texture"), 0);

		// Reset
		matObj = glm::mat4x4(1.0);
		matNormal = glm::mat4x4(1.0);
		canMove = true;
	}

	void DrawInstanced(GLuint _idProgram)
	{
		glUseProgram(_idProgram);

		// Wysyłanie wszystkich parametrów do programu
		glUniform1f(glGetUniformLocation(_idProgram, "texture_scale"), texture_scale);
		glUniform1i(glGetUniformLocation(_idProgram, "obiectNumber"), obiectNumber);
		matNormal = glm::mat3(transpose(inverse(matObj)));
		glUniformMatrix3fv(glGetUniformLocation(_idProgram, "matNormal"), 1, GL_FALSE, &matNormal[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(_idProgram, "matObj"), 1, GL_FALSE, &matObj[0][0]);
		glUniform1i(glGetUniformLocation(_idProgram, "Texture"), 0);
		// matObj = glm::translate(matObj, glm::vec3(tmp));

		// Rysowanie
		glBindVertexArray(idVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glDrawArraysInstanced(GL_TRIANGLES, 0, vertices.size(), NUMBER_OF_INSTANCES);
		glBindVertexArray(0);

		// Reset
		matObj = glm::mat4x4(1.0);
		matNormal = glm::mat4x4(1.0);
	}

	void DrawRandom(GLuint _idProgram, float _angle = 0.0, glm::vec3 _rotate = glm::vec3(0.0f))
	{
		for (int i = 0; i < NUMBER_OF_INSTANCES; i++)
		{
			float x = multiPos[i].x;
			float y = multiPos[i].y;
			float z = multiPos[i].z;
			scale = multiPos[i].w;

			if (_angle != 0.0)
				this->Rotate(_angle, _rotate);
			this->Translate(glm::vec3(x, y, z));
			this->Scale(glm::vec3(scale, scale, scale));
			this->Draw(_idProgram);
		}
	}

	// ------------------------ TRANSLATE ------------------------
	void SetRandomPositions(int _number_of_instances, float _xpos_max, float _zpos_max, C_Mesh &ground, float _moveY = 0.0f)
	{
		NUMBER_OF_INSTANCES = _number_of_instances;
		// srand(time(NULL));

		float Min_x = -_xpos_max;
		float Max_x = _xpos_max;
		float Min_z = -_zpos_max;
		float Max_z = _zpos_max;
		float Min_scale = 0.8;
		float Max_scale = 1.6;
		float y;

		for (int i = 0; i < NUMBER_OF_INSTANCES; i++)
		{
			float x = Min_x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Max_x - Min_x)));
			float z = Min_z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Max_z - Min_z)));
			scale = Min_scale + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Max_scale - Min_scale)));

			if (_moveY != 0.0)
				y = ground.getY(glm::vec2(x, z)) - _moveY;
			else
				y = ground.getY(glm::vec2(x, z));

			multiPos.push_back(glm::vec4(x, y, z, scale));
		}
	}

	void SetRandomPosition(C_Mesh &ground)
	{
		float Min_x = -70;
		float Max_x = 70;
		float Min_z = -70;
		float Max_z = 70;
		float x = 0.0;
		while ((x < 13.0 && x > -13.0))
		{
			printf("############## Czekaj... ##############\n");
			x = Min_x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Max_x - Min_x)));
		}

		// float x = Min_x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Max_x - Min_x)));
		float z = Min_z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (Max_z - Min_z)));
		float y = ground.getY(glm::vec2(x, z));

		Position = glm::vec3(x, y, z);
		matObj = glm::translate(matObj, Position);
		// printf("---------- %f %f %f \n", Position.x, Position.x, Position.z)
	}

	void Translate(glm::vec3 _vec)
	{
		Position = _vec;
		matObj = glm::translate(matObj, _vec);
	}

	void Scale(glm::vec3 _vec)
	{
		matObj = glm::scale(matObj, _vec);
	}

	void Rotate(GLfloat _angle, glm::vec3 _vec)
	{
		matObj = glm::rotate(matObj, glm::radians(_angle), _vec);
	}

	// void RotateByMatrix(glm::mat4x4 Matrix_rotate, glm::vec3 _vec)
	// void RotateByMatrix(glm::mat3x3 Matrix_rotate, GLfloat distance)
	void RotateByMatrix(glm::mat4x4 Matrix_rotate, GLfloat distance)
	{
		// matObj = glm::translate(matObj, glm::vec3(Matrix_rotate * matObj * glm::vec4(_vec, 1.0f)));
		// matObj = glm::translate(matObj, glm::vec3(Matrix_rotate * glm::vec3(distance, distance, distance)));
		// matObj = glm::translate(matObj, glm::vec3(Matrix_rotate * glm::vec3(0.0f, 0.0f, 0.0f)));
		// matObj = matObj * Matrix_rotate;
		matObj = glm::translate(matObj, glm::vec3(Matrix_rotate * matObj * glm::vec4(distance, 0.0f, distance, 1.0f)));
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

	MaterialParam GetMaterial()
	{
		return myMaterial;
	}

	std::vector<glm::vec3> getVertices()
	{
		return vertices;
	}

	std::vector<glm::vec3> getNormals()
	{
		return normals;
	}

	GLint getId()
	{
		return idVAO;
	}

	GLuint getObiectNumber()
	{
		return obiectNumber;
	}

	// ------------------------ GROUND ------------------------
	// stworzenie struktury z listy trojkatow
	// na przyklad przy okazji ladowania z pliku OBJ
	void SetAsGround()
	{
		glm::vec3 p1, p2, p3;

		std::vector<glm::vec3>::iterator it = vertices.begin();
		while (it != vertices.end())
		{
			p1 = *it++;
			p2 = *it++;
			p3 = *it++;

			// utworzenie nowego obiektu CTriangle
			triangles.push_back(CTriangle(p1, p2, p3));
		}

		printf("Utworzono CGround zawierajacy %d trojkatow.\n", triangles.size());
	}

	// Glowna funkcja obliczajaca wysokosci nad podlozem w punkcie X Z
	// - przeszukanie po wszystkich trojkatach
	// - gdy zostanie znaleziony trojkat, obliczana jest wysokosc Y
	float getY(glm::vec2 point)
	{
		// obliczenie listy trojkatow nad ktorymi jestesmy
		CTriangle *collTriangle = NULL;

		for (auto it = triangles.begin(); it != triangles.end(); it++)
		{
			if ((*it).isInside(point))
			{
				collTriangle = &(*it);
				break;
			}
		}

		// jesli zostal znaleziony
		if (collTriangle)
		{
			return collTriangle->calulateY(point);
		}
		else
		{
			// printf("Brak ziemi pod nogami!\n");
			return 0.0;
		}
	}

	bool checkIfOnGround(glm::vec2 point)
	{
		CTriangle *collTriangle = NULL;

		for (auto it = triangles.begin(); it != triangles.end(); it++)
		{
			if ((*it).isInside(point))
			{
				collTriangle = &(*it);
				break;
			}
		}

		return collTriangle;
	}

	// ------------------------ PLAYER ------------------------
	void SetAsPlayer(glm::vec3 _position, glm::vec3 _direction)
	{
		StartPosition = _position;
		Position = _position;
		StartDirection = _direction;
		Direction = _direction;
		// Direction = glm::vec3(0.0, 1.0, 1.0);
		// UpdateMatModel();
	}

	// Obliczenie wysokosci nad ziemia
	void UpdateY(float _y)
	{
		Position.y = _y;
		UpdateMatModel();
	}

	// aktualizacja macierzy modelu
	void UpdateMatModel()
	{
		matObj = glm::translate(glm::mat4(1.0), Position);
		matObj = glm::rotate(matObj, Angle, glm::vec3(0.0, 1.0, 0.0));
		canMove = true;
		// GAMEOVER = false;
	}

	// ustawienie pozycji na scenie
	void SetPosition(glm::vec3 pos)
	{
		Position = pos;
		UpdateMatModel();
	}

	void Rotate(float angle)
	{
		Angle += angle;
		Direction.x = cos(Angle);
		Direction.z = -sin(Angle);
		// printf("Rotate Player () %f %f \n", angle, Angle);

		// aktualizacja
		UpdateMatModel();
	}

	float getAngle()
	{
		return Angle;
	}

	glm::vec3 getPosition()
	{
		return Position;
	}

	float getPosX()
	{
		return Position.x;
	}

	float getPosY()
	{
		return Position.y;
	}

	float getPosZ()
	{
		return Position.z;
	}

	// ------------------------ COLLISION ------------------------
	void MoveForward()
	{
		glm::vec3 OldPosition = Position;
		Position += Direction * nextMoveSpeed;
		if (canMove && checkIfOnGround(glm::vec2(Position.x, Position.z)))
		{
			// Position += Direction * nextMoveSpeed;
		}
		else
		{
			Position = OldPosition;
		}
		UpdateMatModel();
	}

	void MoveBackward()
	{
		Position += Direction * (-nextMoveSpeed);
		UpdateMatModel();
	}

	void CheckCollisionObj(const C_Mesh &_other)
	{
		glm::vec3 OldPosition = Position;
		Position += Direction * nextMoveSpeed;

		if (this->isCollision(_other))
		{
			canMove = false;
			printf("Kolizja sfer (%f, %f, %f) i (%f, %f, %f) \n",
				   Position.x, Position.y, Position.z, _other.Position.x, _other.Position.y, _other.Position.z);
		}
		Position = OldPosition;
	}

	void CheckCollisionObj_GAMEOVER(const C_Mesh &_other)
	{
		glm::vec3 OldPosition = Position;
		Position += Direction * nextMoveSpeed;

		if (this->isCollision(_other))
		{
			GAMEOVER = true;
		}
		else
		{
			Position = OldPosition;
		}
	}

	void CheckCollisionObj_WIN(const C_Mesh &_other)
	{
		glm::vec3 OldPosition = Position;
		Position += Direction * nextMoveSpeed;

		if (this->isCollision(_other))
		{
			GAME_WIN = true;
		}
		else
		{
			Position = OldPosition;
		}
	}

	void Reset()
	{
		Position = StartPosition;
		UpdateMatModel();
	}

	void CheckCollisionVec3(std::vector<glm::vec3> Positions, float _radius)
	{
		glm::vec3 OldPosition = Position;
		Position += Direction * nextMoveSpeed;

		for (int i = 0; i < Positions.size(); i++)
		{
			float distance = glm::distance(this->Position, Positions[i]);
			if (distance < this->radius + _radius)
			{
				canMove = false;
				printf("Kolizja sfer RANDOM\n");
				break;
			}
		}
		Position = OldPosition;
	}

	void CheckCollisionVec3_GAMEOVER(std::vector<glm::vec3> Positions, float _radius)
	{
		glm::vec3 OldPosition = Position;
		Position += Direction * nextMoveSpeed;

		for (int i = 0; i < Positions.size(); i++)
		{
			float distance = glm::distance(this->Position, Positions[i]);
			if (distance < this->radius + _radius)
			{
				GAMEOVER = true;

				break;
			}
			else
			{
				Position = OldPosition;
			}
		}
	}

	bool isCollision(const C_Mesh &_other)
	{
		float distance = glm::distance(this->Position, _other.Position);
		if (distance < this->radius + _other.radius)
		{
			return true;
		}
		return false;
	}
};

#endif