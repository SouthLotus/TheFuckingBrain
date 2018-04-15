#ifndef __gl_vbo_owner
#define __gl_vbo_owner
#include <gl\glew.h>
#include <vector>
class GLVBOOwner {
private:
	std::vector<GLuint> vbos;
	bool flag = false;
	GLsizei num;
public:
	GLVBOOwner() = default;
	GLVBOOwner(GLsizei num);
	~GLVBOOwner();
	const GLuint * get() const;
	GLuint get(unsigned int index) const;
	GLVBOOwner & operator=(GLVBOOwner &another);
};
#endif