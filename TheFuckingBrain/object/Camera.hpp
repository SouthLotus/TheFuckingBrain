#ifndef __camera
#define __camera
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
class Camera {
private:
	static const float minR;
	static const float minH;
	static const glm::vec3 up;
	glm::vec3 pos;
	glm::vec3 at;
	glm::mat4 lookAt;
	glm::mat4 proj;
	float fovy;
	float ratio;
	float zNear;
	float zFar;
public:
	Camera() = default;
	Camera(glm::vec3 pos,
		glm::vec3 at,
		float fovy, float ratio, float zNear, float zFar);
	~Camera();
	void setProj(float fovy, float ratio, float zNear, float zFar);
	void setLookAt(glm::vec3 pos, glm::vec3 at);
	void setPos(glm::vec3 pos);
	void setAt(glm::vec3 at);
	void setFovy(float fovy);
	void setRatio(float ratio);
	void setZNear(float zNear);
	void setZFar(float zFar);
	const glm::vec3 * getPos() const;
	const glm::vec3 * getAt() const;
	const glm::vec3 * getUp() const;
	const glm::mat4 * getLookAt() const;
	const glm::mat4 * getProj() const;
	float getZNear() const;
	float getZFar() const;
	float getFovy() const;
	float getRatio() const;
	void moveRelative(glm::vec3 amount);
	void changeDirectionOnLen(double dx, double dy);
};
#endif