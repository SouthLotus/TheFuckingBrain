#include <iostream>
#include <exception>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <glm\glm.hpp>
#include <cstdio>
#include <map>
#include "GLFWContext.hpp"
#include "tool\MathTool.hpp"
#include "tool\SkeletalAnimParser.hpp"


class A {
	private:
		Assimp::Importer imp;
};

void c(Assimp::Importer &a) {
	Assimp::Importer b;
	a = b;
	
}
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