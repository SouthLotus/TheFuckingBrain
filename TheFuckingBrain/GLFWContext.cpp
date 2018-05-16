#include <gl/glew.h>
#include <gl/freeglut.h>
#include <exception>
#include <iostream>
#include <stb\stb_image.h>
#include <memory>
#include "GLFWContext.hpp"
#include "shader\BasicShader.hpp"
#include "tool\ModelTool.hpp"
#include "owner\STBImageOwner.hpp"
#include "shader\LowPolyShader.hpp"
#include "shader\Anim1Shader.hpp"

GLFWContext *GLFWContext::pointToMe = nullptr;

void GLFWContext::init()
{
	glClearColor(0, 0, 0, 1);
	Anim1Shader anim = Anim1Shader::create();
	world = std::unique_ptr<World>(new World());
}

void GLFWContext::display()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	world->work();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void GLFWContext::dispose()
{
}

GLFWContext::GLFWContext(std::string title)
{
	pointToMe = this;
	initContext(title);
}

GLFWContext::~GLFWContext()
{
}

void GLFWContext::initContext(std::string title)
{
	/*Su kien khi co loi xay ra trong glfw*/
	glfwSetErrorCallback(errorCallback);
	contextOwner = GLFWContextOwner(true);
	/*Lay video mode cua ma hinh hien tai*/
	const GLFWvidmode *vidMode =
		glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_REFRESH_RATE, vidMode->refreshRate);
	glfwWindowHint(GLFW_RED_BITS, vidMode->redBits);
	glfwWindowHint(GLFW_BLUE_BITS, vidMode->blueBits);
	glfwWindowHint(GLFW_GREEN_BITS, vidMode->greenBits);
	glfwWindowHint(GLFW_SAMPLES, 4);
	int windowWidth = vidMode->width;
	int windowHeight = vidMode->height;
	windowOwner = GLFWWindowOwner(
			windowWidth, windowHeight, title, nullptr, nullptr);
	GLFWwindow *window = windowOwner.window;
	/*Dat icon cho window*/
	GLFWimage icon;
	int iw, ih, ichannels;
	stbi_uc *iData = stbi_load(
		"icon/icon.png", &iw, &ih, &ichannels, STBI_rgb_alpha);
	STBImageOwner stbiCleaner(iData);
	icon.pixels = iData;
	icon.width = iw;
	icon.height = ih;
	glfwSetWindowIcon(window, 1, &icon);
	stbiCleaner.destroy();
	/*Dat context chinh*/
	glfwMakeContextCurrent(window);
	/*Su kien thay doi kich thuoc framebuffer*/
	glfwSetFramebufferSizeCallback(
		window, framebufferSizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	/*An con tro va di chuyen vo han*/
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(1);
	/*Thiet lap glew*/
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("glewInit runtime_error!");
	}
	glewExperimental = true;
}

void GLFWContext::run()
{
	/*Thiet lap OpenGL*/
	init();
	/*Ve do hoa*/
	GLFWwindow *window = windowOwner.window;
	while (!glfwWindowShouldClose(window)) {
		display();
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	/*Giai phong tai nguyen, window, glfw*/
	dispose();
}

void GLFWContext::errorCallback(
	int code, const char * des)
{
}

void GLFWContext::framebufferSizeCallback(
	GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	pointToMe->windowOwner.width = width;
	pointToMe->windowOwner.height = height;
	pointToMe->world->framebufferSizeSignal(width, height);
}

void GLFWContext::keyCallback(
	GLFWwindow * window, int key, int scan, int action, int mod)
{
	pointToMe->world->keySignal(key, scan, action, mod);
}

void GLFWContext::cursorPosCallback(
	GLFWwindow * window, double posx, double posy)
{
	pointToMe->world->cursorPosSignal(posx, posy);
}

//GLFWCongtextOwner
GLFWContext::GLFWContextOwner::GLFWContextOwner(bool flag)
{
	if (!flag || this->flag) {
		return;
	}
	if (glfwInit() == 0) {
		throw std::exception(
			"GLFWContext::GLFWContextOwner::GLFWContextOwner()"
			" exception.");
	}
	this->flag = true;
}

GLFWContext::GLFWContextOwner::~GLFWContextOwner()
{
}

GLFWContext::GLFWContextOwner & GLFWContext::GLFWContextOwner::operator=(
	GLFWContextOwner & another)
{
	if (!another.flag) {
		return *this;
	}
	this->flag = true;
	another.flag = false;
	return *this;
}

//GLFWWindowOwner
GLFWContext::GLFWWindowOwner::GLFWWindowOwner(
	int width, int height,
	std::string title, GLFWmonitor *monitor, GLFWwindow *share
)
{
	flag = true;
	window = glfwCreateWindow(
		width, height, title.c_str(),
		monitor, share);
	if (window == nullptr) {
		throw std::exception(
			"GLFWContext::GLFWWindowOwner::GLFWWindowOwner exception");
	}
	this->width = width;
	this->height = height;
}

GLFWContext::GLFWWindowOwner::~GLFWWindowOwner()
{
	if (flag) {
		std::cout << "Window is destroyed." << std::endl;
		glfwDestroyWindow(window);
	}
}

GLFWContext::GLFWWindowOwner & GLFWContext::GLFWWindowOwner::operator=(
	GLFWWindowOwner & another)
{
	if (!another.flag || this->flag) {
		return *this;
	}
	this->flag = true;
	this->window = another.window;
	another.flag = false;
	return *this;
}

