#pragma once
#include "Definition.h"
#include "PhysicsEngine.h"
#include <chrono>

class Bullet;

class Cube
{
public:
	Cube();
	Cube(glm::vec3 pos, glm::vec3 size, float mass);
	~Cube();
	glm::vec3 getPos();
	glm::vec3 getSize();
	glm::vec3 getVelocity();
	glm::mat4 getRotateMtx();
	float getMass();
	void update();
	void onCollision(Bullet object);
private:
	float mass;

	glm::vec3 pos;
	glm::vec3 size;
	glm::vec3 angle;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 angularVelocity;
	glm::vec3 angularAcceleration;
	glm::mat4 rotateMtx;

	chrono::steady_clock::time_point t;
};

