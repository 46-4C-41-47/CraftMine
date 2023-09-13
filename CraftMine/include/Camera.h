#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <cmath>

# define PI 3.141592653589793238462643383279502884L 

using glm::vec3;
using glm::mat4;
using glm::cross;
using glm::normalize;


class Camera
{
private:
	double yawValue = 0.0f, pitchValue = 0.0f, rollValue = 0.0f;
	int lastX = -1, lastY = -1, xOffset, yOffset;

	vec3 position;
	
	vec3 direction;
	vec3 up;
	vec3 right;

	mat4 viewMatrix;

	void computeVectors();
	void rebuildViewMatrix();
	void computeDirection();

public:
	const float HORIZONTAL_SENSITIVITY = 0.002f, VERTICAL_SENSITIVITY = 0.0016f;
	const float CAM_SPEED = 0.1f;

	Camera();
	Camera(vec3 position, vec3 pointToLook);
	~Camera();

	void move(vec3 newLocation);
	void lookAt(vec3 pointToLookAt);

	// positive value move the camera upward, negative downward
	void moveUpward(float offset);
	// positive value move the camera to the right, negative to the left
	void moveSideWays(float offset);
	// positive value move the camera forward, negative backward
	void moveForward(float offset);

	void yaw(float angleInRadians);
	void pitch(float angleInRadians);
	void roll(float angleInRadians);

	void proccessMouse(int x, int y);

	glm::mat4 getViewMatrix() { return viewMatrix; }
};
