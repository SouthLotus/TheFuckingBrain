#ifndef __shadertool
#define __shadertool
#include <gl\glew.h>
#include <string>
class ShaderTool {
public:
	ShaderTool();
	~ShaderTool();
	static std::string loadShaderCode(const char * path);
	static std::string checkShaderStatus(GLuint shader);
	static std::string checkProgramStatus(GLuint program);
};
#endif