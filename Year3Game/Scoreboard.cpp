#include "Scoreboard.h"
#include <Year3Engine\Engine.h>
#include "SoccerMatch.h"

const int Scoreboard::NINETY_MINS_IN_SECS = 5400; // 90mins * 60secs

Scoreboard::Scoreboard(Team* homeTeam, Team* awayTeam, int minsPerGame)
	: Sprite( "Content//Sprites//scoreboard.png",  "Scoreboard", 150, 50),
	font("Content//Fonts//arial.ttf"),
	homeTeam(homeTeam),
	awayTeam(awayTeam),
	currentTime(0),
	displaySeconds(0),
	displayMinutes(0),
	gameOn(false)
{
	realtimeSeconds = minsPerGame * 60; // e.g 5mins = 300secs
}

Scoreboard::~Scoreboard(void)
{
}

void Scoreboard::LoadContent()
{
	Sprite::LoadContent();
	font.LoadContent();
}

void Scoreboard::Update(float deltaTime)
{
	if(!gameOn)
		return;

	currentTime += deltaTime;

	displaySeconds = NINETY_MINS_IN_SECS * (currentTime / realtimeSeconds);

	displayMinutes = displaySeconds / 60;
	displaySeconds %= 60;

	if(currentTime > realtimeSeconds)
		homeTeam->match->TimeUp();
}

void Scoreboard::Draw()
{
	glPopMatrix();

	Sprite::Draw();

	// Reset the string to blank
	stringStream.str("");

	// Add an extra 0 if less than 10
	if(displayMinutes < 10)
		stringStream << "0" << displayMinutes << ":";
	else
		stringStream << displayMinutes << ":";

	// Add an extra 0 if less than 10
	if(displaySeconds < 10)
		stringStream << "0" << displaySeconds;
	else
		stringStream << displaySeconds;

	// Add the rest of the scoreboard
	stringStream << "   "<< homeTeam->name << "              "<< awayTeam->name;

	// Team names are hardcoded atm
	// TODO: Replace with team->name and current time
	font.RenderText(stringStream.str(), 64, 56);

	// Reset the string to blank
	stringStream.str("");
	// Convert int to string using stringstream
	stringStream << homeTeam->score << " - " << awayTeam->score;
	font.RenderText(stringStream.str(), 163, 56, 0, 0, 0); // Black

	glPushMatrix();

	glTranslatef(parent->enginePtr->camera->positionX, parent->enginePtr->camera->positionY, 0);
}

void Scoreboard::SetGameOn()
{
	gameOn = true;
	//parent->enginePtr->soundManager->PlaySound("Whistle"); // Play a sound
}