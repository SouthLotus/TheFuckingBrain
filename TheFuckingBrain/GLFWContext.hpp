#ifndef __glfwcontext
#define __glfwcontext
#include <string>
#include <memory>
#include "model\TexturedCube.hpp"
#include <GLFW\glfw3.h>
#include "object\World.hpp"
class GLFWContext
{
private:
	//GLFWContextOwner
	static class GLFWContextOwner {
	private:
		bool flag = false;
	public: 
		GLFWContextOwner() = default;
		GLFWContextOwner(bool flag);
		GLFWContextOwner & operator=(GLFWContextOwner &another);
		~GLFWContextOwner();
	};

	//GLFWWindowOwner
	static class GLFWWindowOwner {
	private:
		bool flag = false;
	public:
		GLFWwindow *window;
		int width;
		int height;
		GLFWWindowOwner() = default;
		GLFWWindowOwner(
			int width, int height,
			std::string title,
			GLFWmonitor *monitor, GLFWwindow *share);
		GLFWWindowOwner & operator=(GLFWWindowOwner &another);
		~GLFWWindowOwner();
	};

	GLFWWindowOwner windowOwner;
	GLFWContextOwner contextOwner;
	//World
	World world;
	void init();
	void display();
	void dispose();
	void initContext(std::string title);
	static GLFWContext *pointToMe;
public:
	GLFWContext(std::string title);
	~GLFWContext();
	void run();
	static void errorCallback(int code, const char *des);
	static void framebufferSizeCallback(
		GLFWwindow *window, int width, int height);
	static void keyCallback(
		GLFWwindow *window, int key, int scan, int action, int mod);
	static void cursorPosCallback(GLFWwindow *window,
		double posx, double posy);
};
#endif

