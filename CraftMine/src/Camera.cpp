#include "../include/Camera.h"


Camera::Camera() 
{
	position = vec3(0.0f, 0.0f, 0.0f);
	computeVectors();

	viewMatrix = mat4(1.0f);
	rebuildViewMatrix();
	proccessMouse(0, 0);
}


Camera::Camera(vec3 pos, vec3 pointToLook) 
{
	position = pos;
	direction = normalize(pos - pointToLook);
	computeVectors();
	
	viewMatrix = mat4(1.0f);
	rebuildViewMatrix();
	proccessMouse(0, 0);
}


Camera::~Camera() {}


void Camera::computeVectors() 
{
	up = vec3(0.0f, 1.0f, 0.0f);
	right = normalize(glm::cross(up, direction));
	up = glm::cross(direction, right);
}

void Camera::rebuildViewMatrix() 
{
	viewMatrix = glm::lookAt(position, position + (-direction), up);
}


void Camera::move(vec3 newLocation) 
{
	position = newLocation;
	rebuildViewMatrix();
}


void Camera::lookAt(vec3 pointToLookAt) 
{
	// TO DO 
	// direction = normalize(position - pointToLookAt);
	computeVectors();
	rebuildViewMatrix();
}


void Camera::computeDirection() 
{
	direction.x = glm::cos(yawValue) * glm::cos(pitchValue);
	direction.y = glm::sin(pitchValue);
	direction.z = glm::sin(yawValue) * glm::cos(pitchValue);

	computeVectors();
	rebuildViewMatrix();
}


void Camera::moveUpward(float offset) 
{
	position = position + vec3(0.0f, offset, 0.0f);
	rebuildViewMatrix();
}

void Camera::moveSideWays(float offset) 
{
	position = position + vec3(
		right.x * offset, 
		right.y * offset, 
		right.z * offset
	);
	rebuildViewMatrix();
}

void Camera::moveForward(float offset) 
{
	position = position - vec3(
		direction.x * offset, 
		0, 
		direction.z * offset
	);
	rebuildViewMatrix();
}


void Camera::yaw(float angleInRadians) 
{ 
	yawValue += angleInRadians; 
	computeDirection();
}

void Camera::pitch(float angleInRadians) 
{ 
	pitchValue += angleInRadians;
	
	double halfPI = PI * 0.5;

	if (pitchValue > halfPI)
		pitchValue = halfPI;

	if (pitchValue < -halfPI)
		pitchValue = -halfPI;

	computeDirection();
}

void Camera::roll(float angleInRadians) 
{ 
	rollValue += angleInRadians; 
	computeDirection();
}


void Camera::proccessMouse(int x, int y) 
{
	if (lastX == -1 || lastY == -1) {
		lastX = x;
		lastY = y;
	}

	yaw((x - lastX) * params::controls::HORIZONTAL_SENSITIVITY);
	pitch((y - lastY) * params::controls::VERTICAL_SENSITIVITY);

	lastX = x;
	lastY = y;
}
