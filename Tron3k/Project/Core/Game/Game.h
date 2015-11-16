#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "../../../Physics/Physics.h"

class Game
{
private:
	
	virtual void release();

	int max_con;
	Player** playerList;
	Physics* physics;

	void initPhysics();
public:

	Game();
	void init(int max_connections);

	Player* getPlayer(int conID);
	void createPlayer(Player* p, int conID);

	void update(float dt);
};

#endif