#include "ShaderProgram.h"

ShaderProgram::ShaderProgram (string verShaderSourcePath, string fragShaderSourcePath) {
	GLint success;
	GLchar infoLog[512];
	CreateShader (GL_VERTEX_SHADER, verShaderSourcePath);
	CreateShader (GL_FRAGMENT_SHADER, fragShaderSourcePath);
	GLint shaderProgram = glCreateProgram ();
	for each (GLuint i in shaderIDs) {
		glAttachShader (shaderProgram, i);
	}
	glLinkProgram (shaderProgram);
	glGetProgramiv (shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog (shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	shaderProgramID = shaderProgram;
}


ShaderProgram::~ShaderProgram () {

}

void ShaderProgram::CreateShader (int T, string filePath) {
	GLchar infoLog[512];
	GLint success;
	GLuint Shader;
	Shader = glCreateShader (T);
	string str = FileReader::ReadShaderSource (filePath);
	const char *shaderSource = str.c_str ();
	glShaderSource (Shader, 1, &shaderSource, NULL);
	glCompileShader (Shader);

	glGetShaderiv (Shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog (Shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	shaderIDs.push_back (Shader);
}

void ShaderProgram::AttachShader (int T, string filePath) {
	GLint success;
	GLchar infoLog[512];
	CreateShader (T, filePath);
	glDeleteProgram (shaderProgramID);
	GLint shaderProgram = glCreateProgram ();
	for each (GLuint i in shaderIDs) {
		glAttachShader (shaderProgram, i);
	}
	glLinkProgram (shaderProgram);
	glGetProgramiv (shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog (shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	shaderProgramID = shaderProgram;
}
