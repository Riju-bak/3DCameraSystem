#include "camera.h"


Camera::Camera(CameraType cameraType)
{
	this->cameraType = cameraType;
	position = glm::vec3(0.0f, 0.0f, 3.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	UpdateCameraLookAtMatrix();
}


Camera::Camera(CameraType cameraType, const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
{
	this->cameraType = cameraType;
	this->position = position;
	this->target = target;
	this->up = up;
	UpdateCameraLookAtMatrix();
}


void Camera::UpdateCameraLookAtMatrix()
{
	// this->view = glm::lookAt(this->position, this->target, this->up); //fly-cam

	//How this works behind the scenes
	// glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);	//**
	//
	// glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
	//
	// glm::vec3 cameraDirection = glm::normalize(cameraPos - target);	//**
	//
	// glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	//
	// glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection)); //**
	//
	// glm::vec3 cameraUp = glm::cross(cameraRight, cameraDirection); //**

	// A great thing about matrices is that if you define a coordinate space using 3 perpendicular(or
	// 	non - linear) axes you can create a matrix with those 3 axes plus a translation vector and you can
	// 	transform any vector to that coordinate space by multiplying it with this matrix

	// LookAt =
	// 	Rx		Ux		-Dx		0
	// 	Ry		Uy		-Dy		0
	// 	Rz		Uz		-Dz		0
	// 	-R.P  -U.P		-D.P	1

	// R represents the cameraRight vector
	// U represents the cameraUp vector
	// D represents the cameraDirection vector
	// P represents the cameraPos vector

	// target = position + front

	//We can create this LookAt view matrix either manually or by using the glm::LookAt function, which requires a position, target and up vector(not the same as cameraUp) respectively

	//We use target = position + front because we want the target to be just infront of the camera position, so W-A-S-D movement will be proper.
	view = glm::lookAt(position, position + front, up);
}

void Camera::ProcessKeyboard(CameraKeyboardMovement movementDirection)
{
	switch (movementDirection)
	{
	case MOVE_FORWARD:
		position += front * speed;
		break;
	case MOVE_BACKWARD:
		position -= front * speed;
		break;
	case STRAFE_LEFT:
		position -= glm::normalize(glm::cross(front, up)) * speed;
		break;
	case STRAFE_RIGHT:
		position += glm::normalize(glm::cross(front, up)) * speed;
		break;
	}
	if (cameraType == FPS)
	{
		position.y = 0;
	}
}


void Camera::ProcessMouseMovement(double mouseX, double mouseY, float& lastMouseX, float& lastMouseY, bool& firstMouse)
{
	if(firstMouse)
	{
		lastMouseX = static_cast<float>(mouseX);
		lastMouseY = static_cast<float>(mouseY);
		firstMouse = false;
	}
	//calculate xOffset and yOffset
	float xOffset = static_cast<float>(mouseX) - lastMouseX;

	float yOffset = static_cast<float>(mouseY) - lastMouseY;

	//Why this negation? Because when we move the mouse up, the objects must go down and  without this negation mouseY - lastMouseY will be positive
	//Thus the objects will go up, and we'll feel like the camera is moving downwards. We'll want that only if invert-mouse is enabled.
	yOffset = -yOffset;

	lastMouseX = mouseX;
	lastMouseY = mouseY;

	//update the yaw and pitch based on the offset parameters
	yaw += xOffset * sensitivity;
	pitch += yOffset * sensitivity;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	//update camera direction vector using yaw and pitch
	//set front to be a unit vector in the direction of camera direction
	direction.y = sin(glm::radians(pitch));
	direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));

	front = glm::normalize(direction); //this will create a Fly-cam

	// front = glm::normalize(glm::vec3(direction.x, 0, direction.z)); //This will create a FPS-cam
}
