#ifndef _j1PLAYER_H_
#define _j1PLAYER_H_

#include "j1Module.h"

class j1Player : public j1Module {
	
	j1Player();

	virtual ~j1Player();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool CleanUp();

public:

};

#endif // !_j1PLAYER_H_

