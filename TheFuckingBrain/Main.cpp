
#include <iostream>
#include <exception>
#include <memory>
#include <vector>
#include "GLFWContext.hpp"
#include <thread>
#include <chrono>
#include <stb\stb_image.h>
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>

int main(int arg, char **args) {
	try {
		GLFWContext context("The Fucking Brain");
		context.run();
	}
	catch (std::exception exp) { 
		std::cout << exp.what() << std::endl;
	}
	return 0;
}