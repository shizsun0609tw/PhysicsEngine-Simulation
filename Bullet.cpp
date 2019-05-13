#include "Bullet.h"



Bullet::Bullet()
{
	pos = _var::myEye.pos;
	size = glm::vec3(0.1f, 0.1f, 0.1f);
	mass = 0.005f;
	velocity = glm::normalize(_var::myEye.lookPos - _var::myEye.pos) * 100.f;
	acceleration = glm::vec3(0.f);
	t = chrono::steady_clock::now();
}


Bullet::~Bullet()
{
}

glm::vec3 Bullet::getPos() {
	return pos;
}

glm::vec3 Bullet::getSize() {
	return size;
}

glm::vec3 Bullet::getVelocity() {
	return velocity;
}

float Bullet::getMass() {
	return mass;
}

void Bullet::update() {
	chrono::steady_clock::time_point now = chrono::steady_clock::now();
	float passTime = PhysicsEngine::calPassTimePerFrame(t, now);
	t = now;

	glm::vec3 drag = PhysicsEngine::calForceByDrag(0.8, velocity, 0.5f, 0.001f);
	acceleration = PhysicsEngine::calAccelerationByNewton2Law(drag, mass);
	velocity = PhysicsEngine::calVelocity(velocity, acceleration, passTime);
	if (glm::length(velocity) < 0.3) velocity = glm::vec3(0.f);

	pos = PhysicsEngine::calPosition(pos, velocity, passTime);
}

void Bullet::onCollision(Cube object) {
	velocity = glm::vec3(0.f);
}