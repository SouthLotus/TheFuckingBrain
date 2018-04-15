#ifndef __glprogramowner
#define __glprogramowner
#include <gl\glew.h>
class GLProgramOwner {
private:
	GLuint handler;
	bool flag = false;
public:
	GLProgramOwner() = default;
	GLProgramOwner(bool flag);
	~GLProgramOwner();
	GLuint get();
	void link();
	void use();
	void unUse();
	GLProgramOwner & operator =(
		GLProgramOwner &another);
};
#endif