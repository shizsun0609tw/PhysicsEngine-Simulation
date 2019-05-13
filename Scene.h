#pragma once

#include "Definition.h"
#include "shader.h"
#include "_var.h"
#include "draw.h"
#include "Light.h"
#include "Bullet.h"
#include "Cube.h"
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include <vector>

class Scene
{
public:
	Scene();
	~Scene();
	void init();
	void draw();
	void shoot();
	void reset();
private:
	void collisionDetection();
	stack<glm::mat4> modelMatrix;
	shader myShader;
	static Light myLight;
	static vector<Cube> myCubes;
	static vector<Bullet> myBullets;
};

