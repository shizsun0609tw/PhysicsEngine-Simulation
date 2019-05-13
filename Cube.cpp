#include "Cube.h"
#include "Bullet.h"


Cube::Cube()
{
}

Cube::Cube(glm::vec3 pos, glm::vec3 size, float mass) {
	this->mass = mass;
	this->pos = pos;
	this->size = size;
	angle = glm::vec3(0.f);
	velocity = glm::vec3(0.f);
	acceleration = glm::vec3(0.f);
	t = chrono::steady_clock::now();
	rotateMtx = glm::mat4(1.f);
	angularAcceleration = glm::vec3(0.f);
	angularVelocity = glm::vec3(0.f);
}


Cube::~Cube()
{
}

glm::vec3 Cube::getPos() {
	return pos;
}

glm::vec3 Cube::getSize() {
	return size;
}

glm::vec3 Cube::getVelocity() {
	return velocity;
}

float Cube::getMass() {
	return mass;
}

glm::mat4 Cube::getRotateMtx() {
	return rotateMtx;
}

void Cube::update() {
	chrono::steady_clock::time_point now = chrono::steady_clock::now();
	float passTime = PhysicsEngine::calPassTimePerFrame(t, now);
	t = now;

	glm::vec3 drag = PhysicsEngine::calForceByDrag(0.8, velocity, 0.8f, 1.f);
	
	acceleration = PhysicsEngine::calAccelerationByNewton2Law(drag, mass);
	velocity = PhysicsEngine::calVelocity(velocity, acceleration, passTime);
	if (glm::length(velocity) < 0.3) velocity = glm::vec3(0.f);
	pos = PhysicsEngine::calPosition(pos, velocity, passTime);
	
	glm::vec3 angularDrag = PhysicsEngine::calForceByDrag(0.8, angularVelocity, 0.8f, 1.f);
	angularAcceleration = PhysicsEngine::calAccelerationByNewton2Law(angularDrag, mass);
	angularVelocity = PhysicsEngine::calAngularVelocity(angularVelocity, angularAcceleration, passTime);
	rotateMtx = PhysicsEngine::calRotateMtx(rotateMtx, angle, PhysicsEngine::calAngle(angle, angularVelocity, passTime));
	angle = PhysicsEngine::calAngle(angle, angularVelocity, passTime);
	if (glm::length(angularVelocity) < 0.01) angularVelocity = glm::vec3(0.f);
	
	angularAcceleration = glm::vec3(0.f);
}

void Cube::onCollision(Bullet object) {
	//velocity = PhysicsEngine::calVelocityByElasticCollision(mass, velocity, object.getMass(), object.getVelocity());
	
	chrono::steady_clock::time_point now = chrono::steady_clock::now();
	float passTime = PhysicsEngine::calPassTimePerFrame(t, now);

	glm::vec3 transient = object.getVelocity() / passTime;
	glm::vec3 addForce = PhysicsEngine::calForceByNewton2Law(mass, transient);
	
	glm::vec3 arm = pos - object.getPos();
	float theta;

	if (glm::length(addForce) == 0) theta = 0;
	else theta = glm::acos(glm::dot(addForce, arm) / (glm::length(addForce) * glm::length(arm)));

	// for move
	glm::vec3 moveForce = addForce * glm::cos(theta);
	glm::vec3 a = PhysicsEngine::calAccelerationByNewton2Law(moveForce, mass);
	velocity = PhysicsEngine::calVelocity(velocity, a, passTime);

	// for rotate
	glm::vec3 rotateForce = addForce * glm::sin(theta);
	glm::vec3 torque = glm::cross(-arm, rotateForce);
	angularAcceleration = glm::inverse(PhysicsEngine::calInertia(mass, size)) * torque;
	angularVelocity = PhysicsEngine::calAngularVelocity(angularVelocity, angularAcceleration, passTime);
}
