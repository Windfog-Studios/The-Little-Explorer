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
#include "j1Audio.h"
#include <Windows.h>
#include "SDL/include/SDL.h"

j1MainMenu::j1MainMenu() : j1Module()
{
	name.create("main_menu");
	fullscreen = false;
}

j1MainMenu::~j1MainMenu() {}

bool j1MainMenu::Awake(pugi::xml_node& config) {
	bool ret = true;

	return ret;
}

bool j1MainMenu::Start() {
	bool ret = true;
	
	App->gui->Start();

	window_width = App->win->width;
	window_width = App->win->height;

	CreateMainScreen();
	App->audio->PlayMusic("path_to_follow.ogg");
	visible_menu = Menu::MAIN_MENU;
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

			App->gui->DestroyAllGui();
			if (App->console->isVisible) App->console->DestroyInterface();
			App->scene->CreateScreenUI();
			break;

		case ButtonAction::CONTINUE:
			App->gui->DestroyAllGui();
			App->LoadGame();
			App->scene->CreateScreenUI();
			App->scene->visible_menu = Menu::SCREEN_UI;
		break;

		case ButtonAction::SETTINGS:
			App->gui->DestroyAllGui();
			CreateSettingsScreen();
		break;

		case ButtonAction::CREDITS:
			if (visible_menu == Menu::MAIN_MENU) {
				App->gui->DestroyAllGui();
				CreateCreditsScreen();
				visible_menu = Menu::CREDITS;
			}
			else {
				ShellExecuteA(NULL, "open", "https://windfog-studios.github.io/The-Little-Explorer/", NULL, NULL, SW_SHOWNORMAL);
			}
		break;

		case ButtonAction::GO_BACK:
			App->gui->DestroyAllGui();
			CreateMainScreen();
		break;

		case ButtonAction::CONTEXTUAL_1:
			if (visible_menu == Menu::CREDITS) { 
				ShellExecuteA(NULL, "open", "https://github.com/silvino00", NULL, NULL, SW_SHOWNORMAL);}
			else if (visible_menu == Menu::SETTINGS) {
				if (!fullscreen)
				{
					SDL_SetWindowFullscreen(App->win->window, SDL_WINDOW_FULLSCREEN);
					fullscreen = true;
				}
				else
				{
					SDL_SetWindowFullscreen(App->win->window, SDL_WINDOW_RESIZABLE);
					fullscreen = false;
				}
				window_width = App->win->width;
				window_width = App->win->height;
			}
			break;

		case ButtonAction::CONTEXTUAL_2:
			if (visible_menu == Menu::CREDITS) { 
				ShellExecuteA(NULL, "open", "https://github.com/marcpages2020", NULL, NULL, SW_SHOWNORMAL);}
			break;

		case ButtonAction::CONTEXTUAL_3:
			if (visible_menu == Menu::CREDITS) { 
				ShellExecuteA(NULL, "open", "https://github.com/Windfog-Studios/The-Little-Explorer/blob/master/License.txt", NULL, NULL, SW_SHOWNORMAL);}
			break;

		case ButtonAction::QUIT:
			App->quit = true;
		break;

		default:
			break;
		}
	}

	if (element->type == UI_Type::SLIDER) {
		App->audio->ChangeVolume(element->output_value);
	}
}

void j1MainMenu::CreateMainScreen() {
	SDL_Rect camera;
	camera = App->render->camera;
	_TTF_Font* principal_font = App->font->Load("fonts/Some Time Later.otf", 50);

	GuiImage* background = (GuiImage*)App->gui->CreateUIElement(UI_Type::IMAGE, this);
	background->Init({ 0,0 }, { 0,0,(int)App->win->width, (int)App->win->height });
	background->texture = App->tex->Load("sprites/UI/MainMenuBackground.png");

	GuiImage* title = (GuiImage*)App->gui->CreateUIElement(UI_Type::IMAGE, this);
	title->Init({ 200,100 }, { 0,0,600, 180 });
	title->texture = App->tex->Load("sprites/UI/title.png");

	GuiButton* start_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	start_button->Init({ 400, 330 }, { 6,547,200,72 }, { 206,547,200,72 }, { 206,547,200,72 }, "Start", ButtonAction::PLAY);

	GuiButton* continue_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	continue_button->Init({ 400, 440 }, { 8,622,196,71 }, { 206,622,196,71 }, { 206,622,200,72 }, "Continue", ButtonAction::CONTINUE);

	GuiButton* settings_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	settings_button->Init({ 400, 540 }, { 6,547,200,72 }, { 206,547,200,72 }, { 206,547,200,72 }, "Settings", ButtonAction::SETTINGS);

	GuiButton* exit_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	exit_button->Init({ 50, 626 }, { 8,622,196,71 }, { 206,622,196,71 }, { 206,622,200,72 }, "Exit", ButtonAction::QUIT);

	GuiButton* credits_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	credits_button->Init({ 780, 626 }, { 6,547,200,72 }, { 206,547,200,72 }, { 206,547,200,72 }, "Credits", ButtonAction::CREDITS);

	visible_menu = Menu::MAIN_MENU;
}

void j1MainMenu::CreateSettingsScreen() {
	GuiImage* background = (GuiImage*)App->gui->CreateUIElement(UI_Type::IMAGE, this);
	background->Init({ 0,0 }, { 0,0,(int)App->win->width,(int)App->win->height });
	background->texture = App->tex->Load("sprites/UI/MainMenuBackground.png");

	GuiImage* menu_background = (GuiImage*)App->gui->CreateUIElement(UI_Type::IMAGE, this);
	menu_background->Init({ 250, 250 }, { 0,0,512,264 });

	GuiButton* go_back_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	go_back_button->Init({ 20, 20 }, { 897,618,114,94 }, { 897,618,114,94 }, { 897,618,114,94 }, "", ButtonAction::GO_BACK);

	GuiButton* fullscreen_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	fullscreen_button->Init({ 300,410 }, { 206, 697, 49,53 }, { 206, 697, 49,53 }, { 262,697,49,53 }, "", ButtonAction::CONTEXTUAL_1,true);

	GuiText* fullscreen_text = (GuiText*)App->gui->CreateUIElement(UI_Type::TEXT, this, nullptr, false, true);
	fullscreen_text->Init({ 380,406 }, "Fullscreen");

	GuiSlider* slider = (GuiSlider*)App->gui->CreateUIElement(UI_Type::SLIDER, this);
	slider->Init({ 100, 300 }, { 0,280,802,33 }, { 56,750,68,46 });

	visible_menu = Menu::SETTINGS;
}

void j1MainMenu::CreateCreditsScreen() {
	
	GuiImage* background = (GuiImage*)App->gui->CreateUIElement(UI_Type::IMAGE, this);
	background->Init({ 0,0 }, { 0,0,(int) App->win->width,(int) App->win->height });
	background->texture = App->tex->Load("sprites/UI/MainMenuBackground.png");

	GuiImage* panel = (GuiImage*)App->gui->CreateUIElement(UI_Type::IMAGE, this);
	panel->Init({ 140,160 }, { 0,1062,726,522 });

	GuiButton* go_back_button = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	go_back_button->Init({ 20, 20 }, { 897,618,114,94 }, { 897,618,114,94 }, { 897,618,114,94 }, "", ButtonAction::GO_BACK);

	GuiImage* title = (GuiImage*)App->gui->CreateUIElement(UI_Type::IMAGE, this);
	title->Init({ 190, 80 }, { 0,0,600, 180 });
	title->texture = App->tex->Load("sprites/UI/title.png");

	GuiText* by = (GuiText*)App->gui->CreateUIElement(UI_Type::TEXT, this, nullptr);
	by->Init({ 470, 200 }, "by");

	GuiText* studio = (GuiText*)App->gui->CreateUIElement(UI_Type::TEXT, this, nullptr);
	studio->Init({ 380,250 }, "Windfog Studios");

	//GuiImage* traveler = (GuiImage*)App->gui->CreateUIElement(UI_Type::IMAGE, this);
	//traveler->Init({ 800,440 }, { 0,0,199,273 });
	//traveler->texture = App->tex->Load("sprites/UI/opp_promo_traveler.png");

	GuiButton* main_webpage = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	main_webpage->Init({420,510}, {1036,618,137,140}, { 1038,619,138,140 }, { 1038,619,138,140 }, "", ButtonAction::CREDITS);

	GuiButton* silvino_medina = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	silvino_medina->Init({ 80,365 }, { 0,1619,381,110 }, { 0,1742,381,110 }, { 0,1760,381,138 }, "Silvino Medina Cardona", ButtonAction::CONTEXTUAL_1);

	GuiButton* marc_pages = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	marc_pages->Init({ 525,365 }, { 0,1619,381,110 }, { 0,1742,381,110 }, { 0,1760,381,138 }, "Marc Pagès Francesch", ButtonAction::CONTEXTUAL_2);

	GuiButton* license = (GuiButton*)App->gui->CreateUIElement(UI_Type::BUTTON, this, nullptr, false, true);
	license->Init({ 780,25 }, { 8,622,196,71 }, { 206,622,196,71 }, { 206,622,200,72 }, "License", ButtonAction::CONTEXTUAL_3);

	visible_menu = Menu::CREDITS;
}