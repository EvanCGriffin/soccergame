#include "Player.h"

#include <Year3Engine\CirclePhysicsObject.h>
#include <Year3Engine\Engine.h>
#include "SteeringBehaviors.h"
#include "SoccerMatch.h"
#include <Year3Engine\MessageDispatcher.h>
#include "SoccerMessages.h"

#include <iostream>

Player::Player(Team* team, int homeRegion, player_role role, bool dummyPlayer, GLfloat x, GLfloat y)
	: AnimatedPhysicsSprite("Content//Sprites//Player//playerSpriteSheetTransparent.png", "Player", x, y, 
	10, 8, 5, 40, 40, 2), // 10 frames per sec, 8 animations of 5 frames
	team(team),
	homeRegion(homeRegion),
	defaultRegion(homeRegion),
	startingPos(x * 0.04, y * 0.04),
	radius(0.4),
	isControlled(false),
	hasPossession(false),
	tackling(false),
	tackled(false),
	maskTexture(0),
	controlledSprite("Content//Sprites//CurrentPlayerIndicator.png"),
	maxForce(1),
	maxSpeed(4),
	playerRole(role),
	previousPosition(0,0),
	distSqToBall(0),
	passPower(6),
	shotPower(9)
{
	this->dummyPlayer = dummyPlayer; // should it be controlled with keyboard

	controlledSprite.drawColor[0] = team->teamColor[0];
	controlledSprite.drawColor[1] = team->teamColor[1];
	controlledSprite.drawColor[2] = team->teamColor[2];
	controlledSprite.drawColor[3] = 0.5f; // semi transparent

	velocity = b2Vec2(0,0);

	steering = new SteeringBehaviors(this, team->match->pitch.get(), team->match->ball.get());

	steering->SetTarget(team->match->pitch->GetRegionFromIndex(homeRegion)->center);

	smoother = new Smoother<b2Vec2>(10, b2Vec2(0,0)); // 10 samples
}

Player::~Player(void)
{
	delete steering;
	delete smoother;
}

void Player::LoadContent()
{
	controlledSprite.parent = parent;
	controlledSprite.LoadContent();

	AnimatedSprite::LoadContent();

	// load the alpha mask as a sdl sprite
	SDL_Surface* sdlSprite = LoadImage("Content//Sprites//Player//playerSpriteSheetAlphaMask.png");

	if(sdlSprite != NULL)
	{
		maskTexture = Sprite::CreateTexture(sdlSprite);
	}

	// Create our box object
	CirclePhysicsObject circleObject(radius);
	circleObject.CreateObject(parent->enginePtr->world, positionX, positionY, PLAYER, BALL | PLAYER | BOUNDARY | TACKLED_PLAYER);
	circleObject.body->SetType(b2BodyType::b2_dynamicBody);
	circleObject.body->SetLinearDamping(1.0f);
	circleObject.body->SetAngularDamping(1.0f);


	circleObject.body->SetUserData(this);

	// store our physics object
	physicsObject = circleObject;
	controlledSprite.physicsObject = circleObject;
}

void Player::HandleInput(Input input)
{
	if(tackled || !isControlled || !team->Human())
		return;

	velocity.SetZero();

	if((input.IsKeyDown(Keys::KEY_c) && !player2) || (input.IsKeyDown(Keys::KEY_MINUS) && player2))
	{
		if(!hasPossession) 
			tackling = true;
	}
	else
		tackling = false;

	if((input.IsKeyDown(Keys::KEY_w) && !player2) || (input.IsKeyDown(Keys::KEY_i) && player2))
	{
		velocity.y -= 0.5;

		if((input.IsKeyDown(Keys::KEY_a) && !player2) || (input.IsKeyDown(Keys::KEY_j) && player2))
		{
			velocity.x -= 0.6;
		}
		else if((input.IsKeyDown(Keys::KEY_d) && !player2) || (input.IsKeyDown(Keys::KEY_l) && player2))
		{
			velocity.x += 0.6;
		}
		else
		{
			velocity.y -= 0.35;
		}
	}
	else if((input.IsKeyDown(Keys::KEY_s) && !player2) || (input.IsKeyDown(Keys::KEY_k) && player2))
	{
		velocity.y += 0.5;

		if((input.IsKeyDown(Keys::KEY_a) && !player2) || (input.IsKeyDown(Keys::KEY_j) && player2))
		{
			velocity.x -= 0.6;
		}
		else if((input.IsKeyDown(Keys::KEY_d) && !player2) || (input.IsKeyDown(Keys::KEY_l) && player2))
		{
			velocity.x += 0.6;
		}
		else
		{
			velocity.y += 0.35;
		}
	}
	else if((input.IsKeyDown(Keys::KEY_a) && !player2) || (input.IsKeyDown(Keys::KEY_j) && player2))
	{
		velocity.x -= 0.85;
	}
	else if((input.IsKeyDown(Keys::KEY_d) && !player2) || (input.IsKeyDown(Keys::KEY_l) && player2))
	{
		velocity.x += 0.85;
	}

	if((input.IsNewKeyPress(Keys::KEY_v) && !player2) || (input.IsNewKeyPress(Keys::KEY_PLUS) && player2))
	{
		if(hasPossession)
		{
			team->match->Shoot(this);
			parent->enginePtr->soundManager->PlaySound("Bomb"); // Play a sound
		}
	}

	if((input.IsNewKeyPress(Keys::KEY_c) && !player2) || (input.IsNewKeyPress(Keys::KEY_MINUS) && player2))
	{
		if(hasPossession)
			team->Pass(this);
	}
}

void Player::Update(float deltaTime)
{
	if(tackled)
	{
		velocity.SetZero();

		if(currentAnimationIndex < 8)
			currentAnimationIndex += 8;

		tackledTimer -= deltaTime;

		if(tackledTimer < 0)
		{
			b2Filter filter = physicsObject.fixture->GetFilterData();
			filter.categoryBits = PLAYER;
			physicsObject.fixture->SetFilterData(filter);
			tackled = false;
			this->isLooping = true;
		}
	}
	else
	{
		if(velocity.x == 0 && velocity.y == 0)
		{
			//currentFrameIndex = 0;
			// Get the direcion between the ball and player
			b2Vec2 dir =  team->match->ball->GetPositionInPixels() - GetPositionInPixels();
			dir.Normalize();

			// Override the rotation, with a dir looking at the ball
			ClampToDirection(dir);
		}
		else
		{
			if(!team->Human() || (team->Human() && !hasPossession))
				// Average the velocity over the last 10 frames to remove jitter
				velocity = smoother->Update(velocity);

			// Sets the current animation direction based on it's velocity
			ClampToDirection(velocity);

			velocity *= MaxSpeed();
		}

			physicsObject.body->SetLinearVelocity(velocity);
	}

	if(tackling && currentAnimationIndex < 8)
		currentAnimationIndex += 8;

	AnimatedSprite::Update(deltaTime);
}

void Player::Draw()
{
	if(isControlled)
		controlledSprite.Draw();

	// Draw the regular texture
	AnimatedSprite::Draw();

	if(maskTexture != 0)
	{
		// Draw the mask texture with the team's colBor
		glColor4f(team->teamColor[0], team->teamColor[1], team->teamColor[2], team->teamColor[3]);
		glBindTexture(GL_TEXTURE_2D, maskTexture);
		DrawTexture();
	}
}

void Player::BeginContact(b2Contact* contact, GameComponent* collideWith)
{
	if(collideWith->name == "Player")
	{
		Player* player = static_cast<Player*>(collideWith);

		if(player->team != team) // opposing team?
		{
			if(player->tackling && currentAnimationIndex < 8)
			{
				currentAnimationIndex += 8;
				if(hasPossession)
				{
					SetTackled();
					team->match->ball->Free();
				}
			}
			else if ((player->playerRole == Player::goal_keeper) && hasPossession)
			{
				team->match->ball->Free();
				SetTackled();
			}
		}

	}

	std::cout << "Collision!" << std::endl;
}

void Player::Reset()
{
	physicsObject.body->SetTransform(startingPos, 0);
}

GLfloat Player::getRotation()
{
	return 0;
}

void Player::SetTackled()
{
	tackled = true;

	b2Filter filter = physicsObject.fixture->GetFilterData();
	filter.categoryBits = TACKLED_PLAYER;
	physicsObject.fixture->SetFilterData(filter);

	tackledTimer = 2;

	this->isLooping = false;

	hasPossession = false;
	//team->inPossession = false;
}

const PitchRegion* const Player::HomeRegion()const
{
	return team->match->pitch->GetRegionFromIndex(homeRegion);
}

bool Player::AtTarget()
{
	return (DistanceSq(GetPositionInPixels(), Steering()->Target()) < 10);
}

bool Player::InHomeRegion() const
{
	if (playerRole == goal_keeper)
	{
		return team->match->pitch->GetRegionFromIndex(homeRegion)->Inside(GetPositionInPixels(), PitchRegion::normal);
	}
	else
	{
		return team->match->pitch->GetRegionFromIndex(homeRegion)->Inside(GetPositionInPixels(), PitchRegion::halfsize);
	}
}

bool Player::isClosestTeamMemberToBall() const
{
	return team->Closestplayer() == this;
}

bool Player::isClosestPlayerOnPitchToBall()const
{
	return isClosestTeamMemberToBall() && 
		(DistSqToBall() < team->Opponents()->ClosestDistToBallSq());
}

//----------------------------- FindSupport -----------------------------------
//
//  determines the player who is closest to the SupportSpot and messages him
//  to tell him to change state to SupportAttacker
//-----------------------------------------------------------------------------
void Player::FindSupport()
{    
	//if there is no support we need to find a suitable player.
	if (team->SupportingPlayer() == NULL)
	{
		Player* bestSupportingPlayer = team->DetermineBestSupportingAttacker();

		team->SetSupportingPlayer(bestSupportingPlayer);

		Dispatcher->DispatchMsg(
			this,
			team->SupportingPlayer(),
			Msg_SupportAttacker,
			NULL);
	}

	Player* bestSupportingPlayer = team->DetermineBestSupportingAttacker();

	//if the best player available to support the attacker changes, update
	//the pointers and send messages to the relevant players to update their
	//states
	if (bestSupportingPlayer && (bestSupportingPlayer != team->SupportingPlayer()))
	{

		if (team->SupportingPlayer())
		{
			Dispatcher->DispatchMsg(
				this,
				team->SupportingPlayer(),
				Msg_GoHome,
				NULL);
		}



		team->SetSupportingPlayer(bestSupportingPlayer);

		Dispatcher->DispatchMsg(
			this,
			team->SupportingPlayer(),
			Msg_SupportAttacker,
			NULL);
	}
}

bool Player::InHotRegion() const
{
	if(team->Home())
	{
		return fabs(physicsObject.body->GetPosition().y - team->match->pitch->topGoal->physicsObject.body->GetPosition().y) <
			team->match->pitch->PitchArea()->rect.h/3.0;
	}
	else
	{
		return fabs(physicsObject.body->GetPosition().y - team->match->pitch->bottomGoal->physicsObject.body->GetPosition().y) <
			team->match->pitch->PitchArea()->rect.h/3.0;
	}
}

void Player::ClampToDirection(b2Vec2 &vector)
{
	if(vector.x == 0 && vector.y == 0)
		return;

	double angle = RADIANS_TO_DEGREES(b2Atan2(-vector.y, vector.x));

	if(angle < 0)
		angle += 360;

	if (angle >= 0 && angle <= 22.5f || angle <= 360 && angle >= 360 - 22.5f) 
	{
		currentAnimationIndex = 0;
		physicsObject.body->SetTransform(physicsObject.body->GetPosition(), 0);
		return; // right
	}
	if (angle > 45 - 22.5f && angle < 45 + 22.5f) 
	{  
		currentAnimationIndex = 2;
		physicsObject.body->SetTransform(physicsObject.body->GetPosition(), 7.0 / 4.0 * b2_pi);
		return; // up right
	}
	if (angle >= 90 - 22.5f && angle <= 90 + 22.5f) 
	{
		currentAnimationIndex = 1;
		physicsObject.body->SetTransform(physicsObject.body->GetPosition(), 3.0 / 2.0 * b2_pi);
		return; // up
	}
	if (angle > 135 - 22.5f && angle < 135 + 22.5f) 
	{
		currentAnimationIndex = 3;
		physicsObject.body->SetTransform(physicsObject.body->GetPosition(), 5.0 / 4.0 * b2_pi);
		return; // up left
	}
	if (angle >= 180 - 22.5f && angle <= 180 + 22.5f) 
	{
		currentAnimationIndex = 7;
		physicsObject.body->SetTransform(physicsObject.body->GetPosition(), b2_pi);
		return; // left
	}
	if (angle > 225 - 22.5f && angle < 225 + 22.5f) 
	{
		currentAnimationIndex = 6;
		physicsObject.body->SetTransform(physicsObject.body->GetPosition(), 3.0 / 4.0 * b2_pi);
		return; // down left
	}
	if (angle >= 270 - 22.5f && angle <= 270 + 22.5f)
	{
		currentAnimationIndex = 4;
		physicsObject.body->SetTransform(physicsObject.body->GetPosition(), b2_pi / 2);
		return; // down
	}
	if (angle > 315 - 22.5f && angle < 315 + 22.5f) 
	{
		currentAnimationIndex = 5;
		physicsObject.body->SetTransform(physicsObject.body->GetPosition(), b2_pi / 4);
		return; // down right
	}
}

bool Player::BallWithinKeeperRange()const
{
	return (DistanceSq(GetPositionInPixels(), team->match->ball->GetPositionInPixels()) < 100); // keeper range
}

bool Player::isControllingPlayer()const
{
	return team->ControllingPlayer() == this;
}

bool Player::isAheadOfAttacker()const
{
	if(!team->ControllingPlayer())
		return false;

	if(team->Home())
	{
		return fabs(GetPositionInPixels().y - team->match->pitch->topGoal->GetPositionInPixels().y) <
			fabs(team->ControllingPlayer()->GetPositionInPixels().y - team->match->pitch->topGoal->GetPositionInPixels().y);
	}
	else
	{
		return fabs(GetPositionInPixels().y - team->match->pitch->bottomGoal->GetPositionInPixels().y) <
			fabs(team->ControllingPlayer()->GetPositionInPixels().y - team->match->pitch->bottomGoal->GetPositionInPixels().y);
	}
}

//------------------------- IsThreatened ---------------------------------
//
//  returns true if there is an opponent within this player's 
//  comfort zone
//------------------------------------------------------------------------
bool Player::isThreatened()const
{
	//check against all opponents to make sure non are within this
	//player's comfort zone
	std::vector<std::tr1::shared_ptr<Player>>::const_iterator curOpp;  
	curOpp = team->Opponents()->Members().begin();

	for (curOpp; curOpp != team->Opponents()->Members().end(); ++curOpp)
	{
		//calculate distance to the player. if dist is less than our
		//comfort zone, and the opponent is infront of the player, return true
		if (DistanceSq(GetPositionInPixels(), (*curOpp)->GetPositionInPixels()) < 3000)
		{        
			return true;
		}

	}// next opp

	return false;
}

//------------------------- WithinFieldOfView ---------------------------
//
//  returns true if subject is within field of view of this player
//-----------------------------------------------------------------------
bool Player::PositionInFrontOfPlayer(b2Vec2 position)const
{
	b2Vec2 ToSubject = position - GetPositionInPixels();
	ToSubject.Normalize();

	b2Vec2 facing = physicsObject.body->GetWorldVector(b2Vec2(1,0));
	facing.Normalize();

	if (b2Dot(ToSubject, facing) > 0) 
		return true;
	else
		return false;
}

bool Player::CloseEnoughToTackle()const
{
	if(!team->Opponents()->InControl() || tackled)
		return false;

	return (
		DistanceSq(team->match->ball->GetPositionInPixels(), GetPositionInPixels()) < 400 ||
		DistanceSq(team->Opponents()->ControllingPlayer()->GetPositionInPixels(), GetPositionInPixels()) < 400);
}