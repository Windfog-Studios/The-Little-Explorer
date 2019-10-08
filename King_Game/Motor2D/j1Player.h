#ifndef _j1PLAYER_H_
#define _j1PLAYER_H_

#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"
//#include "PugiXml/src/pugixml.hpp"
//#include "p2List.h"

struct SDL_Texture;

struct Player_Input {
	bool pressing_W;
	bool pressing_A;
	bool pressing_S;
	bool pressing_D;
};

class j1Player : public j1Module {
public:
	j1Player();

	virtual ~j1Player();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

private:

	Animation idle;
	Animation jump;
	Animation run;
	Animation walk;
	Animation crouch;
	Animation slide;

public:

	SDL_Texture* player_tex;
	p2SString folder;
	Player_Input player_input;
	p2Point<int> position;

};

#endif // !_j1PLAYER_H_

