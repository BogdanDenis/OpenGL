#ifndef _OPENGL_INIT_
#define _OPENGL_INIT_

#include <GL/glew.h>
#include <glfw3.h>
#include <GL/freeglut.h>
#include <fstream>


int InitOpenGL (GLFWwindow *&window, int width, int height, int version_minor, int version_major) {

	if (!glfwInit ()) {
		fprintf (stderr, "Failed to initialize GLFW!\n");
		return 1;
	}
	glfwWindowHint (GLFW_SAMPLES, 4);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow (width, height, "Test", NULL, NULL);
	if (!window) {
		fprintf (stderr, "Could not create window!\n");
		glfwTerminate ();
		return 2;
	}

	glfwMakeContextCurrent (window);
	glewExperimental = GL_TRUE;
	if (glewInit ()) {
		fprintf (stderr, "Failed to initialize GLEW!\n");
		return 3;
	}
	glfwSetInputMode (window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwGetFramebufferSize (window, &width, &height);
	glViewport (0, 0, width, height);
	glEnable (GL_DEPTH_TEST);
	return 0;
}

#endif