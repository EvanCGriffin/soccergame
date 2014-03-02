#pragma once

#include "Sprite.h"

// Reads a spritesheet and automatically splits it up into seperate images (frames)
class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite(std::string filename, std::string name = "", GLfloat x = 0, GLfloat y = 0, int framesPerSecond = 5,
		int noOfAnimations = 1, int framesPerAnimation = 1, int frameWidth = 0, 
		int frameHeight = 0, int noOfRows = 1);

	virtual ~AnimatedSprite(void);

	AnimatedSprite(const AnimatedSprite &);

	virtual void Update(float deltaTime); // update the animation
	virtual void Draw(); // Draw the sprite at the current animation, at the current frame

protected:
	int noOfAnimations; // The number of animations
	int framesPerAnimation; // The number of frames in each animation
	int frameWidth; // The width of each frame
	int frameHeight; // The height of each frame
	int noOfRows; // The number of rows in the texture
	int currentFrameIndex; // The current frame being displayed
	int currentAnimationIndex; // The current animation being displayed
	float timeElapsed; // The amount of time that has elapsed since the last change
	float timeToUpdate; // The amount of time to display each frame
	bool reverse; // should the animation play in reverse
	bool isLooping; // Should the animation loop
	bool finished; // Is the animation finsihed

	SDL_Rect** frames; // A 2D array of animations and their respective frames, [currentAnimationIndex][currentFrameIndex]

	void CreateFrames(void); // Creates the 2D array of animations and stores in frames
};

