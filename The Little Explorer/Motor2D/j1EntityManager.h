#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_

#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"

class j1Entity;
struct SDL_Texture;

enum class EntityType
{
	PLAYER,
	WALKING_ENEMY,
	FLYING_ENEMY,
	TRAP,
	PARTICLES,
	UNKNOWN
};

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	virtual bool Awake(pugi::xml_node&);
	virtual bool Start();
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp();
	virtual void OnCollision(Collider* c1, Collider* c2);
	virtual bool Load(pugi::xml_node& data);
	virtual bool Save(pugi::xml_node& data);

	j1Entity* CreateEntity(EntityType type, int position_x, int position_y);
	virtual void DestroyEntity(j1Entity* delete_entity);
public:

	p2List<j1Entity*> entities;
	pugi::xml_node config_data;

	SDL_Texture* walking_enemy_tex;
	SDL_Texture* flying_enemy_tex;

	j1Player* player;
	j1WalkingEnemy* walking_enemy;
	//j1FlyingEnemy* exampleFlyingEnemy;
};

#endif // !_ENTITY_MANAGER_H_

