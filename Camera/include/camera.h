#pragma once
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraKeyboardMovement
{
	MOVE_FORWARD,
	MOVE_BACKWARD,
	STRAFE_LEFT,
	STRAFE_RIGHT
};


enum CameraType
{
	FLY,
	FPS
};

class Camera
{
public:
	CameraType cameraType = FLY;

	glm::vec3 position;		//cameraPosition
	glm::vec3 target;		
	glm::vec3 up;			//TODO: not actually cameraUp, need to improve this architecture
	glm::vec3 direction;	//cameraDirection

	float yaw = -90.0f;
	float pitch;

	glm::mat4 view;

	float speed = 2.0f;

	//Just a vector which we'll  set to be in the same direction as cameraDirection and (mostly) a little bit infront of it.
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f); 

	float sensitivity = 0.08f;

	Camera(CameraType cameraType);

	Camera(CameraType cameraType, const glm::vec3& position, const glm::vec3& target, const glm::vec3& up);

	void UpdateCameraLookAtMatrix();

	//For W-A-S-D keyboard movements
	void ProcessKeyboard(CameraKeyboardMovement movementDirection);


	//Changes the camera direction based on offset generated from mouse movement
	void ProcessMouseMovement(double mouseX, double mouseY, float& lastMouseX, float& lastMouseY, bool& firstMouse);
};



