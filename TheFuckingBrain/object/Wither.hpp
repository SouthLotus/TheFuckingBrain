#ifndef __wither
#define __wither
#include "..\model\WitherGraphic.hpp"
#include "Camera.hpp"
#include "DirectionalLight.hpp"
#include "..\model\LowPolyTerrain.hpp"
#include <glm/glm.hpp>
class Wither {
private:
	WitherGraphic graphic;
	glm::vec3 position;
public:
	Wither() = default;
	~Wither() = default;
	void show(Camera &camera, DirectionalLight &dLight);
	void standOnTerrain(LowPolyTerrain &terrain);
	void moveAbsolute(glm::vec3 vec);
	void walk();
	void handBeat();
	void bulletBeat();
	void stand();
};
#endif