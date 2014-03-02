#pragma once

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>

#include "GameComponent.h"

// Draws an image at a specified position and rotation (in pixels, degrees)
class Sprite : public GameComponent
{
public:
	Sprite(std::string filename, std::string name = "", GLfloat x = 0, GLfloat y = 0);
	virtual ~Sprite(void);

	Sprite(const Sprite &);

	virtual GLfloat getPositionX(void);
	virtual GLfloat getPositionY(void);
	virtual GLfloat getRotation(void);

	virtual void setPositionX(GLfloat x);
	virtual void setPositionY(GLfloat y);
	virtual void setRotation(GLfloat rotation);

	virtual void LoadContent();
	virtual void Draw();

	// Color tint
	GLfloat drawColor[4];
protected:
	std::string filename;
	
	// Texture fields
	GLuint texture;
	GLfloat width;
	GLfloat height;
	SDL_Rect bounds;

	// Sprite Fields
	GLfloat positionX;
	GLfloat positionY;
	GLfloat rotation;
	GLfloat scaleX;
	GLfloat scaleY;
	
	// Loads an image using SDL
	SDL_Surface* LoadImage(std::string filepath);
	// Converts SDL image to OpenGL texture
	GLuint CreateTexture(SDL_Surface* sdlSprite);

	// Draw the currently bound texture
	void DrawTexture();
	};

