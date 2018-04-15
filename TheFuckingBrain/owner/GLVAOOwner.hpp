#ifndef __gl_vao_owner
#define __gl_vao_owner
#include <gl\glew.h>
#include <vector>
class GLVAOOwner {
private:
	std::vector<GLuint> vaos;
	bool flag = false;
	GLsizei num;
public:
	GLVAOOwner() = default;
	GLVAOOwner(GLsizei num);
	~GLVAOOwner();
	const GLuint * get() const;
	GLuint get(unsigned int index) const;
	GLVAOOwner & operator=(GLVAOOwner &another);
};
#endif