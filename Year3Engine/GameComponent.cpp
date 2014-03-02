#include "GameComponent.h"

#include "GameScreen.h"

int GameComponent::nextValidID = 0;

GameComponent::GameComponent()
{
}

GameComponent::GameComponent(std::string name)
	: name(name)
{
	SetID(GetNextValidID());
}


GameComponent::~GameComponent(void)
{
}

GameComponent::GameComponent(const GameComponent &source)
{
	name = source.name;
	parent = new GameScreen();
	*parent = *source.parent;
}

void GameComponent::LoadContent()
{
}

void GameComponent::HandleInput(Input input)
{
}

void GameComponent::Update(float deltaTime)
{
}

void GameComponent::Draw()
{
}

void GameComponent::BeginContact(b2Contact* contact, GameComponent* collideWith)
{
}

void GameComponent::EndContact(b2Contact* contact, GameComponent* collideWith)
{
}

//----------------------------- SetID -----------------------------------------
//
//  this must be called within each constructor to make sure the ID is set
//  correctly. It verifies that the value passed to the method is greater
//  or equal to the next valid ID, before setting the ID and incrementing
//  the next valid ID
//-----------------------------------------------------------------------------
void GameComponent::SetID(int val)
{
	//make sure the val is equal to or greater than the next available ID
	assert ( (val >= nextValidID) && "<BaseGameEntity::SetID>: invalid ID");

	ID = val;

	nextValidID = ID + 1;
}
