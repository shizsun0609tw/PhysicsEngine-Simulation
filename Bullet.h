#pragma once
#include "Definition.h"
#include "_var.h"
#include "PhysicsEngine.h"
#include "Cube.h"
#include <chrono>

class Bullet
{
public:
	Bullet();
	~Bullet();
	glm::vec3 getPos();
	glm::vec3 getSize();
	glm::vec3 getVelocity();
	float getMass();
	void update();
	void onCollision(Cube object);
private:
	float mass;

	chrono::steady_clock::time_point t;
	glm::vec3 pos;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 size;
};

