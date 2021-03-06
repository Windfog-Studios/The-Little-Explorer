#ifndef _j1CONSOLE_H_
#define _j1CONSOLE_H_
#include "j1Module.h"
#include "GuiText.h"
#include "SDL_image/include/SDL_image.h"

class GuiInputText;
struct SDL_Rect;
class j1Command;

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
	void OnCommand(p2SString command);

	void AddLogText(p2SString new_text);

	void CreateInterface();
	void DestroyInterface();

	void CreateCommand(const char* command, j1Module* callback, const char* explanation = "No explanation given");
	void CheckCommand(p2SString command);

public:
	GuiInputText* command_input;
	p2List<p2SString> log_record;
	p2List<GuiText*> on_screen_log;
	p2List<p2SString> input_commands;
	p2List<j1Command*> commands;
	p2List_item<p2SString>* current_consulting_command;
	int l;
	bool isVisible;
	SDL_Rect log_box;
	SDL_Rect command_background;
	bool CleanUpStarted;
};

#endif // !_j1CONSOLE_H_

