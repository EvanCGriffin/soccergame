#pragma once

#include <Year3Engine\StateMachine.h>

class Team;

class PrepareForKickOff : public State<Team>
{ 
private:
  
  PrepareForKickOff(){}

public:

  //this is a singleton
  static PrepareForKickOff* Instance();
  
  void Enter(Team* team);

  void Update(Team* team);

  void Exit(Team* team);

  bool OnMessage(Team*, const Telegram&) { return false; }
};

//------------------------------------------------------------------------
class Defending : public State<Team>
{ 
private:
  
  Defending(){}

public:

    //this is a singleton
  static Defending* Instance();

  void Enter(Team* team);

  void Update(Team* team);

  void Exit(Team* team);

  bool OnMessage(Team*, const Telegram&) { return false; }
};

//------------------------------------------------------------------------
class Attacking : public State<Team>
{ 
private:
  
  Attacking(){}

public:

  //this is a singleton
  static Attacking* Instance();

  void Enter(Team* team);

  void Update(Team* team);

  void Exit(Team* team);

  bool OnMessage(Team*, const Telegram&) { return false; }
};