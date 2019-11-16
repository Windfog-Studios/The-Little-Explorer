#ifndef _j1ENTITY_H
#define _j1ENTITY_H
#include "j1EntityManager.h"

struct SDL_Texture;
struct Collider;

class j1Entity : public j1EntityManager
{
public:
	j1Entity(EntityType type);
	virtual ~j1Entity();

	virtual bool Awake(pugi::xml_node&) { return true; };
	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };
	virtual void OnCollision(Collider* c1, Collider* c2) {};

public:
	fPoint position;
	EntityType type;
	SDL_Texture* texture = nullptr;
};
#endif // !_j1ENTITY_H

