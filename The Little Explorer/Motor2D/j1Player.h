#ifndef _j1PLAYER_H_
#define _j1PLAYER_H_

#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Audio.h"
#include "p2Vec2.h"
#include "j1Entity.h"

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
/*
enum Player_States {
	IDLE, 
	RUN_FORWARD,
	RUN_BACKWARD,
	JUMP,
	//JUMP_FORWARD,
	//JUMP_BACKWARD,
	FALL,
	SLIDE_FORWARD,
	SLIDE_BACKWARD,
	WALK,
	CROUCH_DOWN,
	CROUCH_UP
};
*/
class j1Player : public j1Entity {
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

	void MovementControl(float dt);

	bool Save(pugi::xml_node& data) const;
	bool Load(pugi::xml_node& data);

	bool LoadAnimations();

		
private:


public:

	SDL_Texture* player_tex;
	p2SString folder;
	Player_Input player_input;

	//positions
	iPoint position;
	iPoint lastPosition;

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
	p2List<Animation*> animations;
	pugi::xml_document animation_doc;

	//fx
	p2SString die_fx_path;
	p2SString jump_fx_path;

	uint jump_fx;
	uint die_fx;

	EntityState last_state;

	Collider* collider = nullptr;
	Collider* raycast = nullptr;
	Collider* last_collider = nullptr;

	//movement
	float jumpImpulse;
	float doubleJumpImpulse;
	float max_running_speed;
	float acceleration;
	float side_speed;
	bool can_double_jump = true;
	bool can_go_right = true;
	bool can_go_left = true;
	float gravity;
	bool god = false;
	bool grounded = false;

	bool visible = true;
	bool controls_blocked = false;
};

#endif // !_j1PLAYER_H_

