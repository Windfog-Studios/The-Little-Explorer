#ifndef _j1CONSOLE_H_
#define _j1CONSOLE_H_
#include "j1Module.h"
#include "SDL_image/include/SDL_image.h"

class GuiInputText;
class GuiText;
struct SDL_Rect;

#define MAX_LOG_RECORD 100

class j1Console : public j1Module
{
public:
	j1Console();
	~j1Console();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void AddLogText(p2SString new_text);

	void CreateInterface();
	void DestroyInterface();

public:
	GuiInputText* command_input;
	GuiText** log_text;
	int l;
	bool isVisible;
	SDL_Rect log_box;
	SDL_Rect command_background;
};

#endif // !_j1CONSOLE_H_

