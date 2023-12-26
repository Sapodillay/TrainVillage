#include "raylib.h"
#include "EntityManager.h"

#include <stdlib.h>

void initEntityManger(EntityManager* em, int maxEntites)
{
	em->entCount = 0;
}

Entity* addEntity(EntityManager* em)
{

	Entity* entity = (Entity*)malloc(sizeof(Entity));
	em->entites[0] = entity;
	entity->id = 1;
	em->entCount++;
}
