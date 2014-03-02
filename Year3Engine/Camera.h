#pragma once

#include <SDL\SDL_opengl.h>
#include "Input.h"

// A simple extensible camera class
class Camera
{
public:
	Camera(void);
	~Camera(void);

	// Handle any input
	virtual void HandleInput(Input input);
	// Update the camera
	virtual void Update(float deltaTime);

	GLfloat positionX; // The x coord of the camera
	GLfloat positionY; // The y coord of the camera
};

