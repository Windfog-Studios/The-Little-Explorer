#ifndef _j1MAIN_MENU_H_
#define _j1MAIN_MENU_H
#include "j1Module.h"

struct SDL_Texture;
class GuiImage;
class GuiText;
class GuiButton;
class GuiInputText;

class j1MainMenu : public j1Module
{
public:
	j1MainMenu();
	~j1MainMenu();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate() { return true; };
	bool Update(float dt) { return true; };
	bool PostUpdate();
	bool CleanUp() { return true; };

	void OnEvent(j1UI_Element* element, FocusEvent event);
	void CreateMainScreen();
	void CreateSettingsScreen();
	void CreateCreditsScreen();

private:

	GuiImage* lives;
	GuiImage* background;
	GuiImage* menu_background;
	GuiImage* title;
	GuiImage* title2;
	GuiButton* start_button;
	GuiButton* continue_button;
	GuiButton* settings_button;
	GuiButton* credits_button;
	GuiButton* exit_button;
	GuiButton* go_back_button;
};

#endif // !_j1MAIN_MENU_H_

