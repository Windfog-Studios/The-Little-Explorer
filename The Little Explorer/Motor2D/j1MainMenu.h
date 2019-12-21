#ifndef _j1MAIN_MENU_H_
#define _j1MAIN_MENU_H
#include "j1Module.h"

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

	void OnEvent(j1UI_Element* element, FocusEvent event) {};

private:

};

#endif // !_j1MAIN_MENU_H_

