#include <SOIL.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>
#include <vector>

#include "ShaderProgram.h"
#include "Camera.h"
#include "Sphere.h"
#include "OpenGLInit.h"
#include "Terrain.h"

#define CAMERAX camera.getPosition ().x
#define CAMERAY camera.getPosition ().y
#define CAMERAZ camera.getPosition ().z
#define CAMERAPOS camera.getPosition ()

using namespace std;
using namespace glm;

bool keys[1024];
bool Vsync = false;
GLfloat lastX;
GLfloat lastY;
GLfloat viewDist = 1000.0;
GLfloat viewAngle = 45.0;
GLfloat cameraSpeed = 5.0f;
bool justStarted = true;
int polygonMode = GL_FILL;
bool modeChanged = false;

int width = 1024, height = 600;
vec3 lightColour (1.0f, 1.0f, 1.0f);
vec3 objColour (0.690, 0.882, 0.337);
vec3 lightPosition (0.0f, -10.0f, 0.0f);
vec3 quadPos (0.0f, 0.0f, 0.0f);
vec3 spherePos (100.0f, 500.0f, 100.0f);

void key_callback (GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback (GLFWwindow* window, double xpos, double ypos);
void do_movement (Camera &camera, GLfloat deltaTime);

Camera camera;

typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

void setVSync () {
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress ("wglSwapIntervalEXT");

	if (wglSwapIntervalEXT)
		wglSwapIntervalEXT (Vsync);
	Vsync ? Vsync = false : Vsync = true;
};


int main (int argc, char** argv) {
	GLFWwindow *window;
	if (InitOpenGL (window, width, height, 3, 4)) {
		printf ("Failed to initialize OpenGL!");
	}
	setVSync ();

	//CREATE SHADER PROGRAM
	ShaderProgram shaderProgram ("VertexShader.txt", "FragmentShader.txt");
	ShaderProgram sun ("simpleSphere.vert", "simpleSphere.frag");
	ShaderProgram sphere ("terrainShader.vert", "terrainShader.frag");
	sphere.AttachShader (GL_TESS_CONTROL_SHADER, "terrainShader.tessCont");
	sphere.AttachShader (GL_TESS_EVALUATION_SHADER, "terrainShader.tessEval");
	//sphere.AttachShader (GL_GEOMETRY_SHADER, "terrainShader.geom");
	//DEFINE VERTICES

	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	GLfloat quad[] = {
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	GLuint VBO, VAO, sphereVBO;
	glGenVertexArrays (1, &VAO);
	glGenBuffers (1, &VBO);
	glGenBuffers (1, &sphereVBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray (VAO);

	glBindBuffer (GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray (0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	Sphere sphereObj (5.0f, camera.getPosition (), spherePos);

	QuadTree::LODDiam.insert (pair <GLfloat, GLfloat> (50.0, 50.0));
	QuadTree::LODDiam.insert (pair <GLfloat, GLfloat> (150.0, 100.0));
	QuadTree::LODDiam.insert (pair <GLfloat, GLfloat> (400.0, 150.0));
	QuadTree::LODDiam.insert (pair <GLfloat, GLfloat> (600.0, 200.0));
	QuadTree::LODDiam.insert (pair <GLfloat, GLfloat> (1000.0, 300.0));
	QuadTree::LODDiam.insert (pair <GLfloat, GLfloat> (2000.0, 400.0));
	QuadTree::LODDiam.insert (pair <GLfloat, GLfloat> (3000.0, 500.0));


	GLuint gtexture, rtexture;
	glGenTextures (1, &gtexture);
	int gwidth, gheight;
	unsigned char* image = SOIL_load_image ("grass.jpg", &gwidth, &gheight, 0, SOIL_LOAD_RGB);
	glBindTexture (GL_TEXTURE_2D, gtexture);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, gwidth, gheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap (GL_TEXTURE_2D);
	SOIL_free_image_data (image);
	glBindTexture (GL_TEXTURE_2D, 0);

	glGenTextures (1, &rtexture);
	int rwidth, rheight;
	image = SOIL_load_image ("rock.jpg", &rwidth, &rheight, 0, SOIL_LOAD_RGB);
	glBindTexture (GL_TEXTURE_2D, rtexture);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, rwidth, rheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap (GL_TEXTURE_2D);
	SOIL_free_image_data (image);
	glBindTexture (GL_TEXTURE_2D, 0);


	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	int frames = 0;
	float fps = 0;
	GLfloat fpsStartTime = glfwGetTime ();
	GLfloat fpsEndTime = glfwGetTime ();
	string title;

	glfwSetKeyCallback (window, key_callback);
	glfwSetInputMode (window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback (window, mouse_callback);
	
	vector <vec3> data;
	data.reserve (1000000);

	srand (time (NULL));

	GLfloat a = glfwGetTime ();
	
	printf ("Time :%f\n", glfwGetTime () - a);

	Terrain terrain;

	glBindVertexArray (VAO);
	glBindBuffer (GL_ARRAY_BUFFER, sphereVBO);
	glBufferData (GL_ARRAY_BUFFER, sphereObj.getData ()->size () * sizeof (vec3), &(*sphereObj.getDataEl (0)), GL_STATIC_DRAW);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray (1);

	glBindVertexArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, 0);
	glPolygonMode (GL_FRONT_AND_BACK, GL_POINT);

	glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);
	glEnable (GL_DEPTH_TEST);
	//terrain.baseLOD (camera.getPosition (), terrain.baseQuad ());
	//terrain.terrainData (camera.getPosition (), terrain.baseQuad ());

	do {
		do_movement (camera, deltaTime);
		data.clear ();
		data.reserve (1000000);
		//terrain.Traverse (terrain.baseQuad ());
		//vector <vec3> terdata;
		//terdata.reserve (1000000);
		//terrain.traverseChildren (camera.getPosition (), terdata);
		//printf ("Time :%f\n", glfwGetTime () - t);
		printf ("Camera height : %f\n", camera.getPosition ().y);

		//terdata.clear ();
		viewDist = 30000;
		//printf ("Dist : %f\n", viewDist);

		if (modeChanged) {
			glPolygonMode (GL_FRONT_AND_BACK, polygonMode);
			modeChanged = false;
		}

		glClearColor (0.992f, 0.819f, 0.588f, 1.0f);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightPosition.x = cos (radians (glfwGetTime () * 10)) * 2000;
		lightPosition.y = sin (radians (glfwGetTime () * 10)) * 2000;

		title.clear ();
		fpsEndTime = glfwGetTime ();
		if (fpsEndTime - fpsStartTime >= 1.0) {
			fps = frames / (fpsEndTime - fpsStartTime);
			fpsStartTime = glfwGetTime ();
			frames = 0;
		}
		title += "OpenGL   FPS: ";
		printf ("%f\n", distance (camera.getPosition (), spherePos));
		title += to_string ((int)fps);
		glfwSetWindowTitle (window, title.c_str ());

		mat4 model;
		mat4 view;
		view = camera.LookAt ();
		mat4 projection;
		projection = perspective (float (radians (viewAngle)), GLfloat (width) / GLfloat (height), 2.0f, viewDist);

		//sphereObj.buildVertexData (camera.getPosition ());
		glUseProgram (sphere.getShaderProgramID ());
		GLuint modelLoc = glGetUniformLocation (sphere.getShaderProgramID (), "model");
		GLuint viewLoc = glGetUniformLocation (sphere.getShaderProgramID (), "view");
		GLuint projLoc = glGetUniformLocation (sphere.getShaderProgramID (), "projection");
		GLuint cameraPosLoc = glGetUniformLocation (sphere.getShaderProgramID (), "cameraPos");
		GLuint lightPosLoc = glGetUniformLocation (sphere.getShaderProgramID (), "lightPos");
		GLuint objColLoc = glGetUniformLocation (sphere.getShaderProgramID (), "objectColour");
		GLuint lightColLoc = glGetUniformLocation (sphere.getShaderProgramID (), "lightColour");
		GLuint grassLoc = glGetUniformLocation (sphere.getShaderProgramID (), "grass");
		GLuint rockLoc = glGetUniformLocation (sphere.getShaderProgramID (), "rock");
		GLuint viewDistLoc = glGetUniformLocation (sphere.getShaderProgramID (), "viewDist");

		model = mat4 ();

		glUniformMatrix4fv (viewLoc, 1, GL_FALSE, value_ptr (view));
		glUniformMatrix4fv (projLoc, 1, GL_FALSE, value_ptr (projection));
		glUniform3f (cameraPosLoc, camera.getPosition ().x, camera.getPosition ().y, camera.getPosition ().z);
		glUniformMatrix4fv (modelLoc, 1, GL_FALSE, value_ptr (model));
		glUniform3f (lightPosLoc, lightPosition.x, lightPosition.y, lightPosition.z);
		glUniform3f (objColLoc, objColour.x, objColour.y, objColour.z);
		glUniform3f (lightColLoc, lightColour.x, lightColour.y, lightColour.z);
		glUniformMatrix4fv (glGetUniformLocation (sphere.getShaderProgramID (), "view_projection"), 1, GL_FALSE, value_ptr (view * projection));
		glUniform1f (viewDistLoc, viewDist);
		glUniform1f (glGetUniformLocation (sphere.getShaderProgramID (), "viewAngle"), viewAngle);
		glActiveTexture (GL_TEXTURE0);
		glBindTexture (GL_TEXTURE_2D, gtexture);
		glUniform1i (grassLoc, 0);
		glActiveTexture (GL_TEXTURE1);
		glBindTexture (GL_TEXTURE_2D, rtexture);
		glUniform1i (rockLoc, 1);


		glPatchParameteri (GL_PATCH_VERTICES, 4);
		terrain.BuildVertexData (data, projection * view * model);
		glBindVertexArray (VAO);
		glBindBuffer (GL_ARRAY_BUFFER, VBO);
		if (data.size ()) {
			glBufferData (GL_ARRAY_BUFFER, data.size () * sizeof (vec3), &data[0], GL_DYNAMIC_DRAW);
			printf ("Size :%d\n", data.size ());
		}
		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray (0);
		glDrawArrays (GL_PATCHES, 0, data.size ());
		glBindVertexArray (0);


		glUseProgram (sun.getShaderProgramID ());
		modelLoc = glGetUniformLocation (sun.getShaderProgramID (), "model");
		viewLoc = glGetUniformLocation (sun.getShaderProgramID (), "view");
		projLoc = glGetUniformLocation (sun.getShaderProgramID (), "projection");
		GLuint radPos = glGetUniformLocation (sun.getShaderProgramID (), "radius");
		model = mat4 ();
		model = translate (model, lightPosition);
		glUniformMatrix4fv (viewLoc, 1, GL_FALSE, value_ptr (view));
		glUniformMatrix4fv (projLoc, 1, GL_FALSE, value_ptr (projection));
		glUniformMatrix4fv (modelLoc, 1, GL_FALSE, value_ptr (model));
		glUniform1f (radPos, sphereObj.getRadius ());

		glBindBuffer (GL_ARRAY_BUFFER, sphereVBO);
		glDrawArrays (GL_TRIANGLES, 0, sphereObj.getData ()->size ());
		glBindVertexArray (0);
		glBindBuffer (GL_ARRAY_BUFFER, 0);

		GLfloat currentFrame = glfwGetTime ();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;



		glDrawBuffer (GL_BACK);
		glfwSwapBuffers (window);
		glfwPollEvents ();
		frames++;
	} while (glfwGetKey (window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose (window));


	glDeleteVertexArrays (1, &VBO);
	glfwTerminate ();
	return 0;
}

void key_callback (GLFWwindow *window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose (window, GL_TRUE);
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}
void do_movement (Camera &camera, GLfloat deltaTime) {
	GLfloat cameraSpeed = 5.0f * deltaTime;
	if (keys[GLFW_KEY_F1]) {
		setVSync ();
	}
	if (keys[GLFW_KEY_RIGHT_BRACKET])
		cameraSpeed *= 2;
	if (keys[GLFW_KEY_LEFT_BRACKET])
		cameraSpeed /= 2;
	if (keys[GLFW_KEY_LEFT_SHIFT])
		cameraSpeed *= 100;
	if (keys[GLFW_KEY_W])
		camera.setPosition (cameraSpeed * camera.getFront ());
	if (keys[GLFW_KEY_S])
		camera.setPosition (-cameraSpeed * camera.getFront ());
	if (keys[GLFW_KEY_A])
		camera.setPosition (-normalize (cross (camera.getFront (), camera.getUp ())) * cameraSpeed);
	if (keys[GLFW_KEY_D])
		camera.setPosition (normalize (cross (camera.getFront (), camera.getUp ())) * cameraSpeed);
	if (keys[GLFW_KEY_E]) {
		camera.setAngles (0.0, 0.0, 5.0);
		camera.ChangeYawPitchRoll ();
	}
	else if (keys[GLFW_KEY_Q]) {
		camera.setAngles (0.0, 0.0, -5.0);
		camera.ChangeYawPitchRoll ();
	}
	if (keys[GLFW_KEY_KP_1]) {
		modeChanged = true;
		polygonMode = GL_LINE;
	}
	if (keys[GLFW_KEY_KP_2]) {
		modeChanged = true;
		polygonMode = GL_FILL;
	}
	if (keys[GLFW_KEY_KP_3]) {
		modeChanged = true;
		polygonMode = GL_POINT;
	}
}

void mouse_callback (GLFWwindow* window, double xpos, double ypos) {
	if (justStarted) {
		lastX = xpos;
		lastY = ypos;
		justStarted = false;
	}
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;
	GLfloat sensitivity = 0.3f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	GLfloat yawOff = xoffset;
	GLfloat pitchOff = yoffset;
	if (pitchOff || yawOff) {
		camera.setAngles (pitchOff, yawOff, 0.0);
		camera.ChangeYawPitchRoll ();
	}
}