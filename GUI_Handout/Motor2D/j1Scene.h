#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class GuiImage;
class GuiText;
class GuiButton;
class GuiInputText;

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

	void OnFocus(j1UI_Element* element);

private:
	SDL_Texture*	debug_tex;
	GuiImage*		banner;
	GuiText*		text;
	GuiImage*		window;
	GuiText*		window_text;
	GuiButton*		button;
	GuiText*		button_text;
	GuiInputText*	inputText;
};

#endif // __j1SCENE_H__