#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <iostream>

#include "Parameters.h"

#define PI 3.141592653589793238462643383279502884L 

using glm::vec3;
using glm::mat4;
using glm::cross;
using glm::normalize;


class Camera
{
private:
	double yawValue = 0.0f, pitchValue = 0.0f, rollValue = 0.0f;
	int lastX = -1, lastY = -1, xOffset, yOffset;
	
	vec3 direction;
	vec3 up;
	vec3 right;

	mat4 viewMatrix;

	void computeVectors();
	void rebuildViewMatrix();
	void computeDirection();

public:
	vec3 position;

	Camera();
	Camera(vec3 position, vec3 pointToLook);
	~Camera();

	void move(vec3 newLocation);
	void lookAt(vec3 pointToLookAt); // à finir

	// les valeurs positives bougent la camera vers le haut et les négatives vers le bas
	void moveUpward(float offset);
	// les valeurs positives bougent la camera vers la droite et les négatives vers la gauche
	void moveSideWays(float offset);
	// les valeurs positives bougent la camera vers l'avant et les négatives vers l'arrière
	void moveForward(float offset);

	void yaw(float angleInRadians);
	void pitch(float angleInRadians);
	void roll(float angleInRadians); // à finir

	void proccessMouse(int x, int y);

	glm::mat4 getViewMatrix() { return viewMatrix; }
};
