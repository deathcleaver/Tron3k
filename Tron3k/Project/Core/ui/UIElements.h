#pragma once
#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include "../../../RenderPipeline/IRenderPipeline.h"

#include "uiVertex.h"

#include <string>
#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class UIElements
{
public:
	virtual ~UIElements() {};

	virtual void render(int id) = 0;

	virtual void setWorldMatrix(float x, float y, int id) = 0;

	virtual void changeTexUsed(int id) = 0;

	virtual void fromPosToQuadScreen(glm::vec2 positions, int id) = 0;

	virtual int checkCollision(glm::vec2 pos) = 0;

	virtual void hoverCheck(glm::vec2 pos) = 0;

	virtual void setTexture(std::vector<GLuint> uiTextureIds) = 0;

	//virtual uiVertex* returnPosAUv(int id) = 0;
};

#endif