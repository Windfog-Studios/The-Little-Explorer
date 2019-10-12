#ifndef _j1PLAYER_H_
#define _j1PLAYER_H_

#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "p2Vec2.h"

struct SDL_Texture;
struct Collider;

struct Player_Input {
	bool pressing_W;
	bool pressing_A;
	bool pressing_S;
	bool pressing_D;
	bool pressing_F;
	bool pressing_space;
};

enum Player_States {
	IDLE, 
	RUN_FORWARD,
	RUN_BACKWARD,
	JUMP,
	FALL,
	SLIDE_FORWARD,
	SLIDE_BACKWARD,
	WALK,
	CROUCH_DOWN,
	CROUCH_UP
};

class j1Player : public j1Module {
public:
	j1Player();

	virtual ~j1Player();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

private:


public:

	SDL_Texture* player_tex;
	p2SString folder;
	Player_Input player_input;
	iPoint position;
	iPoint lastPosition;
	fVec2 v;


	//animations
	Animation idle;
	Animation jump;
	Animation run;
	Animation walk;
	Animation crouch_down;
	Animation crouch_up;
	Animation slide;
	Animation fall;
	Animation* current_animation;

	Player_States state;

	SDL_RendererFlip flip;
	Collider* collider = nullptr;
	Collider* collider_copy;

	float speed;

};

#endif // !_j1PLAYER_H_

