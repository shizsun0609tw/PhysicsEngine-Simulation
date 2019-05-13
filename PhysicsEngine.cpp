#include "PhysicsEngine.h"



PhysicsEngine::PhysicsEngine()
{
}


PhysicsEngine::~PhysicsEngine()
{
}

float PhysicsEngine::calPassTimePerFrame(chrono::steady_clock::time_point t0, chrono::steady_clock::time_point t1) {
	return chrono::duration_cast<chrono::milliseconds>(t1 - t0).count() / 1000.f;
}

glm::vec3 PhysicsEngine::calAccelerationByNewton2Law(glm::vec3 F, float m) {
	// F = ma
	return F / m;
}

glm::vec3 PhysicsEngine::calForceByNewton2Law(float m, glm::vec3 a) {
	return m * a;
}

glm::vec3 PhysicsEngine::calVelocity(glm::vec3 v0, glm::vec3 a, float t) {
	return v0 + a * t;
}

glm::vec3 PhysicsEngine::calVelocityByElasticCollision(float m1, glm::vec3 v1, float m2, glm::vec3 v2) {
	// m1 * v1 + m2 * v2 = m1 * v1' + m2 * v2'
	// 1/2 m1v1^2 + 1/2 m2v2^2 = 1/2 m1v1'^2 + 1/2 m2v2'^2
	// v1'= (v1(m1 - m2) + 2 m2v2) / (m1 + m2) 
	return (v1 * (m1 - m2) + 2 * m2 * v2) / (m1 + m2);  
}

glm::vec3 PhysicsEngine::calPosition(glm::vec3 pos, glm::vec3 v, float t) {
	return pos + v * t;
}

glm::vec3 PhysicsEngine::calForceByDrag(float p, glm::vec3 v, float Cd, float A) {
	return (glm::length(v) < 1e-10 ? glm::vec3(0.f) : -0.5f * p * glm::length(v) * glm::length(v) * Cd * A * glm::normalize(v));
}

glm::vec3 PhysicsEngine::Sign(glm::vec3 v) {
	return glm::vec3(v.x > 0 ? 1 : -1, v.y > 0 ? 1 : -1, v.z > 0 ? 1 : -1);
}

glm::vec3 PhysicsEngine::calAngle(glm::vec3 angle, glm::vec3 v, float t) {
	glm::vec3 rev = angle + v * t;
	rev.x = fmod(rev.x, 360.f);
	rev.y = fmod(rev.y, 360.f);
	rev.z = fmod(rev.z, 360.f);

	return rev;
}

glm::vec3 PhysicsEngine::calAngularAcceleration() {
	return glm::vec3();
}

glm::vec3 PhysicsEngine::calAngularVelocity(glm::vec3 v, glm::vec3 a, float t) {
	return v + a * t;
}

glm::mat4 PhysicsEngine::calRotateMtx(glm::mat4 rotateMtx, glm::vec3 anglePass, glm::vec3 angleCurrent){
	glm::vec3 angle = angleCurrent - anglePass;
	angle.x = fmod(angle.x + 2 * PI, 2 * PI);
	angle.y = fmod(angle.y + 2 * PI, 2 * PI);
	angle.z = fmod(angle.z + 2 * PI, 2 * PI);

	float i = 0.f;
	while(i < angle.x) {
		rotateMtx = glm::rotate(rotateMtx, float(PI / 180.f), glm::vec3(1.f, 0.f, 0.f));
		i += PI / 180.f;
	}

	i = 0;
	while(i < angle.y){
		rotateMtx = glm::rotate(rotateMtx, float(PI / 180.f), glm::vec3(0.f, 1.f, 0.f));
		i += PI / 180.f;
	}

	i = 0;
	while(i < angle.z) {
		rotateMtx = glm::rotate(rotateMtx, float(PI / 180.f), glm::vec3(0.f, 0.f, 1.f));
		i += PI / 180.f;
	}
	return rotateMtx;
}

glm::mat3 PhysicsEngine::calInertia(float mass, glm::vec3 arm) {
	glm::mat3 inertia(0.f);

	inertia[0][0] = (arm.y * arm.y + arm.z * arm.z) * mass;
	inertia[1][1] = (arm.x * arm.x + arm.z * arm.z) * mass;
	inertia[2][2] = (arm.x * arm.x + arm.y * arm.y) * mass;

	return inertia;
} 