#include "Scene.h"



Scene::Scene()
{
}


Scene::~Scene()
{
}

Light Scene::myLight;
vector<Bullet> Scene::myBullets;
vector<Cube> Scene::myCubes;

void Scene::init() {
	myShader = shader("shader.vert", "shader.frag");
	myLight = Light();
	myCubes.push_back(Cube(glm::vec3(0.f), glm::vec3(3.f), 1.f));
	myCubes.push_back(Cube(glm::vec3(0.f, 0.f, 10.f), glm::vec3(2.f, 5.f, 2.f), 1.f));
	myCubes.push_back(Cube(glm::vec3(0.f, 0.f, 20.f), glm::vec3(5.f, 2.f, 2.f), 5.f));
	myCubes.push_back(Cube(glm::vec3(0.f, 0.f, -10.f), glm::vec3(2.f, 5.f, 5.f), 3.f));
	myCubes.push_back(Cube(glm::vec3(0.f, 0.f, -20.f), glm::vec3(5.f, 5.f, 5.f), 10.f));

	glUseProgram(myShader.shaderProgram);
}

void Scene::reset() {
	myCubes.clear();
	myBullets.clear();

	myCubes.push_back(Cube(glm::vec3(0.f), glm::vec3(3.f), 1.f));
	myCubes.push_back(Cube(glm::vec3(0.f, 0.f, 10.f), glm::vec3(2.f, 5.f, 2.f), 1.f));
	myCubes.push_back(Cube(glm::vec3(0.f, 0.f, 20.f), glm::vec3(5.f, 2.f, 2.f), 5.f));
	myCubes.push_back(Cube(glm::vec3(0.f, 0.f, -10.f), glm::vec3(2.f, 5.f, 5.f), 3.f));
	myCubes.push_back(Cube(glm::vec3(0.f, 0.f, -20.f), glm::vec3(5.f, 5.f, 5.f), 10.f));
}

void Scene::draw() {
	glUniformMatrix4fv(glGetUniformLocation(myShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(_var::myViewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(myShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(_var::myProjectionMatrix));
	//glUniform1f(glGetUniformLocation(myShader.shaderProgram, "useTexture"), 0.f);

	// paint light
	myLight.paint(myShader);

	// eye 
	_var::myEye.update(_var::width, _var::height);
	_var::myEye.draw(_var::myViewMatrix, _var::myProjectionMatrix, _var::width, _var::height);


	collisionDetection();

	for (int i = 0; i < myBullets.size(); ++i) {
		myBullets[i].update();
	}
	for (int i = 0; i < myCubes.size(); ++i) {
		myCubes[i].update();
	}


	// bullet
	for (int i = 0; i < myBullets.size(); ++i) {
		modelMatrix.push(glm::mat4(1.f));
		modelMatrix.top() = glm::translate(modelMatrix.top(), myBullets[i].getPos());
		modelMatrix.top() = glm::scale(modelMatrix.top(), myBullets[i].getSize());
		glUniformMatrix4fv(glGetUniformLocation(myShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix.top()));
		glUniform4fv(glGetUniformLocation(myShader.shaderProgram, "color"), 1, glm::value_ptr(blue));
		draw::drawSphere();
		modelMatrix.pop();
	}

	// cube target
	draw::changeMaterial(whitePlastic, myShader);
	for (int i = 0; i < myCubes.size(); ++i) {
		modelMatrix.push(glm::mat4(1.f));
		modelMatrix.top() = glm::translate(modelMatrix.top(), myCubes[i].getPos());
		modelMatrix.top() = modelMatrix.top() * myCubes[i].getRotateMtx();
		modelMatrix.top() = glm::scale(modelMatrix.top(), myCubes[i].getSize());
		glUniformMatrix4fv(glGetUniformLocation(myShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix.top()));
		glUniform4fv(glGetUniformLocation(myShader.shaderProgram, "color"), 1, glm::value_ptr(yellow));
		draw::drawCube();
		modelMatrix.pop();
	}
	

	modelMatrix.empty();
}

void Scene::shoot() {
	myBullets.push_back(Bullet());
}

void Scene::collisionDetection() {
	for (int i = 0; i < myBullets.size(); ++i) {
		for (int j = 0; j < myCubes.size(); ++j) {
			if (((myBullets[i].getPos().x - myCubes[j].getPos().x) * (myBullets[i].getPos().x - myCubes[j].getPos().x) 
				< ((myBullets[i].getSize().x + myCubes[j].getSize().x) / 2) * ((myBullets[i].getSize().x + myCubes[j].getSize().x) / 2))
				&& ((myBullets[i].getPos().y - myCubes[j].getPos().y) * (myBullets[i].getPos().y - myCubes[j].getPos().y)
					< ((myBullets[i].getSize().y + myCubes[j].getSize().y)/2) * ((myBullets[i].getSize().y + myCubes[j].getSize().y)/2))
				&& ((myBullets[i].getPos().z - myCubes[j].getPos().z) * (myBullets[i].getPos().z - myCubes[j].getPos().z)
					< ((myBullets[i].getSize().z + myCubes[j].getSize().z)/2) * ((myBullets[i].getSize().z + myCubes[j].getSize().z)/2))) {
				Bullet temp = myBullets[i];
				myBullets[i].onCollision(myCubes[j]);
				myCubes[j].onCollision(temp);
				cout << "collision" << endl;
			}
		}
	}
}