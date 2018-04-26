#include "World.hpp"
#include <glm\glm.hpp>
#include <glfw\glfw3.h>
#include <string>

void World::makeWorld()
{
	//camera
	camera.setLookAt(glm::vec3(5.f, 70.f, 5.f),
		glm::vec3(0.f, 70.f, 10.f));
	camera.setProj(glm::radians(60.f), 1366.f / 768, 0.025f, 3000.f);
	camController.setCamera(&camera);
	//terrain
	terrain.init();
	//cube
	//land.init("model/low.fbx");
	//skybox
	std::string paths[] = {
		"texture/skybox1/right.jpg",
		"texture/skybox1/left.jpg",
		"texture/skybox1/top.jpg",
		"texture/skybox1/bottom.jpg",
		"texture/skybox1/back.jpg",
		"texture/skybox1/front.jpg"
	};
	skybox.init("model/skybox.fbx", paths);
	//Light
	dLight.setDirection(glm::vec3(0, -1, 1));
	dLight.setColor(glm::vec3(1, 1, 1));
	dLight.setIntensity(0.5f);
}

void World::showAll()
{
	camController.controll();
	const glm::vec3 *pos = camera.getPos();
	float height = terrain.getHeight(pos->x, pos->z);
	camera.moveRelative(glm::vec3(0, height - pos->y + 1.6f, 0));
	terrain.render(camera, dLight);
	skybox.render(camera);
}

void World::framebufferSizeSignal(int width, int height)
{
	camController.changeCameraRatio(width * 1.f / height);
}

void World::keySignal(int key, int scan, int action, int mod)
{
	if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS) {
			camController.moveLeft();
		}
		else if (action == GLFW_RELEASE) {
			camController.finishLeft();
		}
	} else 
	if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS) {
			camController.moveRight();
		}
		else if (action == GLFW_RELEASE) {
			camController.finishRight();
		}
	} else 
	if (key == GLFW_KEY_W) {
		if (action == GLFW_PRESS) {
			camController.moveToward();
		}
		else if (action == GLFW_RELEASE) {
			camController.finishToward();
		}
	} else
	if (key == GLFW_KEY_S) {
		if (action == GLFW_PRESS) {
			camController.moveBackward();
		}
		else if (action == GLFW_RELEASE) {
			camController.finishBackward();
		}
	}
}

void World::cursorPosSignal(
	double posx, double posy)
{
	double upY = -posy;
	camController.changeDirection(posx, upY);
}

//_________________CameraController
void World::CameraController::changeCameraRatio(float ratio)
{
	camera->setRatio(ratio);
}

void World::CameraController::setMeterPerPixel(double mpp)
{
	meterPerPixel = mpp;
}

void World::CameraController::changeDirection(double fx, double fy)
{
	if (firstX != 0 || firstY != 0) {
		double dx = (fx - firstX) * 0.00002;
		double dy = (fy - firstY) * 0.00002;
		camera->changeDirectionOnLen(dx, dy);
	}
	firstX = fx;
	firstY = fy;
}

void World::CameraController::controll()
{ 
	if (isLeft) {
		camera->moveRelative(glm::vec3(-0.3f, 0, 0));
	}
	else if (isRight) {
		camera->moveRelative(glm::vec3(0.3f, 0, 0));
	}
	else if (isToward) {
		camera->moveRelative(glm::vec3(0, 0, -0.3f));
	}
	else if (isBackward) {
		camera->moveRelative(glm::vec3(0, 0, 0.3f));
	}
}

void World::CameraController::setCamera(Camera * camera)
{
	this->camera = camera;
}

void World::CameraController::moveLeft()
{
	isLeft = true;
}

void World::CameraController::finishLeft()
{
	isLeft = false;
}

void World::CameraController::moveRight()
{
	isRight = true;
}

void World::CameraController::finishRight()
{
	isRight = false;
}

void World::CameraController::moveToward()
{
	isToward = true;
}

void World::CameraController::finishToward()
{
	isToward = false;
}

void World::CameraController::moveBackward()
{
	isBackward = true;
}

void World::CameraController::finishBackward()
{
	isBackward = false;
}
//CameraController_________________