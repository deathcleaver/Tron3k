#ifndef ANIMATEDMESH_H
#define ANIMATEDMESH_H

#include <string>
#include <glm\glm.hpp>
#include "Map\MapHeaders.h"
#include <GL/glew.h>

struct animationMatrices
{
	glm::mat4 bMatrix;
};

class AnimatedMesh
{

private:

	int activeAnimation;
	int currentKeyFrame;
	int* matOffsets;
	int* indices;
	AnimVertex* verts;
	int* animationKeyCounts;
	int* animationType;

	GLuint vbo;
	GLuint ibo;
	GLuint vao;

	int indexCount;

	int animationCount;
	int jointCount;

	Animation* animations;

	GLuint matricesBuffer;
	animationMatrices* matrixBufferDataOneKey;

public:

	void init();

	~AnimatedMesh();

	void update(float deltaTime);

	void load(std::string fileName);

	void draw(GLuint uniformKeyMatrixLocation);
};

#endif