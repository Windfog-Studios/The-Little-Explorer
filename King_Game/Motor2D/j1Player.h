#ifndef _j1PLAYER_H_
#define _j1PLAYER_H_

#include "j1Module.h"

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"

class j1Player : public j1Module {
	
	j1Player();

	virtual ~j1Player();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

public:
	//SDL_Texture* player_tex;
	p2SString		 folder;
};

#endif // !_j1PLAYER_H_

