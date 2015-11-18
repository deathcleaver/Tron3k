#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include <glm\glm.hpp>
#include "Mesh.h"
#include <GL\glew.h>

struct PlayerObject
{
	GLuint meshID;
	GLuint index;
	GLuint vao;
	int facecount;
	GLuint textureID;
};

class ContentManager
{

private:

	Mesh* meshes;
	PlayerObject* playerModels;

	Mesh tempMesh;

public:

	void init();

	void renderChunks(GLuint shader, GLuint shaderLocation);

	void renderPlayer(int playerID, glm::mat4 world);

	void loadChunk();
	void releaseChunk();


};

#endif