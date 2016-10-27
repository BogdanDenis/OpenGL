#include <GL/glew.h>
#include <glfw3.h>
#include <GL/freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <stdio.h>

using namespace glm;

class Camera {
private:
	vec3 cameraPosition;
	vec3 cameraFront;
	vec3 cameraTarget;
	vec3 cameraUp;
	vec3 cameraRight;
	GLfloat Yaw;
	GLfloat Pitch;
	GLfloat Roll;
public:
	Camera ();
	~Camera ();

	mat4 LookAt ();

	vec3 getPosition () { return cameraPosition; }
	vec3 getFront () { return cameraFront; }
	vec3 getTarget () { return cameraTarget; }
	vec3 getUp () { return cameraUp; }
	vec3 getRight () { return cameraRight; }
	void ChangeYawPitchRoll ();
	void ChangeRoll ();

	void setPosition (vec3 pos) { cameraPosition += pos; } // += operation
	void setFront (vec3 front) { cameraFront = front; } // = operation
	void setTarget (vec3 targ) { cameraTarget = targ; } // = operation
	void setUp (vec3 up) { cameraUp = up; } // = operation
	void setRight (vec3 right) { cameraRight = right; } // = operation
	void setAngles (GLfloat Pitch, GLfloat Yaw, GLfloat Roll);

	GLfloat getRoll () { return Roll; }
};

