#include "Player.h"

Player::Player()
{
	
}

Player::~Player()
{
	for (unsigned int c = 0; c < myModifiers.size(); c++)
		delete myModifiers[c];
}

void Player::init(std::string pName, glm::vec3 initPos, bool isLocal)
{
	cam = CameraInput::getCam();
	i = Input::getInput();

	name = pName;
	pos = initPos;
	vel = glm::vec3(0, 0, 0);
	dir = cam->getDir();

	isLocalPlayer = isLocal;

	rotate(0, -3.141592654f, 0);
}

void Player::setName(std::string newName)
{
	name = newName;
}

void Player::setGoalPos(glm::vec3 newPos)
{
	goalpos = newPos;
	oldPos = pos;
	goalTimer = 0.0f;
}

void Player::setGoalDir(glm::vec3 newDir)
{
	goaldir = newDir;
	oldDir = dir;
}

void Player::movePlayer(float dt, glm::vec3 oldDir, bool freecam, bool specingThis)
{
	/*
	
	L�gg till matematik f�r kollisioner h�r!!
	
	*/
	glm::vec3 playerVel = vel*role.getMovementSpeed();
	pos += playerVel * dt; //Here we will also include external forces
	if (freecam == false || specingThis == true)
	{
		cam->setCam(pos);
		rotatePlayer(oldDir, dir);
	}
}

void Player::modifiersGetData(float dt)
{
	for (unsigned int c = 0; c < myModifiers.size(); c++)
	{
		int msg = myModifiers[c]->getData(dt);
		if (msg == 1)
		{
			delete myModifiers[c];
			myModifiers[c] = myModifiers[myModifiers.size() - 1];
			myModifiers.pop_back();
			c--;
		}
	}
}

void Player::modifiersSetData(float dt)
{
	for (unsigned int c = 0; c < myModifiers.size(); c++)
	{
		int msg = myModifiers[c]->setData(dt);
		if (msg == 1)
		{
			delete myModifiers[c];
			myModifiers[c] = myModifiers[myModifiers.size() - 1];
			myModifiers.pop_back();
			c--;
		}
	}
}

void Player::cleanseModifiers(bool stickies)
{
	for (int c = myModifiers.size() - 1; c >= 0 ; c++)
	{
		if (!stickies)
		{
			if (!myModifiers[c]->isSticky())
			{
				delete myModifiers[c];
				myModifiers[c] = myModifiers[myModifiers.size() - 1];
				myModifiers.pop_back();
			}
		}
		else
		{
			delete myModifiers[c];
			myModifiers[c] = myModifiers[myModifiers.size() - 1];
			myModifiers.pop_back();
		}
	}
}

bool Player::removeSpecificModifier(MODIFIER_TYPE mt)
{
	for (unsigned int c = 0; c < myModifiers.size(); c++)
	{
		if (myModifiers[c]->getType() == mt)
		{
			delete myModifiers[c];
			myModifiers[c] = myModifiers[myModifiers.size() - 1];
			myModifiers.pop_back();
			return true;
		}
	}
	return false;
}

PLAYERMSG Player::update(float dt, bool freecam, bool spectatingThisPlayer, bool spectating)
{
	PLAYERMSG msg = NONE;

	modifiersGetData(dt);

	vec3 olddir = cam->getDir();
	if (isLocalPlayer) // even if we are the local player we can be dead and spectating some one
	{
		if (i->justPressed(GLFW_KEY_ENTER))
		{
			if (lockControls)
				lockControls = false;
			else
				lockControls = true;
		}

		if (!lockControls)
		{
			//move camera to where we are looking.
			//if freecam is true the cam can move on its own
			if(spectating == false)
				cam->update(dt, freecam);

			//If freecam or spectating dont take player move input
			if (freecam == false)
			{
				dir = cam->getDir();
				bool stop = true;
				vec2 tempvec = vec2(0, 0);

				if (i->getKeyInfo(GLFW_KEY_W))
				{
					tempvec = normalize(vec2(dir.x, dir.z));
					if (grounded)
					{
						vel.x = tempvec.x;
						vel.z = tempvec.y;
						stop = false;
					}
				}

				if (i->getKeyInfo(GLFW_KEY_S))
				{
					tempvec = -normalize(vec2(dir.x, dir.z));
					if (grounded)
					{
						vel.x = tempvec.x;
						vel.z = tempvec.y;
						stop = false;
					}
				}

				if (!(i->getKeyInfo(GLFW_KEY_A) && i->getKeyInfo(GLFW_KEY_D)))
				{
					if (i->getKeyInfo(GLFW_KEY_A))
					{
						vec3 left = cross(vec3(0, 1, 0), dir);
						tempvec = normalize(vec2(left.x, left.z));
						if (grounded)
						{
							vel.x += tempvec.x;
							vel.z += tempvec.y;
							stop = false;
							tempvec = vec2(vel.x, vel.z);
							if (glm::length(tempvec) > 0)
							{
								tempvec = normalize(tempvec);
								vel.x = tempvec.x;
								vel.z = tempvec.y;
							}
						}
					}

					if (i->getKeyInfo(GLFW_KEY_D))
					{
						vec3 right = cross(dir, vec3(0, 1, 0));
						tempvec = glm::normalize(vec2(right.x, right.z));
						right = glm::normalize(right);
						if (grounded)
						{
							vel.x += tempvec.x;
							vel.z += tempvec.y;
							stop = false;
							tempvec = vec2(vel.x, vel.z);
							
							if (glm::length(tempvec) > 0)
							{
								tempvec = normalize(tempvec);
								vel.x = tempvec.x;
								vel.z = tempvec.y;
							}
						}
					}
				}

				if (stop)
				{
					if (grounded)
					{
						vel.x = 0;
						vel.z = 0;
					}
				}

				Special* mobility = role.getMobilityAbility();
				if (i->justPressed(mobility->getActivationKey()))
				{
					if (mobility->allowedToActivate(this))
					{
						if (mobility->getActivationCost() < role.getSpecialMeter())
						{
							role.setSpecialMeter(role.getSpecialMeter() - mobility->getActivationCost());
							msg = MOBILITYUSE;
						}
					}
				}

				if (i->justPressed(GLFW_KEY_SPACE))
				{
					if (grounded)
					{
						vel.y = role.getJumpHeight();
						grounded = false;
					}
				}

				role.update(dt);		//Temp;

				if (i->justPressed(GLFW_KEY_R))
				{
					role.getCurrentWeapon()->reload();
				}

				if (i->justPressed(GLFW_KEY_1))
				{
					role.swapWeaponLocal(0);
					msg = WPNSWITCH;
				}

				if (i->justPressed(GLFW_KEY_2))
				{
					role.swapWeaponLocal(1);
					msg = WPNSWITCH;
				}

				if (i->getKeyInfo(GLFW_MOUSE_BUTTON_LEFT))		//Temp
				{
					if (role.getCurrentWeapon()->shoot())
						msg = SHOOT;
				}

				if (i->justPressed(GLFW_KEY_Q))
				{
					Consumable* c = role.getConsumable();
					if (c->use())
						msg = USEITEM;
				}

				if (i->justPressed(GLFW_KEY_E))
				{
					/*
					Add logic (in role) that checks against the applicable special and other conditions
					*/
					if (role.getSpecialAbility()->allowedToActivate(this))
					{
						msg = SPECIALUSE;
					}
				}

				if (i->justPressed(GLFW_KEY_Z))					//Temp?
				{
					if (GetSoundActivated() == 0 && GetInitialized() == 0)
					{
						InitSound(CreateSound(), 1);
						GetSound()->playMusic(mainMenu);
					}
					else if (GetInitialized())
					{
						GetSound()->enableSounds();
					}
				}

				if (i->justPressed(GLFW_KEY_O))
				{
					msg = DEATH;
					isDead = true;
					respawnTimer = respawnTime;
				}
				if (i->justPressed(GLFW_KEY_M))
					role.setSpecialMeter(100.0f);
			} // end of player input
		} // end of lock control check

		if (role.getHealth() == 0 && !isDead)
		{
			isDead = true;
			msg = DEATH;
			respawnTimer = respawnTime;
			vel = glm::vec3(0, 0, 0);
		}

		if (isDead && respawnTimer != 0.0f)
		{
			respawnTimer -= dt;
			if (respawnTimer < FLT_EPSILON)
			{
				respawnTimer = 0.0f;
				msg = PLAYERRESPAWN;
				//role.returnToLife();
			}
		}

	} // end of local player check
	else
	{
		/*
		THIS IS NOT THE LOCAL PLAYER
		*/
		goalTimer += dt;
		float t = goalTimer / interpolationTick;

		if (t > 1.0f)
			t = 1.0f;

		pos = (oldPos * (1.0f - t)) + (goalpos * t);
		glm::vec3 prev = dir;
		dir = (oldDir * (1.0f - t)) + (goaldir * t);

		rotatePlayer(prev, dir);

		if (role.getHealth() == 0)
		{
			isDead = true;
			vel = glm::vec3(0, 0, 0);
		}
	}

	modifiersSetData(dt);

	/*
	Kind of ugly, but I put it here so it interacts properly with movement-changing modifiers
	*/
	if (isLocalPlayer)
		movePlayer(dt, olddir, freecam, spectatingThisPlayer); //Move the player regardless of control lock

	if (spectatingThisPlayer == true)
	{
		cam->setCam(pos, dir);
	}

	clearCollisionNormals(); //Doesn't actually clear the array, just manually sets size to 0. This is to speed things up a little.

	worldMat[0].w = pos.x;
	worldMat[1].w = pos.y-0.6f;
	worldMat[2].w = pos.z;

	return msg;
}

void Player::rotatePlayer(vec3 olddir, vec3 newdir)
{
	float angle = atan2(newdir.x, newdir.z) - atan2(olddir.x, olddir.z);
	rotate(0, -angle, 0);
}

Weapon* Player::getPlayerCurrentWeapon()
{
	return role.getCurrentWeapon();
}

void Player::switchWpn(WEAPON_TYPE ws, int swapLoc)
{
	role.swapWeapon(ws, swapLoc);
}

void Player::hitByBullet(Bullet* b, int newHPtotal)
{
	/*
	This is where we will extract additional modifiers from the Bullet, when applicable.
	*/
	if (newHPtotal == -1) //We are actually taking damage on the server now
	{
		int dmg = b->getDamage();
		role.takeDamage(dmg);
	}
	else //We are on a client, and thus are only interested on our HP on the server
	{
		role.setHealth(newHPtotal);
	}

	//If hacking dart -> modifier on player
}

void Player::hitByEffect(Effect* e, int newHPtotal)
{
	/*
	Big ol' switch case to identify which effect is hitting us and what we should do about it
	*/
	if (newHPtotal == -1) //This is the server, dealing damage to the player
	{
		int dmg = e->getDamage();
		role.takeDamage(dmg);
	}
	else //Hello I'm the client. I accept my new HP.
	{
		role.setHealth(newHPtotal);
	}
}

void Player::applyGravity(Physics* p, float dt)
{
	p->addGravity(vel, dt);
	/*if (pos.y <= -10.0f) // Temp code for floor lol
	{
		vel.y = 0.0f;
		pos.y = -10.0f;
		grounded = true;
	}*/
}

void Player::addModifier(MODIFIER_TYPE mt)
{
	Modifier* m = nullptr;
	switch (mt)
	{
	case LIGHTWALLCONTROLLOCK:
		removeSpecificModifier(LIGHTWALLCONTROLLOCK);
		m = new LightWallLockedControls();
		myModifiers.push_back(m);
		break;
	}
	myModifiers[myModifiers.size() - 1]->init(this);
}

void Player::setRole(Role role)
{
	cleanseModifiers(true);
	this->role = role;
	this->role.chooseRole(TRAPPER);
}

void Player::respawn(glm::vec3 respawnPos)
{
	pos = respawnPos;
	vel = glm::vec3(0, 0, 0);
	if (isLocalPlayer)
		cam->setCam(pos, dir);
	worldMat[0].w = pos.x;
	worldMat[1].w = pos.y - 0.6f;
	worldMat[2].w = pos.z;
	isDead = false;
	cleanseModifiers();
	role.returnToLife();
}

void Player::healing(int amount)
{
	role.setHealth(role.getHealth()+amount);
}