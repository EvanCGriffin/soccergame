#pragma once

#include <SDL/SDL.h>
#include <Box2D\Dynamics\Contacts\b2Contact.h>

#include <string>
#include "Input.h"

class GameScreen; // Forward Declaration
struct Telegram;

// The base component to interface with the engine, 
// allows an object to load, update, draw and collide
// (provided a physics object is attached to an object)
class GameComponent
{
public:
	GameComponent();
	GameComponent(std::string name);
	virtual ~GameComponent(void);

	GameComponent(const GameComponent &);

	virtual void LoadContent(); // Load any content (called when added to a screen)
	virtual void Update(float deltaTime); // Update this component
	virtual void HandleInput(Input input); // Handle any input
	virtual void Draw(); // Draw this component

	// Called on first contact with another body
	virtual void BeginContact(b2Contact* contact, GameComponent* collideWith);
	// Called when no longer colliding with another body
	virtual void EndContact(b2Contact* contact, GameComponent* collideWith);

	virtual bool HandleMessage(const Telegram& msg) { return false; }

	std::string name; // a unique name to identify this component
	GameScreen* parent; // The screen this component is attached to

	//use this to grab the next valid ID
	static int GetNextValidID() { return nextValidID; }

	//this can be used to reset the next ID
	static void ResetNextValidID() { nextValidID = 0; }

private:
	//each component has a unique ID
	int ID;

	//this is the next valid ID. Each time a BaseGameEntity is instantiated
	//this value is updated
	static int nextValidID;

	//this must be called within each constructor to make sure the ID is set
	//correctly. It verifies that the value passed to the method is greater
	//or equal to the next valid ID, before setting the ID and incrementing
	//the next valid ID
	void SetID(int val);
};

