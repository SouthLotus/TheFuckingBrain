#include <gl\glew.h>
#include <gl\freeglut.h>
#include "ShaderTool.hpp"
#include <iostream>
#include <fstream>
#include <exception>
#include <string>

ShaderTool::ShaderTool() {

}

ShaderTool::~ShaderTool() {

}

//throws std::runtime_error
std::string ShaderTool::loadShaderCode(const char * path) {
 	std::ifstream file(path, std::ios::in | std::ios::binary);
 	if(!file.is_open()) {
 		throw std::runtime_error("ShaderTool::loadShaderCode exception.");
 	}
 	file.seekg(0, file.end);
 	std::streampos fileSize = file.tellg();
 	char * fileData = new char[(long)fileSize + 1];
 	file.seekg(0, file.beg);
 	file.read(fileData, fileSize);
	fileData[fileSize] = '\0';
 	std::string strFileData(fileData);
 	delete[] fileData;
 	file.close();
 	return strFileData;
 }

 std::string ShaderTool::checkShaderStatus(GLuint shader) {
 	int status;
 	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
 	if(status == GL_FALSE) {
 		int logLen;
 		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
 		char *log = new char[logLen];
		//shader, max length of log, get length, log
 		glGetShaderInfoLog(shader, logLen, NULL, log);
 		std::string strLog(log);
 		delete[] log;
 		return strLog;
 	}
 	return "";
 }

 std::string ShaderTool::checkProgramStatus(GLuint program) {
 	int status;
 	glGetProgramiv(program, GL_LINK_STATUS, &status);
 	if(!status) {
 		int logLen;
 		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
 		char *log = new char[logLen];
 		//program, max length of log, get length, log
 		glGetProgramInfoLog(program, logLen, NULL, log);
 		std::string strLog(log);
 		delete[] log;
 		return strLog;
 	}
 	return "";
 }

