#ifndef __world
#define __world
#include "..\object\Camera.hpp"
#include "..\model\TexturedCube.hpp"
#include "..\model\SkyBox.hpp"
#include "..\object\DirectionalLight.hpp"
#include "..\model\StaticLandscape.hpp"
class World {
private:

	static class CameraController {
	private:
		bool isLeft = false;
		bool isRight = false;
		bool isToward = false;
		bool isBackward = false;
		double firstX = 0;
		double firstY = 0;
		double meterPerPixel;
		Camera *camera;
	public:
		
		CameraController() = default;
		~CameraController() = default;
		void setCamera(Camera *camera);
		void moveLeft();
		void finishLeft();
		void moveRight();
		void finishRight();
		void moveToward();
		void finishToward();
		void moveBackward();
		void finishBackward();
		void changeCameraRatio(float ration);
		void setMeterPerPixel(double mpp);
		void changeDirection(double fx, double y);
		void controll();
	};

	Camera camera;
	StaticLandscape land;
	SkyBox skybox;
	DirectionalLight dLight;
	CameraController camController;

public:
	World() = default;
	~World() = default;
	void makeWorld();
	void showAll();
	void framebufferSizeSignal(int width, int height);
	void keySignal(int key, int scan, int action, int mod);
	void cursorPosSignal(
		double posx, double posy);
};
#endif