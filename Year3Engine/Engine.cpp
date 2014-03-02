#include "Engine.h"
#include <stdlib.h>
#include <time.h>

#include "Keys.h"
#include "ContactListener.h"
#include "Utils.h"

Engine::Engine(b2Vec2 gravity, int velIterations, int posIterations)
	: velocityIterations(velIterations),
	positionIterations(posIterations),
	debug(false),
	physicsEnabled(true)
{
	currentTicks = SDL_GetTicks(); 
	previousTicks = currentTicks; 
	elapsedTicks = 0;
	bukkit = 0;

	targetFps = 60;
	ticksPerSecond = 1000;
	ticksPerFrame = ticksPerSecond / targetFps; 
	updateThreshold = ticksPerFrame * 5; // don't update more than this

	// time between frames
	deltaTime = static_cast<float>(ticksPerFrame) / ticksPerSecond;

	// Construct a world object, which will hold and simulate the rigid bodies.
	world = new b2World(gravity);

	camera = new Camera();

	// Debug Draw settings
	uint32 flags = b2Draw::e_shapeBit;
	world->SetDebugDraw(&debugDraw);
	debugDraw.SetFlags(flags);

	world->SetContactListener(new ContactListener());
}

Engine::~Engine(void)
{
	delete world;
	delete camera;
	delete soundManager;

	//SDL_FreeSurface(screen);
}

int Engine::Init(std::string title)
{
	// Start SDL 
	if(SDL_Init( SDL_INIT_VIDEO ) == -1)
		return -1;

	// Setup double Buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//kill sdl on exiting
	atexit(SDL_Quit);

	// setup screen (with OpenGL)
	screen = SDL_SetVideoMode(ScreenWidth, ScreenHeight, BitsPerPixel, SDL_OPENGL);

	if(screen == NULL)
		return -1;

	// set the window title
	SDL_WM_SetCaption(title.c_str(), NULL);

	//Initialise OpenGL
	InitGL();

	//Intialise Audio
	soundManager = new SoundManager();

	// seed random
	srand(time(NULL));

	return 0;
}

// Set the OpenGL State
void Engine::InitGL()
{
	// Setup 2D rendering
	glEnable(GL_TEXTURE_2D);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Clear color is green
	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);

	// Create the viewport
	glViewport(0, 0, ScreenWidth, ScreenHeight);

	// Clear with the clear colour
	glClear(GL_COLOR_BUFFER_BIT);

	// Set to Projecttion Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Create an orthographic projection
	glOrtho(0.0f, 640, 480, 0.0f, -1.0f, 1.0f);

	// Set back to view matrix
	glMatrixMode( GL_MODELVIEW );
}

// Game Loop
void Engine::Run()
{
	while (running)
	{
		// Seperate Update and Rendering loops
		// Regulate update ~60 while maximising Drawing
		previousTicks =	currentTicks;
		currentTicks = SDL_GetTicks();
		elapsedTicks = currentTicks - previousTicks;

		if ( elapsedTicks >= updateThreshold )
			elapsedTicks %= ticksPerFrame;

		for ( bukkit += elapsedTicks; bukkit >= ticksPerFrame; bukkit -= ticksPerFrame ) 
		{
			// Update input
			if(input.Update(screen))
				running = false;

			// Update the camera
			if(camera != NULL)
				camera->Update(deltaTime);

			// Allow each screen to update their components
			for(screenIter = gameScreens.begin(); screenIter != gameScreens.end(); screenIter++)
			{
				if(!(*screenIter)->paused)
					(*screenIter)->Update(deltaTime);
			}

			// Allow the camera to handle input
			if(camera != NULL)
				camera->HandleInput(input);

			// Allow each screen to handle input
			for(screenIter = gameScreens.begin(); screenIter != gameScreens.end(); screenIter++)
			{
				if(!(*screenIter)->paused)
					(*screenIter)->HandleInput(input);
			}

			if(physicsEnabled)
				// Step the physics simulation
				world->Step(deltaTime, velocityIterations, positionIterations);

			if(toDelete.size() > 0)
				DeleteScreens();
		}

		// Draw
		Draw();

		// Pause cpu for short time
		SDL_Delay(1);
	}

	//Quit SDL 
	SDL_Quit(); 
}

void Engine::Draw()
{
	//Fill the screen green 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable Blending
	glEnable(GL_BLEND);

	// Clear the Matrix
	glLoadIdentity();

	// Push the camera matrix
	glPushMatrix();

	// Translate scene to camera position
	if(camera != NULL)
		glTranslatef(camera->positionX, camera->positionY, 0);

	// Draw our components
	for(screenIter = gameScreens.begin(); screenIter != gameScreens.end(); screenIter++)
		(*screenIter)->Draw();

	// Disable Blending
	glDisable(GL_BLEND);

	// Scale the physics world
	glScalef(METERS_TO_PIXELS, METERS_TO_PIXELS, 0);

	// Draw Physics Data
	if(debug)
		world->DrawDebugData();

	// Pop the camera matrix
	glPopMatrix();

	// Draw to screen
	SDL_GL_SwapBuffers();
}

void Engine::AddScreen(GameScreen* screen)
{
	// Set the enginePtr
	screen->enginePtr = this;

	// Load the screen
	screen->LoadContent();

	// Add the screen to the list
	gameScreens.push_back(screen);
}

void Engine::RemoveScreen(GameScreen* screen)
{
	// can't delete while iterating over the list
	// so store the screen in a different list
	toDelete.push_back(screen);
}

void Engine::DeleteScreens()
{
	// Now it is safe to delete from the screens
	std::list<GameScreen*>::iterator del = toDelete.begin();

	while (del != toDelete.end())
	{
		gameScreens.remove(*del);
		delete (*del);
		++del;
	}

	toDelete.clear();
}