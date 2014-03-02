#include "FieldPlayerStates.h"

#include <Year3Engine\MessageDispatcher.h>
#include <Year3Engine\Telegram.h>
#include <Year3Engine\Utils.h>

#include "FieldPlayer.h"
#include "SoccerMessages.h"
#include "SteeringBehaviors.h"
#include "PitchRegion.h"
#include "Team.h"
#include "SoccerMatch.h"


GlobalPlayerState* GlobalPlayerState::Instance()
{
	static GlobalPlayerState instance;

	return &instance;
}


void GlobalPlayerState::Update(FieldPlayer* player)                                     
{
	////if a player is in possession and close to the ball reduce his max speed
	//if((player->BallWithinReceivingRange()) && (player->isControllingPlayer()))
	//{
	//  player->SetMaxSpeed(Prm.PlayerMaxSpeedWithBall);
	//}

	//else
	//{
	//   player->SetMaxSpeed(Prm.PlayerMaxSpeedWithoutBall);
	//}

}


bool GlobalPlayerState::OnMessage(FieldPlayer* player, const Telegram& telegram)
{
	switch(telegram.Msg)
	{
	case Msg_ReceiveBall:
		{
			//set the target
			player->Steering()->SetTarget(*(static_cast<b2Vec2*>(telegram.ExtraInfo)));

			//change state 
			player->GetFSM()->ChangeState(ReceiveBall::Instance());

			return true;
		}

		break;

	case Msg_SupportAttacker:
		{
			//if already supporting just return
			if (player->GetFSM()->isInState(*SupportAttacker::Instance()))
			{
				return true;
			}

			//set the target to be the best supporting position
			player->Steering()->SetTarget(player->team->GetSupportSpot());

			//change the state
			player->GetFSM()->ChangeState(SupportAttacker::Instance());

			return true;
		}

		break;

	case Msg_Wait:
		{
			//change the state
			player->GetFSM()->ChangeState(Wait::Instance());

			return true;
		}

		break;

	case Msg_GoHome:
		{
			player->SetDefaultHomeRegion();

			player->GetFSM()->ChangeState(ReturnToHomeRegion::Instance());

			return true;
		}

		break;

	case Msg_PassToMe:
		{  

			//get the position of the player requesting the pass 
			FieldPlayer* receiver = static_cast<FieldPlayer*>(telegram.ExtraInfo);

			//if the ball is not within kicking range or their is already a 
			//receiving player, this player cannot pass the ball to the player
			//making the request.
			if (player->team->Receiver() != NULL || !player->hasPossession || player->team->Human())
			{
				return true;
			}

			b2Vec2 dir = receiver->GetPositionInPixels() - player->team->match->ball->GetPositionInPixels();
			dir.Normalize();

			//make the pass   
			player->team->match->ball->Pass(dir, player->PassPower());

			//let the receiver know a pass is coming 
			Dispatcher->DispatchMsg(player,
				receiver,
				Msg_ReceiveBall,
				&receiver->GetPositionInPixels());

			//change state   
			player->GetFSM()->ChangeState(Wait::Instance());

			player->FindSupport();

			return true;
		}

		break;

	}//end switch

	return false;
}

ReturnToHomeRegion* ReturnToHomeRegion::Instance()
{
	static ReturnToHomeRegion instance;

	return &instance;
}


void ReturnToHomeRegion::Enter(FieldPlayer* player)
{
	player->Steering()->ArriveOn();

	if (!player->HomeRegion()->Inside(player->Steering()->Target(), PitchRegion::halfsize))
	{
		player->Steering()->SetTarget(player->HomeRegion()->center);
	}
}

void ReturnToHomeRegion::Update(FieldPlayer* player)
{
	if (player->team->match->GameOn())
	{
		//if the ball is nearer this player than any other team member  &&
		//there is not an assigned receiver && the goalkeeper does not gave
		//the ball, go chase it
		if (player->isClosestTeamMemberToBall() &&
			!player->team->match->GoalKeeperHasBall())
		{
			player->GetFSM()->ChangeState(ChaseBall::Instance());

			return;
		}
	}

	//if game is on and close enough to home, change state to wait and set the 
	//player target to his current position.(so that if he gets jostled out of 
	//position he can move back to it)
	if (player->team->match->GameOn() && player->HomeRegion()->Inside(player->GetPositionInPixels(), PitchRegion::halfsize))
	{
		player->Steering()->SetTarget(player->GetPositionInPixels());
		player->GetFSM()->ChangeState(Wait::Instance());
	}
	//if game is not on the player must return much closer to the center of his
	//home region
	else if(!player->team->match->GameOn() && player->AtTarget())
	{
		player->GetFSM()->ChangeState(Wait::Instance());
	}
}

void ReturnToHomeRegion::Exit(FieldPlayer* player)
{
	player->Steering()->ArriveOff();
}


//***************************************************************************** WAIT

Wait* Wait::Instance()
{
	static Wait instance;

	return &instance;
}


void Wait::Enter(FieldPlayer* player)
{
	//if the game is not on make sure the target is the center of the player's
	//home region. This is ensure all the players are in the correct positions
	//ready for kick off
	if (!player->team->match->GameOn())
	{
		player->Steering()->SetTarget(player->HomeRegion()->center);
	}
}

void Wait::Update(FieldPlayer* player)
{    
	if(player->hasPossession)
		player->GetFSM()->ChangeState(InPossession::Instance());

	//if the player has been jostled out of position, get back in position  
	if (!player->AtTarget())
	{
		player->Steering()->ArriveOn();

		return;
	}

	else
	{
		player->Steering()->ArriveOff();

		player->physicsObject.body->SetLinearVelocity(b2Vec2(0,0));
	}

	//if this player's team is controlling AND this player is not the attacker
	//AND is further up the field than the attacker he should request a pass.
	if (player->team->InControl() &&
		(!player->isControllingPlayer()) &&
		player->isAheadOfAttacker() )
	{
		player->team->RequestPass(player);

		return;
	}

	if (player->team->match->GameOn())
	{
		//if the ball is nearer this player than any other team member  AND
		//there is not an assigned receiver AND neither goalkeeper has
		//the ball, go chase it
		if (player->isClosestTeamMemberToBall() &&
			!player->team->match->GoalKeeperHasBall())
		{
			player->GetFSM()->ChangeState(ChaseBall::Instance());

			return;
		}
	} 
}

void Wait::Exit(FieldPlayer* player){}



//***************************************************************************** CHASEBALL

ChaseBall* ChaseBall::Instance()
{
	static ChaseBall instance;

	return &instance;
}


void ChaseBall::Enter(FieldPlayer* player)
{
	player->Steering()->SeekOn();

#ifdef PLAYER_STATE_INFO_ON
	debug_con << "Player " << player->ID() << " enters chase state" << "";
#endif
}

void ChaseBall::Update(FieldPlayer* player)                                     
{
	if(player->hasPossession)
	{
		player->tackling = false;
		player->GetFSM()->ChangeState(InPossession::Instance());
		return;
	}

	//if the ball is within kicking range the player changes state to KickBall.
	if (player->CloseEnoughToTackle())
		player->tackling = true;
	else
		player->tackling = false;


	//if the player is the closest player to the ball then he should keep
	//chasing it
	if (player->isClosestTeamMemberToBall() && !player->team->match->GoalKeeperHasBall())
	{
		player->Steering()->SetTarget(player->team->match->ball->GetPositionInPixels());

		return;
	}

	//if the player is not closest to the ball anymore, he should return back
	//to his home region and wait for another opportunity
	player->GetFSM()->ChangeState(ReturnToHomeRegion::Instance());
}


void ChaseBall::Exit(FieldPlayer* player)
{
	player->Steering()->SeekOff();
}


//***************************************************************************** CHASEBALL

InPossession* InPossession::Instance()
{
	static InPossession instance;

	return &instance;
}


void InPossession::Enter(FieldPlayer* player)
{
	//let the team know this player is controlling
	player->team->SetControllingPlayer(player);

	player->Steering()->SeekOn();

	if(player->team->Home())
	{
		player->Steering()->SetTarget(player->team->match->pitch->topGoal->GetPositionInPixels());
	}
	else
	{
		player->Steering()->SetTarget(player->team->match->pitch->bottomGoal->GetPositionInPixels());
	}
}

void InPossession::Update(FieldPlayer* player)                                     
{
	if (!player->hasPossession)
	{
		player->GetFSM()->ChangeState(ReturnToHomeRegion::Instance());
		return;
	}

	if(player->team->Human())
	{
		player->FindSupport();
		return;
	}

	/* Attempt a shot at the goal */

	//if a shot is possible, this vector will hold the position along the 
	//opponent's goal line the player should aim for.
	b2Vec2    BallTarget;

	//the dot product is used to adjust the shooting force. The more
	//directly the ball is ahead, the more forceful the kick
	double power = 50; //player->ShotPower(); //2.8; //Prm.MaxShootingForce * dot;

	//if it is determined that the player could score a goal from this position
	//OR if he should just kick the ball anyway, the player will attempt
	//to make the shot
	if (player->team->CanShoot(player->team->match->ball->GetPositionInPixels(),
		power,
		BallTarget))
	{

		//add some noise to the kick. We don't want players who are 
		//too accurate! The amount of noise can be adjusted by altering
		//Prm.PlayerKickingAccuracy
		//BallTarget = AddNoiseToKick(player->Ball()->Pos(), BallTarget);

		//this is the direction the ball will be kicked in
		b2Vec2 KickDirection = BallTarget - player->team->match->ball->GetPositionInPixels();

		KickDirection.Normalize();
		player->team->match->ball->Pass(KickDirection, player->ShotPower());

		//change state   
		player->GetFSM()->ChangeState(Wait::Instance());

		player->FindSupport();
	}

	/* Attempt a pass to a player */

  //if a receiver is found this will point to it
  Player* receiver = NULL;

  power = 50; //Prm.MaxPassingForce * dot;
  
  //test if there are any potential candidates available to receive a pass
  if (player->isThreatened()  &&
      player->team->FindPass(player,
                              receiver,
                              BallTarget,
                              power,
                              10))
  {     
    //add some noise to the kick
    //BallTarget = AddNoiseToKick(player->Ball()->Pos(), BallTarget);

	  b2Vec2 KickDirection = BallTarget - player->team->match->ball->GetPositionInPixels();

	  KickDirection.Normalize();

	  player->team->match->ball->Pass(KickDirection, player->PassPower());

    #ifdef PLAYER_STATE_INFO_ON
    debug_con << "Player " << player->ID() << " passes the ball with force " << power << "  to player " 
              << receiver->ID() << "  Target is " << BallTarget << "";
    #endif

    
    //let the receiver know a pass is coming 
    Dispatcher->DispatchMsg(player,
                            receiver,
                            Msg_ReceiveBall,
                            &BallTarget);                            
   

    //the player should wait at his current position unless instruced
    //otherwise  
    player->GetFSM()->ChangeState(Wait::Instance());

    player->FindSupport();

    return;
  }
  else
	  player->FindSupport();
}

void InPossession::Exit(FieldPlayer* player)
{
	player->Steering()->SeekOff();
}


//*****************************************************************************SUPPORT ATTACKING PLAYER

SupportAttacker* SupportAttacker::Instance()
{
	static SupportAttacker instance;

	return &instance;
}


void SupportAttacker::Enter(FieldPlayer* player)
{
	player->Steering()->ArriveOn();

	player->Steering()->SetTarget(player->team->GetSupportSpot());
}

void SupportAttacker::Update(FieldPlayer* player)                                     
{
	//if his team loses control go back home
	if (!player->team->InControl())
	{
		player->GetFSM()->ChangeState(ReturnToHomeRegion::Instance()); return;
	} 

	if(player->team->match->BallIsFree() && player->isClosestTeamMemberToBall())
	{
		player->GetFSM()->ChangeState(ChaseBall::Instance());

		return;
	}

	//if the best supporting spot changes, change the steering target
	if (!(player->team->GetSupportSpot() == player->Steering()->Target()))
	{    
		player->Steering()->SetTarget(player->team->GetSupportSpot());

		player->Steering()->ArriveOn();
	}

	//if this player has a shot at the goal AND the attacker can pass
	//the ball to him the attacker should pass the ball to this player
	if( player->team->CanShoot(player->GetPositionInPixels(), 30))
	{
		player->team->RequestPass(player);
	}


	//if this player is located at the support spot and his team still have
	//possession, he should remain still and turn to face the ball
	if (player->AtTarget())
	{
		player->Steering()->ArriveOff();

		player->physicsObject.body->SetLinearVelocity(b2Vec2(0,0));

		//if not threatened by another player request a pass
		if (!player->isThreatened())
		{
			player->team->RequestPass(player);
		}
	}
}


void SupportAttacker::Exit(FieldPlayer* player)
{
	//set supporting player to null so that the team knows it has to 
	//determine a new one.
	player->team->SetSupportingPlayer(NULL);

	player->Steering()->ArriveOff();
}

//************************************************************************     RECEIVEBALL

ReceiveBall* ReceiveBall::Instance()
{
	static ReceiveBall instance;

	return &instance;
}


void ReceiveBall::Enter(FieldPlayer* player)
{
	//let the team know this player is receiving the ball
	player->team->SetReceiver(player);

	//this player is also now the controlling player
	player->team->SetControllingPlayer(player);

	//there are two types of receive behavior. One uses arrive to direct
	//the receiver to the position sent by the passer in its telegram. The
	//other uses the pursuit behavior to pursue the ball. 
	//This statement selects between them dependent on the probability
	//ChanceOfUsingArriveTypeReceiveBehavior, whether or not an opposing
	//player is close to the receiving player, and whether or not the receiving
	//player is in the opponents 'hot region' (the third of the pitch closest
	//to the opponent's goal
	const double PassThreatRadius = 70.0;

	if (( player->InHotRegion() || RandFloat() < 0.5) &&
		!player->team->isOpponentWithinRadius(player->GetPositionInPixels(), PassThreatRadius))
	{
		player->Steering()->ArriveOn();

	}
	else
	{
		player->Steering()->PursuitOn();
	}
}

void ReceiveBall::Update(FieldPlayer* player)
{
	//if the ball comes close enough to the player or if his team lose control
	//he should change state to chase the ball
	if (player->hasPossession)
	{
		player->GetFSM()->ChangeState(InPossession::Instance());

		return;
	}  

	if(player->team->match->BallIsFree() && player->isClosestTeamMemberToBall())
	{
		player->GetFSM()->ChangeState(ChaseBall::Instance());

		return;
	}

	if (player->Steering()->PursuitIsOn())
	{
		player->Steering()->SetTarget(player->team->match->ball->GetPositionInPixels());
	}

	//if the player has 'arrived' at the steering target he should wait and
	//turn to face the ball
	if (player->AtTarget())
	{
		player->Steering()->ArriveOff();
		player->Steering()->PursuitOff();  
		player->physicsObject.body->SetLinearVelocity(b2Vec2(0,0));
	} 
}

void ReceiveBall::Exit(FieldPlayer* player)
{
	player->Steering()->ArriveOff();
	player->Steering()->PursuitOff();

	player->team->SetReceiver(NULL);
}