#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <vector>
#include <iostream>

#include "FileReader.h"

using std::string;
using std::iostream;
using std::vector;

class ShaderProgram {
private:
	GLuint shaderProgramID;
	vector <GLuint> shaderIDs;
public:
	ShaderProgram (string verShaderSourcePath, string fragShaderSourcePath);
	~ShaderProgram ();

	GLuint getShaderProgramID () { return shaderProgramID; };

	void CreateShader (int T, string filePath);
	void AttachShader (int T, string filePath);
};