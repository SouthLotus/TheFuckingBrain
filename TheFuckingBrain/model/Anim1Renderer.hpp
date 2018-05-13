#ifndef __anim1_renderer
#define __anim1_renderer
#include <vector>
#include <glm\glm.hpp>
#include "..\tool\MyTool.hpp"
#include "..\object\DirectionalLight.hpp"
#include "..\object\Camera.hpp"
#include "..\owner\GLVAOOwner.hpp"
#include "..\owner\GLVBOOwner.hpp"
#include "..\shader\Anim1Shader.hpp"
class Anim1Renderer {
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
	//anim
	long long beginTime = 0;
	bool isAnim = false;
	void initVAO();
public:
	Anim1Renderer();
	~Anim1Renderer() = default;
	void render(Camera &Camera, DirectionalLight &light);
	void beginAnim();
	void endAnim();
};

#endif