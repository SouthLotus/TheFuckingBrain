#include "Camera.hpp"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "..\tool\MathTool.hpp"

const float Camera::minR = 0.25f;
const glm::vec3 Camera::up = glm::vec3(0, 1, 0);
const float Camera::minH = glm::sqrt(1 + minR * minR);

Camera::Camera(glm::vec3 pos, glm::vec3 at,
	float fovy, float ratio, float zNear, float zFar):
	pos(pos), at(at), fovy(fovy), ratio(ratio),
	zNear(zNear), zFar(zFar)
{
	lookAt = glm::lookAt(pos, at, up);
	proj = glm::perspective(fovy, ratio, zNear, zFar);
}

Camera::~Camera() {

}

void Camera::setProj(float fovy, float ratio, float zNear, float zFar)
{
	this->fovy = fovy;
	this->ratio = ratio;
	this->zNear = zNear;
	this->zFar = zFar;
	proj = glm::perspective(fovy, ratio, zNear, zFar);
}

void Camera::setLookAt(glm::vec3 pos, glm::vec3 at)
{
	this->pos = pos;
	this->at = at;
	lookAt = glm::lookAt(pos, at, up);
}

void Camera::setPos(glm::vec3 pos)
{
	this->pos = pos;
	lookAt = glm::lookAt(pos, at, up);
}

void Camera::setAt(glm::vec3 at)
{
	this->at = at;
	lookAt = glm::lookAt(pos, at, up);
}

void Camera::setFovy(float fovy)
{
	this->fovy = fovy;
	proj = glm::perspective(fovy, ratio, zNear, zFar);
}

void Camera::setRatio(float ratio)
{
	this->ratio = ratio;
	proj = glm::perspective(fovy, ratio, zNear, zFar);
}

void Camera::setZNear(float zNear)
{
	this->zNear = zNear;
	proj = glm::perspective(fovy, ratio, zNear, zFar);
}

void Camera::setZFar(float zFar)
{
	this->zFar = zFar;
	proj = glm::perspective(fovy, ratio, zNear, zFar);
}

const glm::vec3 * Camera::getPos() const 
{
	return &pos;
}

const glm::vec3 * Camera::getAt() const
{
	return &at;
}

const glm::vec3 * Camera::getUp() const
{
	return &up;
}

const glm::mat4 * Camera::getLookAt() const
{
	return &lookAt;
}

const glm::mat4 * Camera::getProj() const
{
	return &proj;
}

float Camera::getZNear() const
{
	return zNear;
}

float Camera::getZFar() const
{
	return zFar;
}

float Camera::getRatio() const
{
	return ratio;
}

float Camera::getFovy() const
{
	return fovy;
}

//amount trong he toa do giong cua camera nhung thanh phan direction
//nam tren mat phang oxy cua world
//nen ta phai chuyen amount sang he oxy world
void Camera::moveRelative(glm::vec3 amount) {
	glm::vec3 noYAt(at);
	glm::vec3 noYPos(pos);
	//noYAt.y = 0;
	//noYPos.y = 0;
	glm::vec3 nZ(noYPos - noYAt);
	glm::vec3 nY(0, 1, 0);
 	glm::mat3 toWorld(matht::thisToAnother(
		glm::cross(nY, nZ),  //nx
		nY,  //ny
		nZ  //nz
	));
	glm::vec3 worldAmount = toWorld * amount;
	pos += worldAmount;
	at += worldAmount;
	lookAt = glm::lookAt(pos, at, up);
}

//Dieu chinh At sao cho khong xay ra gimba lock
//Dat ra mot ban kinh minR nho nhat vuong goc voi truc up,
//Ve mot vong tron xung quanh up va 2 mR (tren/duoi)
//Lay ti le k = mH (canh huyen cua tam giac tao boi truc up va 1 mR) / (apVec = vec(At - Pos)).
//Lay ra vecXZ cua apVec
//Neu k * apVec <= mR thi At co kha nang gimba lock nen thuc hien cac viec sau
//tinh ti le k' = minR / |vecXZ|;
//tinh vi tri At moi: newAt = vec3(k' * vecXZ.x, dau cua apVec.y, k * vecXZ.z);
void Camera::changeDirectionOnLen(double dx, double dy)
{
	glm::vec3 newRelativeAt(dx, dy, - zNear);
	glm::mat3 invLookat = glm::inverse(glm::mat3(lookAt));
	glm::vec3 dir = invLookat * newRelativeAt;
	glm::vec3 newWorldAt;
	float paLen = glm::length(dir);
	float paXZLen = glm::sqrt(dir.x * dir.x + dir.z * dir.z);
	float k = minH / paLen;
	if (paXZLen * k <= minR) {
		k = minR / paXZLen;
		newWorldAt = glm::vec3(dir.x * k, glm::sign(dir.y), dir.z * k) + pos;
	}
	else {
		newWorldAt = dir + pos;
	}
	setAt(newWorldAt);
}
