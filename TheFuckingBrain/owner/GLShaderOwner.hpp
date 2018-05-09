#ifndef __glshaderowner
#define __glshaderowner
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