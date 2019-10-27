#ifndef _j1UI_H
#define _j1UI_H

#include "j1Module.h"
#include "j1App.h"
#include "SDL/include/SDL_rect.h"

enum TransitionDirection {
	STATIC,
	OPEN,
	CLOSE
};

class j1UI : public j1Module
{
public:

	j1UI();

	virtual ~j1UI();

	bool Awake(pugi::xml_node& config);

	bool Start();

	//bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void LevelTransition();

	void ResetTransition(TransitionDirection state = CLOSE);

public:
	//transition variables
	int transition_time = 5;
	int transition_moment = 0;
	float transition_speed = 0;
	TransitionDirection direction = CLOSE;
	SDL_Rect* camera;
	SDL_Rect left_square;
	SDL_Rect right_square;
	bool loaded = false;
	bool transition = false;
	int DeltaTime = 0;
};

#endif // !_j1UI_H

