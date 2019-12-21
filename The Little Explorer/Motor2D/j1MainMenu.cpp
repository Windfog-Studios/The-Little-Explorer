#include "j1MainMenu.h"
#include "j1Render.h"
#include "j1App.h"
#include "j1Gui.h"
#include "GuiButton.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "j1Scene.h"
#include "j1Textures.h"

j1MainMenu::j1MainMenu() : j1Module()
{
	name.create("main_menu");
	show_quad = true;
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
	if (show_quad)
	{
		App->render->DrawQuad({ 0,0,1024,768 }, 100, 200, 100, 200);
	}
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
			SDL_Delay(50);
			PERF_START(App->scene->timer);
			show_quad = false;
			break;

		case ButtonAction::CONTINUE:
		//TODO
		break;

		case ButtonAction::SETTINGS:
			App->gui->DestroyAllGui();
			CreateSettingsScreen();
		break;

		case ButtonAction::CREDITS:
		//TODO
		break;
		case ButtonAction::GO_BACK:
			App->gui->DestroyAllGui();
			SDL_Delay(50);
			CreateMainScreen();
		break;
		case ButtonAction::QUIT:
		SDL_Quit();
		break;

		default:
			break;
		}
	}
}

void j1MainMenu::CreateMainScreen() {

	title = (GuiImage*)App->gui->CreateUIElement(UI_Type::IMAGE, this);
	title->Init({ 160,120 }, { 0,0,700,200 });
	title->tex = App->tex->Load("sprites/UI/title.png");

	start_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	start_button->Init({ 710, 392 }, { 6,547,200,72 }, { 206,547,200,72 }, { 206,547,200,72 }, "Start", ButtonAction::PLAY);

	continue_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	continue_button->Init({ 750, 470 }, { 6,547,200,72 }, { 206,547,200,72 }, { 206,547,200,72 }, "Continue", ButtonAction::CONTINUE);

	settings_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	settings_button->Init({ 710, 546 }, { 6,547,200,72 }, { 206,547,200,72 }, { 206,547,200,72 }, "Settings", ButtonAction::SETTINGS);

	exit_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	exit_button->Init({ 750, 626 }, { 6,547,200,72 }, { 206,547,200,72 }, { 206,547,200,72 }, "Exit", ButtonAction::QUIT);

	credits_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	credits_button->Init({ 60, 626 }, { 6,547,200,72 }, { 206,547,200,72 }, { 206,547,200,72 }, "Credits", ButtonAction::CREDITS);

}

void j1MainMenu::CreateSettingsScreen() {
	//TODO: create slider
	go_back_from_settings_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	go_back_from_settings_button->Init({ 60, 626 }, { 6,547,200,72 }, { 206,547,200,72 }, { 206,547,200,72 }, "Go Back", ButtonAction::GO_BACK);
}