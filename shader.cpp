#include "shader.h"
#include <GL/glew.h>

Shader::Shader(const char* vsPath, const char* fsPath) {
	std::string vCode;
	std::string fCode;
	std::ifstream vsFile;
	std::ifstream fsFile;

	vsFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	fsFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	try {
		// open files
		vsFile.open(vsPath);
		fsFile.open(fsPath);
		std::stringstream vsStream, fsStream;
		// read file's buffer contents into streams
		vsStream << vsFile.rdbuf();
		fsStream << fsFile.rdbuf();		
		// close file handlers
		vsFile.close();
		fsFile.close();
		// convert stream into string
		vCode = vsStream.str();
		fCode = fsStream.str();		
	} catch(std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vsCode = vCode.c_str();
	const char* fsCode = fCode.c_str();
	//compiling
	unsigned int vs, fs;
	// vertex shader
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsCode, NULL);
	glCompileShader(vs);
	checkCompileErrors(vs, "VERTEX");
	// fragment Shader
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsCode, NULL);
	glCompileShader(fs);
	checkCompileErrors(fs, "FRAGMENT");
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vs);
	glAttachShader(ID, fs);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vs);
	glDeleteShader(fs);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type) {
	int success;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	} else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}


