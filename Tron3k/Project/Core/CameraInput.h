#ifndef CAMERAINPUT_H
#define CAMERAINPUT_H

#include <glm\glm.hpp>
#include "Input.h"

#define toDEGREE 57.2957795f
#define toRADIAN 0.0174532925f

using namespace glm;

class CameraInput
{

private:
	static CameraInput* singleton;
	CameraInput();



	Input* i;

	glm::mat4* viewMat;

	vec3 pos;
	vec3 dir;
	vec3 start;

	float angleH;
	float angleV;

	float mouseSpeed = 5.0f; //higher val = slower
	float camSpeed = 10.0f; // higher val = faster

	mat3 rotH;
	mat3 rotV;

	double x_new, y_new;
	double x_last, y_last;

	void mousepan(float x, float y);
	void keypan(float dt);

public:
	static CameraInput* getCam();

	void init(glm::mat4* viewMat);

	void update(float dt, bool freeCam);

	glm::vec3 getDir() { return dir; };
	void setCam(vec3 _pos, vec3 _dir);

	static void release();
};

#endif