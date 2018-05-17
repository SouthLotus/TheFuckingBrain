#ifndef __anim1_renderer
#define __anim1_renderer
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include "..\tool\MyTool.hpp"
#include "..\object\DirectionalLight.hpp"
#include "..\object\Camera.hpp"
#include "..\owner\GLVAOOwner.hpp"
#include "..\owner\GLVBOOwner.hpp"
#include "..\shader\Anim1Shader.hpp"
class WitherGraphic {
private:
	GLVBOOwner vboOwener;
	GLVAOOwner vaoOwner;
	Anim1Shader shader;
	std::vector<std::vector<glm::mat4>> mats;
	unsigned int maxAffectBones;
	unsigned int numBones;
	unsigned int ticksPerSecond;
	unsigned int duration;
	unsigned int numVertices;
	//transform
	glm::vec3 translate = glm::vec3(0.f);
	glm::vec3 scale = glm::vec3(1.f);
	glm::quat rotate = glm::quat(1.f, 0.f, 0.f, 0.f);
	//anim
	long long beginTime = 0;
	bool isAnim = false;
	void initVAO();
public:
	WitherGraphic();
	~WitherGraphic() = default;
	void render(Camera &Camera, DirectionalLight &light);
	void beginAnim();
	void endAnim();
	void setTranslate(glm::vec3 vec);
	void setScale(glm::vec3 vec);
	void setRotate(glm::tquat<float> quat);
};

#endif