#ifndef LIGHTS_H
#define LIGHTS_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

struct SpotLight
{
	glm::vec3 Color;
	float DiffuseIntensity;
	glm::vec3 Position;
	float AmbientIntensity;
	glm::vec3 Direction;
	float Cutoff;

	SpotLight()
	{
		Color = glm::vec3(1.0f, 1.0f, 1.0f); // 0 1 2
		Position = glm::vec3(0.0f, 0.0f, 0.0f); // 3 4 5
		Direction = glm::vec3(0.0f, -1.0f, 0.0f); // 6 7 8

		AmbientIntensity = 0.0f; // 9
		DiffuseIntensity = 0.7f; // 10
		Cutoff = 0.9f; //14
	}
};

/*
struct SpotLightH
{
int roomID;
float r, g, b;
float intensity;
float x, y, z;
float ambientIntensity;
float dx, dy, dz;
float coneAngle;
float ax, ay, az, aw;
};
*/


#endif