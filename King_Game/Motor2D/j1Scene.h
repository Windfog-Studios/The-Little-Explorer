#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1App.h"

#define CAMERA_SPEED 10

struct SDL_Texture;

enum Map
{
	NO_MAP = 0,
	LEVEL_1,
	LEVEL_2,
};

enum TransitionDirection {
	STATIC,
	OPEN,
	CLOSE
};

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void Reset_Camera();

	void ResetLevel();

	void LevelChange(Map unloading_map, Map loading_map);

	void LevelTransition();

public:
	//edges
	int left_edge;
	int right_edge;
	int top_edge;
	int bottom_edge;

	//levels
	Map current_level;
	Map want_to_load = NO_MAP;

	//player initial position
	int	player_x_position;
	int	player_y_position;

	p2SString song;
	bool transition = false;
private:
	//transition variables
	int transition_time = 5;
	int transition_moment = 0;
	TransitionDirection direction = CLOSE;
	SDL_Rect* camera;
	SDL_Rect left_square;
	SDL_Rect right_square;
};

#endif // __j1SCENE_H__