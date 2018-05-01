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
#include "GLFWContext.hpp"
#include "tool\MathTool.hpp"

int main(int arg, char **args) {
	/*try {
		GLFWContext context("The Fucking Brain");
		context.run();
	}
	catch (std::exception exp) {
		std::cout << exp.what() << std::endl;
	}*/
	Assimp::Importer imp;
	const aiScene *scene = imp.ReadFile(
		"D:/anim.fbx", aiProcess_Triangulate);
	if (scene == nullptr) {
		return 0;
	}
	std::cout << scene->mMeshes[0]->mNumBones;
	return 0;
}