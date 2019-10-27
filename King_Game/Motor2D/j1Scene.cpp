#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1UI.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	folder.create(config.child("folder").child_value());
	camera_limits.x = camera_limits_x_offset = config.child("camera_limits").attribute("x").as_int();
	camera_limits.y = camera_limits_y_offset = config.child("camera_limits").attribute("y").as_int();
	camera_limits.w = config.child("camera_limits").attribute("w").as_int();
	camera_limits.h = config.child("camera_limits").attribute("h").as_int();
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	//initial map
	//App->map->Load("hello2.tmx");
	App->map->Load("Level1.tmx");
	//App->map->Load("Level2.tmx");

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	SDL_Rect* camera = &App->render->camera;
	iPoint* player_position = &App->player->position;

	//player inputs ---------------
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		App->ui->transition = true;
		App->ui->loaded = false;
		App->ui->transition_moment = SDL_GetTicks();
		want_to_load = LEVEL_1;
		App->ui->ResetTransition();
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		App->ui->transition = true;
		App->ui->loaded = false;
		App->ui->transition_moment = SDL_GetTicks();
		want_to_load = LEVEL_2;
		App->ui->ResetTransition();
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		ResetLevel();
		App->ui->ResetTransition();
		App->ui->transition_moment = SDL_GetTicks();
		App->ui->transition = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();


	//camera window ------------------
	if (!blocked_camera) {
		if (((player_position->x < camera_limits.x + camera_limits.w /2)) && (-camera->x > 0)) {
			App->render->camera.x += App->player->speed * 2 / 3;
			camera_limits.x -= App->player->speed * 2 / 3;
		}

		if ((player_position->x + App->player->current_animation->GetCurrentFrame().w > camera_limits.x + camera_limits.w/2)&&(-camera->x + camera->w + App->player->speed < App->map->data.width * App->map->data.tile_width)) {
			App->render->camera.x -= App->player->speed * 2 / 3;
			camera_limits.x += App->player->speed * 2 / 3;
		}

		if (((player_position->y < camera_limits.y)) && (camera_limits.y - camera_limits_y_offset > 0)) {
			App->render->camera.y += App->player->speed * 2 / 3;
			camera_limits.y -= App->player->speed * 2 / 3;
		}

		if (((player_position->y + App->player->current_animation->GetCurrentFrame().h > camera_limits.y + camera_limits.h)) && (-camera->y + camera->h + App->player->speed < App->map->data.height * App->map->data.tile_height)) {
			App->render->camera.y -= App->player->speed*2/3;
			camera_limits.y += App->player->speed * 2 / 3;
		}
	}
	//camera manual control --------------

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += CAMERA_SPEED;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= CAMERA_SPEED;

	if((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)&&(App->render->camera.x < 0))
		App->render->camera.x += CAMERA_SPEED;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= CAMERA_SPEED;

	App->map->Draw();

	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count());
	
	App->win->SetTitle(title.GetString());
	SDL_Rect test_rect = { 1000, App->map->data.height * App->map->data.tile_height, 10,10 };
	App->render->DrawQuad(test_rect, 255, 0, 0, 255);
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::Reset_Camera(int kind_of_reset) {
	if (kind_of_reset == 0)
	{
		App->render->camera.x = App->render->initial_camera_x;
		App->render->camera.y = App->render->initial_camera_y;
	}
	camera_limits.x = -App->render->camera.x + camera_limits_x_offset;
	camera_limits.y = -App->render->camera.y + camera_limits_y_offset;
}

void j1Scene::ResetLevel() {
	App->player->position.x = player_x_position;
	App->player->position.y = player_y_position;
	App->player->flip = SDL_FLIP_NONE;
}

void j1Scene::LevelChange(Map loading_map, Map unloading_map) {

	App->map->CleanUp();
	if (loading_map == LEVEL_1) App->map->Load("Level1.tmx");
	if (loading_map == LEVEL_2) App->map->Load("Level2.tmx");
	ResetLevel();
}

bool j1Scene::Save(pugi::xml_node& data) const {

	pugi::xml_node cam_limits = data.append_child("camera_limits");
	pugi::xml_node level = data.append_child("level");

	cam_limits.append_attribute("x") = camera_limits.x;
	cam_limits.append_attribute("y") = camera_limits.y;

	if (current_level == LEVEL_1) level.append_attribute("number") = 1;
	if (current_level == LEVEL_2) level.append_attribute("number") = 2;

	return true;
}

bool j1Scene::Load(pugi::xml_node& data)
{
	camera_limits.x = data.child("position").attribute("x").as_int();
	camera_limits.y = data.child("position").attribute("y").as_int();

	if ((current_level == LEVEL_1) && (data.child("level").attribute("number").as_int() == 2)) LevelChange(LEVEL_2, LEVEL_1);
	if ((current_level == LEVEL_2) && (data.child("level").attribute("number").as_int() == 1)) LevelChange(LEVEL_1, LEVEL_2);

	return true;
}
