#include "WindowManagement.h"

WindowManagement::WindowManagement()
{
}

WindowManagement::~WindowManagement()
{
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

int WindowManagement::displayMode;
int WindowManagement::controlMode;
Scene WindowManagement::myScene;

bool WindowManagement::init(int w, int h) {
	// glfw init
	GLuint err = glfwInit();
	if (err != GLFW_TRUE) {
		cout << err << endl;
		getchar();
		return false;
	}
	// window init
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	_var::width = w; _var::height = h;
	window = glfwCreateWindow(_var::width, _var::height, "Physics Engine simulation", NULL, NULL);
	glfwMakeContextCurrent(window);
	// glew init
	glewExperimental = GL_TRUE;
	err = glewInit();
	if (err != GLEW_OK) {
		cout << glewGetErrorString(err) << endl;
		getchar();
		return false;
	}

	draw::init();
	_var::init();

	myScene.init();


	displayMode = DISPLAY_MODE4;
	controlMode = CONTROL_EYE;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return (window == NULL ? false : true);
}

void WindowManagement::setCallbackFunction() {
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_cursor_callback);
}

void WindowManagement::mainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glfwGetFramebufferSize(window, &_var::width, &_var::height);
		glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		display();

		glfwSwapInterval(1);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}

void WindowManagement::error_callback(int error, const char * description)
{
	cout << "Error: " << description << endl;
}

void WindowManagement::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_RELEASE) return;
	switch (controlMode) {
	case CONTROL_EYE:
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_W:
			_var::myEye.pos -= glm::vec3(1.f, 0.f, 0.f);
			break;
		case GLFW_KEY_A:
			_var::myEye.pos += glm::vec3(0.f, 0.f, 1.f);
			break;
		case GLFW_KEY_S:
			_var::myEye.pos += glm::vec3(1.f, 0.f, 0.f);
			break;
		case GLFW_KEY_D:
			_var::myEye.pos -= glm::vec3(0.f, 0.f, 1.f);
			break;
		case GLFW_KEY_Q:
			_var::myEye.pos += glm::vec3(0.f, 1.f, 0.f);
			break;
		case GLFW_KEY_E:
			_var::myEye.pos -= glm::vec3(0.f, 1.f, 0.f);
			break;
		case GLFW_KEY_R:
			myScene.reset();
			break;
		case GLFW_KEY_SPACE:
			myScene.shoot();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void WindowManagement::mouse_cursor_callback(GLFWwindow *window, double xpos, double ypos) {
	static int label = 0;
	static double last_xpos, last_ypos;
	if (label == 0) {
		last_xpos = xpos;
		last_ypos = ypos;
		label = 1;
	}
	else {
		_var::myEye.eye_theta += (last_xpos - xpos) / 5;
		_var::myEye.eye_fi += (last_ypos - ypos) / 5;
		
		last_xpos = xpos;
		last_ypos = ypos;
	}
}

void WindowManagement::changeDisplayMode() {
	switch (displayMode) {
	case DISPLAY_MODE0:
		displayMode = DISPLAY_MODE1;
		break;
	case DISPLAY_MODE1:
		displayMode = DISPLAY_MODE2;
		break;
	case DISPLAY_MODE2:
		displayMode = DISPLAY_MODE3;
		break;
	case DISPLAY_MODE3:
		displayMode = DISPLAY_MODE4;
		break;
	case DISPLAY_MODE4:
		displayMode = DISPLAY_MODE0;
		break;
	default:
		break;
	}
}

void WindowManagement::display() {
	float wRation, hRation;
	int width = _var::width, height = _var::height;
	if (width > height) {
		wRation = 1;
		hRation = (float)height / width;
	}
	else {
		hRation = 1;
		wRation = (float)width / height;
	}

	calViewProjcetMatrix();	// Cal View Projection matrix and update eye position

	switch (displayMode) {
	case DISPLAY_MODE0:
		// LB window
		glViewport(0, 0, width / 2, height / 2);
		_var::myViewMatrix = glm::lookAt(glm::vec3(terrainWidth, 0.f, 0.f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		_var::myProjectionMatrix = glm::ortho(-terrainWidth / 2 * wRation, terrainWidth / 2 * wRation, 
												-100 * hRation, 100 * hRation, 0.f, 2 * terrainHeight);
		myScene.draw();
		// RB window
		glViewport(width / 2, 0, width / 2, height / 2);
		_var::myViewMatrix = glm::lookAt(glm::vec3(0, 100.f, 0.f), glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0));
		_var::myProjectionMatrix = glm::ortho(-terrainWidth * wRation, terrainWidth * wRation, 
												-terrainHeight * hRation, terrainHeight * hRation, 0.f, 100 + terrainDepth);
		myScene.draw();
		// LT window
		glViewport(0, height / 2, width / 2, height / 2);
		_var::myViewMatrix = glm::lookAt(glm::vec3(0, 0, terrainHeight), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		_var::myProjectionMatrix = glm::ortho(-terrainWidth / 2 * wRation, terrainWidth / 2 * wRation,
												-100 * hRation, 100 * hRation, 0.f, 2 * terrainHeight);
		myScene.draw();
		// RT window
		glViewport(width / 2, height / 2, width / 2, height / 2);
		calViewProjcetMatrix();
		myScene.draw();

		break;
	case DISPLAY_MODE1:
		glViewport(0, 0, width, height);
		_var::myViewMatrix = glm::lookAt(glm::vec3(terrainWidth, 0.f, 0.f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		_var::myProjectionMatrix = glm::ortho(-terrainWidth / 2 * wRation, terrainWidth / 2 * wRation, 
												-100 * hRation, 100 * hRation, 0.f, 2 * terrainHeight);
		myScene.draw();
		break;
	case DISPLAY_MODE2:
		glViewport(0, 0, width, height);
		_var::myViewMatrix = glm::lookAt(glm::vec3(0, 100.f, 0.f), glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0));
		_var::myProjectionMatrix = glm::ortho(-terrainWidth * wRation, terrainWidth * wRation, 
												-terrainHeight * hRation, terrainHeight * hRation, 0.f, 100 + terrainDepth);
		myScene.draw();
		break;
	case DISPLAY_MODE3:
		glViewport(0, 0, width, height);
		_var::myViewMatrix = glm::lookAt(glm::vec3(0, 0, terrainHeight), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		_var::myProjectionMatrix = glm::ortho(-terrainWidth / 2 * wRation, terrainWidth / 2 * wRation, 
												-100 * hRation, 100 * hRation, 0.f, 2 * terrainHeight);
		myScene.draw();
		break;
	case DISPLAY_MODE4:
		glViewport(0, 0, width, height);
		calViewProjcetMatrix();
		myScene.draw();
		break;
	default:
		break;
	}
}

void WindowManagement::calViewProjcetMatrix() {
	glm::mat4 transform = glm::mat4(1.f);
	glm::vec4 eye_position = glm::vec4(0.f, 0.f, 0.f, 1.f);

	transform = glm::translate(transform, _var::myEye.pos);
	transform = glm::rotate(transform, glm::radians(_var::myEye.eye_theta), glm::vec3(0.f, 1.f, 0.f));
	transform = glm::rotate(transform, glm::radians(_var::myEye.eye_fi), glm::vec3(0.f, 0.f, 1.f));
	transform = glm::translate(transform, glm::vec3(_var::myEye.d, 0.f, 0.f));
	_var::myEye.lookPos = transform * eye_position;

	_var::myViewMatrix = glm::lookAt(_var::myEye.pos, _var::myEye.lookPos, glm::vec3(0, 1, 0));
	_var::myProjectionMatrix = glm::perspective(glm::radians(_var::myEye.fovy), (float)_var::width / (float)_var::height, 
												viewVolumnNear, viewVolumnFar);
}