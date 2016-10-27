#include "Camera.h"

Camera::Camera () {
	cameraPosition = vec3 (0.0f, 0.0f, 3.0f);
	cameraTarget = vec3 (0.0f, 0.0f, 0.0f);
	cameraFront = vec3 (0.0f, 0.0f, -1.0f);
	vec3 cameraDirection = normalize (cameraPosition - cameraTarget);
	vec3 up = vec3 (0.0f, 1.0f, 0.0f);
	cameraRight = normalize (cross (up, cameraDirection));
	cameraUp = cross (cameraDirection, cameraRight);
	Yaw = -90.0f;
	Pitch = 0.0f;
	Roll = 0.0f;
}

Camera::~Camera () {

}

mat4 Camera::LookAt () {
	mat4 view;
	view = lookAt (cameraPosition, cameraPosition + cameraFront, cameraUp);
	return view;
}

void Camera::setAngles (GLfloat Pitch, GLfloat Yaw, GLfloat Roll) {
	this->Pitch += Pitch;
	if (this->Pitch > 89.0f)
		this->Pitch = 89.0f;
	else if (this->Pitch < -89.0f)
		this->Pitch = -89.0f;
	this->Roll += Roll;
	if (this->Roll >= 360)
		this->Roll = this->Roll - 360;
	else if (this->Roll <= -360)
		this->Roll = this->Roll + 360;
	this->Yaw += Yaw;
	if (this->Yaw >= 360)
		this->Yaw = this->Yaw - 360;
	else if (this->Yaw <= -360)
		this->Yaw = this->Yaw + 360;

	cameraFront.x = cos (radians (this->Pitch)) * cos (radians (this->Yaw));
	cameraFront.y = -sin (radians (this->Pitch));
	cameraFront.z = cos (radians (this->Pitch)) * sin (radians (this->Yaw));
	cameraFront = normalize (cameraFront);
}

void Camera::ChangeYawPitchRoll () {
	/*cameraFront.x = cos (radians (Pitch)) * cos (radians (Yaw));
	cameraFront.y = -sin (radians (Pitch));
	cameraFront.z = cos (radians (Pitch)) * sin (radians (Yaw));
	cameraUp.x = cos (radians (Yaw)) * sin (radians (Pitch));
	cameraUp.y = cos (radians (Pitch));
	cameraUp.z = sin (radians (Yaw)) * sin (radians (Pitch));
	mat4 rotation;
	rotation = rotate (rotation, Roll, cameraFront);
	cameraUp = vec3 (rotation * vec4 (cameraUp, 1.0));
	cameraUp = normalize (cameraUp);
	cameraFront = normalize (cameraFront);*/
	//mat4 mRoll = rotate (mRoll, Roll, cameraFront);
	//cameraUp = vec3 (mRoll * vec4 (cameraUp, 1.0));
	//cameraRight = vec3 (mRoll * vec4 (cameraRight, 1.0));
	/*mat4 mPitch = rotate (mPitch, Pitch, cameraRight);
	cameraFront = vec3 (mPitch * vec4 (cameraFront, 1.0));
	cameraUp = vec3 (mPitch * vec4 (cameraUp, 1.0));
	cameraFront = normalize (cameraFront);
	cameraUp = normalize (cameraUp);
	mat4 mYaw = rotate (mYaw, Yaw, cameraUp);
	cameraFront = vec3 (mYaw * vec4 (cameraFront, 1.0));
	cameraUp = vec3 (mYaw * vec4 (cameraUp, 1.0));
	cameraFront = normalize (cameraFront);
	cameraUp = normalize (cameraUp);
	cameraRight = normalize (cameraRight);*/
}

void Camera::ChangeRoll () {

}