#include "j1MainMenu.h"
#include "j1Render.h"
#include "j1App.h"

j1MainMenu::j1MainMenu() : j1Module()
{
	name.create("main_menu");
}

j1MainMenu::~j1MainMenu() {}

bool j1MainMenu::Awake(pugi::xml_node& config) {
	bool ret = true;

	return ret;
}

bool j1MainMenu::Start() {
	bool ret = true;
	App->render->DrawQuad({ 0,0,640,480 }, 100, 100, 100, 255);
	return ret;
}

bool j1MainMenu::PostUpdate() {
	bool ret = true;
	//App->render->DrawQuad({ 0,0,640,480 }, 100, 100, 100, 255);
	return ret;
}
