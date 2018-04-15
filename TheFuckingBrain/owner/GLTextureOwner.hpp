#ifndef __gl_texture_owner
#define __gl_texture_owner
#include <gl\glew.h>
#include <vector>
class GLTextureOwner {
private:
	std::vector<GLuint> texs;
	bool flag = false;
	GLsizei num;
public:
	GLTextureOwner() = default;
	GLTextureOwner(GLsizei num);
	~GLTextureOwner();
	const GLuint * get() const;
	GLuint get(unsigned int index) const;
	GLTextureOwner & operator=(GLTextureOwner &another);
};
#endif