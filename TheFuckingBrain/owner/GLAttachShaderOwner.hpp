#ifndef __glattachshaderowner
#define __glattachshaderowner
#include <gl\glew.h>
#include <vector>

class GLAttachShaderOwner {
private:
	GLuint handler;
	std::vector<GLuint> shaders;
public:
	GLAttachShaderOwner(GLuint handler);
	~GLAttachShaderOwner();
	void attach(GLuint shader);
};
#endif