#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

#define CAMERA_SPEED 10

struct SDL_Texture;

enum MapLoaded
{
	NO_MAP = 0,
	LEVEL_1,
	LEVEL_2,
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

public:
	
	int left_edge;
	int right_edge;
	int top_edge;
	int bottom_edge;
	MapLoaded current_level;
private:
};

#endif // __j1SCENE_H__