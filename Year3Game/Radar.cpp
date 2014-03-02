#include "Radar.h"
#include <Year3Engine\Engine.h>

Radar::Radar(Team* homeTeam, Team* awayTeam, Ball* ball)
	:Sprite( "Content//Sprites//radar.png",  "Radar", 550, 100),
	homeTeam(homeTeam),
	awayTeam(awayTeam),
	ball(ball),
	blip("Content//Sprites//radar_player.png", "Radar_blip", 0, 0)
{
	rotation = 90;
}

Radar::~Radar(void)
{
}

void Radar::LoadContent()
{
	Sprite::LoadContent(); // Load our radar
	blip.LoadContent(); // Load the blip texture
}

void Radar::Draw()
{
	// Pop the world matrix to draw directly to the screen (screen space)
	glPopMatrix();

	// Draw the radar
	Sprite::Draw();

	// Push a new matrix for the blips
	glPushMatrix();

	// Translate to where the radar is being drawn
	glTranslatef(positionX, positionY, 0);

	// Copy the team color to the blips
	blip.drawColor[0] = homeTeam->teamColor[0];
	blip.drawColor[1] = homeTeam->teamColor[1];
	blip.drawColor[2] = homeTeam->teamColor[2];
	blip.drawColor[3] = homeTeam->teamColor[3];

	// Loop through the player list and draw a blip at their respective positions
	for(homeTeam->playerIter = homeTeam->players.begin(); homeTeam->playerIter != homeTeam->players.end(); homeTeam->playerIter++)
	{
		blip.setPositionX((*homeTeam->playerIter)->getPositionX() / 8); 
		blip.setPositionY((*homeTeam->playerIter)->getPositionY() / 8);
		blip.Draw();
	}

	// Copy the team color to the blips
	blip.drawColor[0] = awayTeam->teamColor[0];
	blip.drawColor[1] = awayTeam->teamColor[1];
	blip.drawColor[2] = awayTeam->teamColor[2];
	blip.drawColor[3] = awayTeam->teamColor[3];

	// Loop through the player list and draw a blip at their respective positions
	for(awayTeam->playerIter = awayTeam->players.begin(); awayTeam->playerIter != awayTeam->players.end(); awayTeam->playerIter++)
	{
		blip.setPositionX((*awayTeam->playerIter)->getPositionX() / 8);
		blip.setPositionY((*awayTeam->playerIter)->getPositionY() / 8);
		blip.Draw();
	}
	
	// Copy the ball color to the blip
	blip.drawColor[0] = ball->drawColor[1];
	blip.drawColor[1] = ball->drawColor[1];
	blip.drawColor[2] = ball->drawColor[1];
	blip.drawColor[3] = ball->drawColor[1];
	
	//draws ball blip at respective position
	blip.setPositionX(ball->getPositionX() / 8);
	blip.setPositionY(ball->getPositionY() / 8);
	blip.Draw();
	

	// Pop the blip matrix
	glPopMatrix();

	// Push back on the world matrix
	glPushMatrix();
	
	// Translate by the camera's coords
	glTranslatef(parent->enginePtr->camera->positionX, parent->enginePtr->camera->positionY, 0);
}
