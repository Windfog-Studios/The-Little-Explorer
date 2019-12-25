#include "j1MainMenu.h"
#include "j1Render.h"
#include "j1App.h"
#include "j1Gui.h"
#include "GuiButton.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "j1Scene.h"
#include "j1Textures.h"
#include "j1Console.h"
#include "j1Window.h"

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
	
	App->gui->Start();

	CreateMainScreen();

	return ret;
}

bool j1MainMenu::PostUpdate() {
	bool ret = true;
	return ret;
}

void j1MainMenu::OnEvent(j1UI_Element* element, FocusEvent event) {

	if ((element->type == UI_Type::BUTTON) && (event == FocusEvent::CLICKED))
	{
		GuiButton* button = (GuiButton*) element;
		switch (button->action)
		{
		case ButtonAction::PLAY:
			if (App->map->Load("Level1.tmx") == true)
			{
				int w, h;
				uchar* data = NULL;
				if (App->map->CreateWalkabilityMap(w, h, &data))
					App->pathfinding->SetMap(w, h, data);
				RELEASE_ARRAY(data);
			}

			App->render->camera.x = App->scene->initial_camera_position.x;
			App->render->camera.y = App->scene->initial_camera_position.y;

			App->scene->camera_frame.x = -App->render->camera.x + App->scene->camera_frame_x_margin;
			App->scene->camera_frame.y = -App->scene->initial_camera_position.y + App->scene->camera_frame_y_margin;

			App->gui->DestroyAllGui();
			App->scene->visible_menu = Menu::NO_MENU;
			if (App->console->isVisible) App->console->DestroyInterface();
			App->scene->CreateScreenUI();
			break;

		case ButtonAction::CONTINUE:
			App->LoadGame();
			App->gui->DestroyAllGui();
			App->scene->visible_menu = Menu::NO_MENU;
			App->scene->CreateScreenUI();
		break;

		case ButtonAction::SETTINGS:
			App->gui->DestroyAllGui();
			CreateSettingsScreen();
		break;

		case ButtonAction::CREDITS:
			App->gui->DestroyAllGui();
			CreateCreditsScreen();
		break;

		case ButtonAction::GO_BACK:
			App->gui->DestroyAllGui();
			CreateMainScreen();
		break;

		case ButtonAction::QUIT:
			App->quit = true;
		break;

		default:
			break;
		}
	}
}

void j1MainMenu::CreateMainScreen() {
	SDL_Rect camera;
	camera = App->render->camera;
	_TTF_Font* principal_font = App->font->Load("fonts/Some Time Later.otf", 50);

	background = (GuiImage*)App->gui->CreateUIElement(UI_Type::IMAGE, this);
	background->Init({ 0,0 }, { 0,0,(int) App->win->width, (int)App->win->height });
	background->texture = App->tex->Load("sprites/UI/MainMenuBackground.png");

	/*
	title = (GuiImage*)App->gui->CreateUIElement(UI_Type::IMAGE, this);
	title->Init({ 160,120 }, { 0,0,700,200 });
	title->tex = App->tex->Load("sprites/UI/title.png");
	*/

	title2 = (GuiImage*)App->gui->CreateUIElement(UI_Type::IMAGE, this);
	title2->Init({ 250,180 }, { 165,2,378, 198 });
	title2->texture = App->tex->Load("sprites/UI/title2.png");
	
	start_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	start_button->Init({ 710, 392 }, { 6,547,200,72 }, { 206,547,200,72 }, { 206,547,200,72 }, "Start", ButtonAction::PLAY);
	
	continue_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	continue_button->Init({ 750, 470 }, { 8,622,196,71 }, { 206,622,196,71 }, { 206,622,200,72 }, "Continue", ButtonAction::CONTINUE);

	settings_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	settings_button->Init({ 710, 546 }, { 6,547,200,72 }, { 206,547,200,72 }, { 206,547,200,72 }, "Settings", ButtonAction::SETTINGS);

	exit_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	exit_button->Init({ 750, 626 }, { 8,622,196,71 }, { 206,622,196,71 }, { 206,622,200,72 }, "Exit", ButtonAction::QUIT);

	credits_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	credits_button->Init({ 60, 626 }, { 6,547,200,72 }, { 206,547,200,72 }, { 206,547,200,72 }, "Credits", ButtonAction::CREDITS);

}

void j1MainMenu::CreateSettingsScreen() {
	//TODO: create slider
	background = (GuiImage*)App->gui->CreateUIElement(UI_Type::IMAGE, this);
	background->Init({ 0,0 }, { 0,0,(int) App->win->width,(int) App->win->height });
	background->texture = App->tex->Load("sprites/UI/MainMenuBackground.png");

	go_back_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	go_back_button->Init({ 70, 606 }, { 897,618,114,94 }, { 897,618,114,94 }, { 897,618,114,94 }, "", ButtonAction::GO_BACK);
}

void j1MainMenu::CreateCreditsScreen() {
	background = (GuiImage*)App->gui->CreateUIElement(UI_Type::IMAGE, this);
	background->Init({ 0,0 }, { 0,0,(int) App->win->width,(int) App->win->height });
	background->texture = App->tex->Load("sprites/UI/MainMenuBackground.png");

	go_back_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	go_back_button->Init({ 70, 606 }, { 897,618,114,94 }, { 897,618,114,94 }, { 897,618,114,94 }, "", ButtonAction::GO_BACK);
}