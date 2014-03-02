#include "ContactListener.h"
#include "GameComponent.h"

void ContactListener::BeginContact(b2Contact* contact)
{
	  //check for user data
      void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	  void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

      if (bodyUserDataA && bodyUserDataB)
	  {
		  // Get the Game Component associated with the body
		  GameComponent* componentA  = static_cast<GameComponent*>(bodyUserDataA);
		  GameComponent* componentB  = static_cast<GameComponent*>(bodyUserDataB);
		  // Tell each component about the collision
		  componentA->BeginContact(contact, componentB);
		  componentB->BeginContact(contact, componentA);
	  }
}

void ContactListener::EndContact(b2Contact* contact)
{
	 //check for user data
      void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	  void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

      if (bodyUserDataA && bodyUserDataB)
	  {
		  // Get the Game Component associated with the body
		  GameComponent* componentA  = static_cast<GameComponent*>(bodyUserDataA);
		  GameComponent* componentB  = static_cast<GameComponent*>(bodyUserDataB);
		  // Tell each component about the collision
		  componentA->EndContact(contact, componentB);
		  componentB->EndContact(contact, componentA);
	  }
}
