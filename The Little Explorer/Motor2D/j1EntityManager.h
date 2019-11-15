#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_

#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"

enum class EntityType
{
	PLAYER,
	PARTICLES,
	ENEMIES,
	TRAPS,

	UNKNOWN
};
class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	bool Load(pugi::xml_node& data);
	bool Save(pugi::xml_node& data);


public:



};

#endif // !_ENTITY_MANAGER_H_

