#include "raylib.h"

#define MAX_ENTITIES 100

typedef struct {
	Vector3 position;
	Vector3 previous_position;
	Vector3 velocity;
} TransformComponent;

typedef struct {
	int id;
	TransformComponent transform;
} Entity;


typedef struct {
	int entCount;
	Entity* entites[MAX_ENTITIES];
} EntityManager;

void initEntityManger(EntityManager* em, int maxEntities);

Entity* addEntity(EntityManager* em);