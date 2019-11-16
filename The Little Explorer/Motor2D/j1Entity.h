#ifndef _j1ENTITY_H
#define _j1ENTITY_H
#include "j1EntityManager.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;
struct Collider;

#define COLLIDER_MARGIN 25

enum EntityState {
	IDLE,
	JUMP,
	RUN_FORWARD,
	RUN_BACKWARD,
	FALL,
	ATTACK,
	SLIDE_FORWARD,
	SLIDE_BACKWARD,
	CROUCH_UP,
	CROUCH_DOWN,
};

class j1Entity : public j1EntityManager
{
public:

	j1Entity(EntityType type);
	virtual ~j1Entity();

	virtual bool Awake(pugi::xml_node&);
	virtual bool Start();
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp();
	virtual void OnCollision(Collider* c1, Collider* c2) {};

public: 

	fPoint position;
	fPoint current_speed;
	int initial_x_position;
	int initial_y_position;
	int health;
	bool grounded = false;

	Collider* collider = nullptr;
	Collider* raycast = nullptr;
	Collider* last_collider = nullptr;
	
	uint die_fx;

	p2SString die_fx_path;

	Animation* current_animation;

	EntityType type;
	EntityState state;
	SDL_Texture* texture = nullptr;
	SDL_RendererFlip flip;
	bool visible = true;
};
#endif // !_j1ENTITY_H

