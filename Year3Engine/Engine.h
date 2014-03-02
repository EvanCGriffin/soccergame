#pragma once

#ifndef _ENGINE_H
#define _ENGINE_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <Box2D/Box2D.h>
#include <string>
#include <list>

#include "GameScreen.h"
#include "DebugDraw.h"
#include "Input.h"
#include "Camera.h"
#include "SoundManager.h"

class Engine
{
public:
	Engine(b2Vec2 gravity, int velocityIterations = 8, int positionIterations = 3);
	~Engine(void);

	int Init(std::string title); // Intialise SDL
	void Run(void); // Game loop

	// Add a screen to be managed calls LoadContent on that screen
	void AddScreen(GameScreen* screen); 
	// Remove a screen from the engine
	void RemoveScreen(GameScreen* screen);

	bool Debug() { return debug; }
	void ToggleDebug() { debug = !debug; }

	int ScreenWidth; // The width of the screen
	int ScreenHeight; // The height of the screen
	int BitsPerPixel; // The bits per pixel e.g. 8, 16, 32

	Input input; // Input that's passed to HandleInput method

	b2World* world; // Physics Simulation
	DebugDraw debugDraw; // Draws simple physics overlay
	Camera* camera; 
	SoundManager* soundManager; // Holds all sounds/music and access to play them

	int velocityIterations; // No of velocity iterations per step
	int positionIterations; // No of position iterations per step

	bool physicsEnabled;
private:

	std::list<GameScreen*> gameScreens;
	std::list<GameScreen*> toDelete;
	std::list<GameScreen*>::iterator screenIter;

	void InitGL(void); // Initialise OpenGL

	unsigned currentTicks; // Current Time
	unsigned previousTicks; // Time at last frame
	unsigned elapsedTicks; // Time between current and previous frame
	unsigned bukkit; // holds the left over time at the end of a frame

	unsigned targetFps; 
	unsigned ticksPerSecond; // 1000
	unsigned ticksPerFrame; // ticksPerSecond / targetFps
	unsigned updateThreshold; // We shouldn't update more than this

	float deltaTime; // Time between frames

	SDL_Surface* screen; // Points to the screen we're rendering to

	bool running; // used to control the game loop

	bool debug; // display Debug info

	void Draw(void); // Draws everything

	void DeleteScreens();
};

#endif

