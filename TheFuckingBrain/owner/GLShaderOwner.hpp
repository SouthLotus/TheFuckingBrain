#ifndef __glshaderowner
#define __glshaderowner
#include <gl\glew.h>
class GLShaderOwner {
private:
	GLuint handler;
public:
	GLShaderOwner(int type);
	~GLShaderOwner();
	void compile(const char *codeFile);
	GLuint get();
};
#endif