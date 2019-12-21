#include "j1MainMenu.h"
#include "j1Render.h"
#include "j1App.h"
#include "j1Gui.h"

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

	App->gui->Start();
	/*

	window = (GuiImage*)App->gui->CreateUIElement(UI_Type::IMAGE, this);
	window->Init({ 325,350 }, { 0, 512, 483, 512 });

	window_text = (GuiText*)App->gui->CreateUIElement(UI_Type::TEXT, this, window, false);
	window_text->Init({ 520,405 }, "Window Title");

	button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, window, false);
	button->Init({ 450, 700 }, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, "Button");

	inputText = (GuiInputText*)App->gui->CreateUIElement(UI_Type::INPUT_TEXT, this, window, false);
	inputText->Init({ 400,550 }, "Editable Text", { 488, 569, 344, 61 });

	*/

	start_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this,nullptr,false,true);
	start_button->Init({ 725, 400 }, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, "Start");

	continue_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	continue_button->Init({ 725, 472 }, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, "Continue");

	settings_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	settings_button->Init({ 725, 545 }, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, "Settings");

	credits_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	credits_button->Init({ 60, 620 }, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, "Credits");

	exit_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	exit_button->Init({ 725, 620 }, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, "Exit");

	return ret;
}

bool j1MainMenu::PostUpdate() {
	bool ret = true;
	App->render->DrawQuad({ 0,0,1024,768 }, 100, 200, 100, 255);
	return ret;
}
