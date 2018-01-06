#ifndef RAYZAPPER_WORLDMANAGER_H
#define RAYZAPPER_WORLDMANAGER_H

#include "ActorBehavior.h"

class WorldManager
{
public:
	WorldManager();
	~WorldManager();
private:
	GameActor *m_Player;
};

#endif