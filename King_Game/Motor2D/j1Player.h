#ifndef _j1PLAYER_H_
#define _j1PLAYER_H_

#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
//#include "PugiXml/src/pugixml.hpp"
//#include "p2List.h"

struct SDL_Texture;

struct Player_Input {
	bool pressing_W;
	bool pressing_A;
	bool pressing_S;
	bool pressing_D;
	bool pressing_F;
};

enum Player_States {
	IDLE, 
	RUN_FORWARD,
	RUN_BACKWARD,
	JUMP,
	SLIDE,
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

private:


public:

	SDL_Texture* player_tex;
	p2SString folder;
	Player_Input player_input;
	p2Point<int> position;

	//animations
	Animation idle;
	Animation jump;
	Animation run;
	Animation walk;
	Animation crouch_down;
	Animation crouch_up;
	Animation slide;
	Animation* current_animation;

	Player_States state;

	SDL_RendererFlip flip;

};

#endif // !_j1PLAYER_H_

