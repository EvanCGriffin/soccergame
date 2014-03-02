#pragma once

#include <vector>

#include "Input.h"
#include "GameComponent.h"

class Engine; // Forward Declaration

using namespace std::tr1;

// A GameScreen holds a collection of GameComponents 
// and updates/Draw them
class GameScreen
{
public:
	GameScreen();
	virtual ~GameScreen(void);

	virtual void LoadContent(); // Load any content
	virtual void Update(float deltaTime); // Update the screen and its components
	virtual void HandleInput(Input input); // Handle input and allow its components to handle input
	virtual void Draw(); // Draw, allow its components to draw

	// Adds a component to this screen, LoadContent is called on the component
	void AddComponent(shared_ptr<GameComponent> component);
	// Remove a component from this screen
	void RemoveComponent(shared_ptr<GameComponent> component);
	// Remove a component by name
	void RemoveComponent(std::string name);
	// Sort Components by draw order
	void SortComponents();

	Engine* enginePtr;

	bool paused;
private:
	// Holds all the components for the game screen
	std::vector<shared_ptr<GameComponent>> components;
	std::vector<shared_ptr<GameComponent>>::iterator compIter;
};

