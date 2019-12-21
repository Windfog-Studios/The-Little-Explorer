#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1App.h"
#include "j1Timer.h"

#define CAMERA_SPEED 200
#define CAMERA_CENTER_MARGIN 30

struct SDL_Texture;
class GuiImage;
class GuiText;
class GuiButton;
class GuiInputText;

enum Map
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
	bool Awake(pugi::xml_node& config);
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

	//void ResetCamera(int kind_of_reset);
	//void ResetLevel();
	void LevelChange(Map unloading_map, Map loading_map);
	void CreatePauseMenu();
	void OnEvent(j1UI_Element*, FocusEvent);

	bool Save(pugi::xml_node& data) const;
	bool Load(pugi::xml_node& data);

public:
	//edges
	SDL_Rect camera_frame;
	int left_edge;
	int right_edge;
	int top_edge;
	int bottom_edge;

	int camera_frame_x_margin;
	int camera_frame_y_margin;

	bool blocked_camera;
	bool showing_menu;

	//UI Info
	int score;
	int time;
	j1Timer timer;
	GuiText* time_text;

	//levels
	Map current_level;

	p2SString song;
	p2SString folder;

	int camera_margin;
	iPoint initial_camera_position;

	//pause menu
	GuiImage* menu_background;
	GuiImage* pause_text;

private:
};

#endif // __j1SCENE_H__