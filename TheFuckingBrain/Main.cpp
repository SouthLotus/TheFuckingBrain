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

class B {
public:
	B(int x, int y) {
		//complex thing....
	}
	~B() = default;
};

class A {
private:
	B b;
public:
	A(): b(0,0) {
	}
	~A() = default;
};

/*class Monster {
public:
	Monster() = default;
	virtual ~Monster() = 0;
	virtual void hpDown() = 0;
};

class JeffKiller : public Monster {
public:
	JeffKiller() = default;
	~JeffKiller() = default;
	void hpDown() {

	}
};

class HellWolf: public Monster {
public:
	HellWolf() = default;
	~HellWolf() = default;
	void hpDown() {

	}
};

class Warrior {
public:
	Warrior() = default;
	Warrior() = default;
	bool dragonPulse(Monster &monster) {

	}
};*/

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