#include "AnimatedSprite.h"


AnimatedSprite::AnimatedSprite(std::string filename, std::string name, 
	GLfloat x, GLfloat y, int framesPerSecond,
	int noOfAnimations,int framesPerAnimation, int frameWidth, 
	int frameHeight, int noOfRows)
	:Sprite(filename, name, x, y),  // Sprite const
	noOfAnimations(noOfAnimations),
	framesPerAnimation(framesPerAnimation),
	frameWidth(frameWidth),
	frameHeight(frameHeight),
	noOfRows(noOfRows),
	currentFrameIndex(0),
	currentAnimationIndex(0),
	timeElapsed(0),
	reverse(false),
	isLooping(true),
	finished(false)
{
	timeToUpdate = static_cast<float>(1) / framesPerSecond;
	CreateFrames();
}


AnimatedSprite::~AnimatedSprite(void)
{
	// Delete each row first
	for(int i = 0; i < noOfRows; ++i)
	{
		delete [] frames[i];
	}

	delete [] frames;
}

AnimatedSprite::AnimatedSprite(const AnimatedSprite &source)
	:Sprite(source.filename, source.name, source.positionX, source.positionY)
{
	frames = new SDL_Rect*[noOfAnimations * noOfRows];
	for(int i = 0; i < source.noOfRows; ++i)
	{
		frames[i] = new SDL_Rect[source.framesPerAnimation];
		*frames[i] = *source.frames[i];
	}
}

void AnimatedSprite::Update(float deltaTime)
{
	// Update animation
	timeElapsed += deltaTime;

	if(timeElapsed > timeToUpdate && !finished)
	{
		timeElapsed -= timeToUpdate;

		if(reverse)
		{
			if(currentFrameIndex > 0)
				currentFrameIndex--;
			else
			{
				if(isLooping)
					currentFrameIndex = framesPerAnimation - 1;
				else
					finished = true;
			}
		}
		else
		{
			if(currentFrameIndex < framesPerAnimation - 1)
				currentFrameIndex++;
			else if(isLooping)
				currentFrameIndex = 0;
		}
	}
}

void AnimatedSprite::Draw()
{
	// Setup the texture coords to wrap the current animation at the current frame
	bounds = frames[currentAnimationIndex][currentFrameIndex];

	Sprite::Draw();
}

void AnimatedSprite::CreateFrames()
{
	int animWidth = frameWidth * framesPerAnimation;

	// Create the array with the noOfAnimations (rows)
	frames = new SDL_Rect*[noOfAnimations * noOfRows];

	// Fill each row with the number of frames (cols)
	for(int i = 0; i < noOfAnimations * noOfRows; i++)
		frames[i] = new SDL_Rect[framesPerAnimation];

	// fill the 2D array
	for (int i = 0; i < noOfRows; i++)
	{
		for (int j = 0; j < noOfAnimations; j++)
		{
			for (int k = 0; k < framesPerAnimation; k++)
			{
				frames[j + (i * noOfAnimations)][k].x = (j * animWidth) + (k * frameWidth);
				frames[j + (i * noOfAnimations)][k].y = i * frameHeight;
				frames[j + (i * noOfAnimations)][k].w = frameWidth;
				frames[j + (i * noOfAnimations)][k].h = frameHeight;
			}
		}
	}
}