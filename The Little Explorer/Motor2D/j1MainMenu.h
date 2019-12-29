#ifndef _j1MAIN_MENU_H_
#define _j1MAIN_MENU_H
#include "j1Module.h"

struct SDL_Texture;
class GuiImage;
class GuiText;
class GuiButton;
class GuiInputText;
enum class Menu;

class j1MainMenu : public j1Module
{
public:
	j1MainMenu();
	~j1MainMenu();

	bool Start();
	bool CleanUp();

	void OnEvent(j1UI_Element* element, FocusEvent event);
	void CreateMainScreen();
	void CreateSettingsScreen();
	void CreateCreditsScreen();

private:
	Menu visible_menu;
	int window_width;
	int window_height;
	bool fullscreen;
	SDL_Texture* main_menu_background_tex = nullptr;
	SDL_Texture* main_title_tex = nullptr;
};

#endif // !_j1MAIN_MENU_H_

