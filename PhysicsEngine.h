#pragma once
#include "Definition.h"
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <iostream>

class PhysicsEngine
{
public:
	PhysicsEngine();
	~PhysicsEngine();
	static float calPassTimePerFrame(chrono::steady_clock::time_point t0, chrono::steady_clock::time_point t1);
	static glm::vec3 calAccelerationByNewton2Law(glm::vec3 F, float m);
	static glm::vec3 calForceByNewton2Law(float m, glm::vec3 a);
	static glm::vec3 calVelocity(glm::vec3 v0, glm::vec3 a,float t);
	static glm::vec3 calPosition(glm::vec3 p0, glm::vec3 v, float t);
	static glm::vec3 calVelocityByElasticCollision(float m1, glm::vec3 v1, float m2, glm::vec3 v2);
	static glm::vec3 calForceByDrag(float p, glm::vec3 v, float Cd, float A);
	static glm::vec3 calAngularAcceleration();
	static glm::vec3 calAngularVelocity(glm::vec3 v, glm::vec3 a, float t);
	static glm::vec3 calAngle(glm::vec3 angle, glm::vec3 v, float t);
	static glm::mat4 calRotateMtx(glm::mat4 rotateMtx, glm::vec3 anglePast, glm::vec3 angleCurrent);
	static glm::mat3 calInertia(float mass, glm::vec3 arm);
	static glm::vec3 Sign(glm::vec3 v);
};

