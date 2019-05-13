#include "Light.h"



Light::Light()
{
	myLight[POINT_LIGHT].ambient = glm::vec3(0.95f, 0.95f, 0.95f);
	myLight[POINT_LIGHT].diffuse = glm::vec3(0.95f, 0.95f, 0.95f);
	myLight[POINT_LIGHT].specular = glm::vec3(0.f, 0.f, 0.f);
	myLight[POINT_LIGHT].position = glm::vec3(30.f, 0.f, 0.f);
	myLight[POINT_LIGHT].direction = glm::vec3(0.f, 0.f, 0.f);
	myLight[POINT_LIGHT].light_theta = 0.f;
	myLight[POINT_LIGHT].light_fi = 0.f;
	myLight[POINT_LIGHT].exponent = 8.f;
	myLight[POINT_LIGHT].cutoff = 180.f;
	myLight[POINT_LIGHT].mode = LIGHT_ON;
}


Light::~Light()
{
}

light Light::myLight[3];

void Light::paint(shader s) {
	glUniform3fv(glGetUniformLocation(s.shaderProgram, "myPointLight.ambient"), 1, glm::value_ptr(myLight[POINT_LIGHT].ambient));
	glUniform3fv(glGetUniformLocation(s.shaderProgram, "myPointLight.diffuse"), 1, glm::value_ptr(myLight[POINT_LIGHT].diffuse));
	glUniform3fv(glGetUniformLocation(s.shaderProgram, "myPointLight.specular"), 1, glm::value_ptr(myLight[POINT_LIGHT].specular));
	glUniform3fv(glGetUniformLocation(s.shaderProgram, "myPointLight.position"), 1, glm::value_ptr(myLight[POINT_LIGHT].position));
	glUniform1f(glGetUniformLocation(s.shaderProgram, "myPointLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(s.shaderProgram, "myPointLight.linear"), 0.001f);

	glUniform3fv(glGetUniformLocation(s.shaderProgram, "eyePos"), 1, glm::value_ptr(_var::myEye.pos));
}