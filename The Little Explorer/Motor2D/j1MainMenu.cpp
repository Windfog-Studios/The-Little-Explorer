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
	App->render->DrawQuad({ 0,0,640,480 }, 100, 100, 100, 255);

	App->gui->Start();

	title = (GuiImage*)App->gui->CreateUIElement(UI_Type::IMAGE, this);
	title->Init({ 160,120 }, { 0,0,700,200 });
	title->tex = App->tex->Load("sprites/UI/title.png");


	start_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this,nullptr,false,true);
	start_button->Init({ 725, 400 }, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, "Start", ButtonAction::PLAY);

	continue_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	continue_button->Init({ 725, 472 }, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, "Continue");

	settings_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	settings_button->Init({ 725, 545 }, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, "Settings");

	credits_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	credits_button->Init({ 60, 620 }, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, "Credits");

	exit_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	exit_button->Init({ 725, 620 }, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, "Exit", ButtonAction::QUIT);

	return ret;
}

bool j1MainMenu::PostUpdate() {
	bool ret = true;
	if (show_quad)
	{
		App->render->DrawQuad({ 0,0,1024,768 }, 100, 200, 100, 255);
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
			show_quad = false;
			break;

		case ButtonAction::CONTINUE:
		//TODO
		break;

		case ButtonAction::SETTINGS:
		//TODO
		break;

		case ButtonAction::CREDITS:
		//TODO
		break;

		case ButtonAction::QUIT:
		SDL_Quit();
		break;

		default:
			break;
		}
	}
}