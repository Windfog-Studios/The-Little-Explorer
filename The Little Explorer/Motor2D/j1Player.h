#ifndef _j1PLAYER_H_
#define _j1PLAYER_H_

#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Audio.h"
#include "SDL/include/SDL.h"
#include "p2Vec2.h"

struct SDL_Texture;
struct Collider;

#define COLLIDER_MARGIN speed.x*1.5f

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
	//JUMP_FORWARD,
	//JUMP_BACKWARD,
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

	void MovementControl();

	bool Save(pugi::xml_node& data) const;
	bool Load(pugi::xml_node& data);

	bool LoadAnimations();

		
private:


public:

	SDL_Texture* player_tex;
	p2SString folder;
	Player_Input player_input;

	//positions
	int initial_x_position;
	int initial_y_position;
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

	Player_States state;
	Player_States last_state;

	SDL_RendererFlip flip;

	Collider* collider = nullptr;
	Collider* collider_copy;

	//movement
	fVec2 speed;
	float jumpImpulse;
	float gravity;
	float acceleration = 0.5;
	float max_x_speed = 5.0f;
	float air_movement = 5.0f;
	bool can_double_jump = true;
	bool can_go_right = true;
	bool can_go_left = true;
	bool god = false;
	bool grounded = false;

	bool visible = true;
	bool controls_blocked = false;
};

#endif // !_j1PLAYER_H_

