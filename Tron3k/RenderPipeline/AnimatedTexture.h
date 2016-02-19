#ifndef ANIMATED_TEXTURE_H
#define ANIMATED_TEXTURE_H

#include "Utils\GPUMemoryLeakChecker.h"
#include <gl/GL.h>
#include <glm\glm.hpp>
#include <vector>

using namespace glm;

struct AnimatedObject
{
	glm::mat4 worldMat;
	int roomID;
	unsigned int textureID;

	int type;

	vec2 offsets;
	float timer;
	
	float freezeTimer;
	float swapTimer;

	bool freezeMode = true;

	float segments;

	int currentSegment = 0;

	void init_standing_right(vec3 topRight, vec3 topLeft)
	{
		worldMat[0] = glm::vec4(topRight, 1);
		worldMat[1] = glm::vec4(topRight.x, topLeft.y, topRight.z, 1);
		worldMat[2] = glm::vec4(topLeft.x, topRight.y, topLeft.z, 1);
		worldMat[3] = glm::vec4(topLeft, 1);
	}

	void init_standing_down(vec3 topRight, vec3 topLeft)
	{
		worldMat[0] = glm::vec4(topRight.x, topLeft.y, topRight.z, 1);
		worldMat[1] = glm::vec4(topLeft, 1);
		worldMat[2] = glm::vec4(topRight, 1);
		worldMat[3] = glm::vec4(topLeft.x, topRight.y, topLeft.z, 1);
	}
	
	void init_time_segments(float _segments, float _freezeTime, float _swapTime)
	{
		segments = _segments;
		offsets.x = 0;
		offsets.y = 1.0f / segments;
		freezeTimer = _freezeTime;
		swapTimer = _swapTime;
		timer = freezeTimer;
	}

	void update(float dt)
	{
		if (freezeMode)
		{
			timer -= dt;
			if (timer < 0)
			{
				freezeMode = false;
				timer = swapTimer;
			}
		}

		if (!freezeMode)
		{
			timer -= dt;


			if (timer < 0)
			{
				freezeMode = true;
				timer = freezeTimer;
				currentSegment++;
				if (currentSegment >= segments)
					currentSegment = 0;

				offsets.x = currentSegment / segments;
				offsets.y = (currentSegment + 1) / segments;
				return;
			}

			float scroll = (1.0f - (timer / swapTimer)) / segments;

			offsets.x = (currentSegment / segments) + scroll;
			offsets.y = ((currentSegment + 1.0f) / segments) + scroll;
		}
	}
};

class AnimatedTexture
{
public:

	AnimatedTexture();
	~AnimatedTexture();

	void update(float);
	void render();
	void init();

	GLuint animQuadShader;
	GLuint animQuadUVset;
	GLuint animQuadWorld;
	GLuint animQuadVP;

private:
	std::vector<AnimatedObject> objects;
	bool initialized = false;
};

#endif