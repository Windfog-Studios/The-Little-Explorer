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

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	//invisible square delimiting the space in the camera where the player can move
	top_edge = App->render->camera.y + App->render->camera.h / 4;
	bottom_edge = App->render->camera.y + App->render->camera.h* 3/4;
	left_edge = App->render->camera.x + App->render->camera.w / 4;
	right_edge = App->render->camera.x + App->render->camera.w *3/4;
	
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
	iPoint* player_position = &App->player->position;

	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();


	//camera window ------------------

	if ((player_position->x < left_edge)) {
		right_edge -= App->player->speed;
		left_edge -= App->player->speed;
		if (App->render->camera.x > 0) {
			App->render->camera.x += App->player->speed;
		}
	}
	if (player_position->x+App->player->current_animation->GetCurrentFrame().w > right_edge) { 
		App->render->camera.x -= App->player->speed;
		right_edge += App->player->speed;
		left_edge+= App->player->speed;
	}
	if ((player_position->y < top_edge)) {
		top_edge -= App->player->speed;
		bottom_edge -= App->player->speed;
		if (App->render->camera.y > App->render->initial_camera_y - App->map->map_offset * App->map->data.tile_height)	
			App->render->camera.y += App->player->speed;
	}
	if ((player_position->y + App->player->current_animation->GetCurrentFrame().h > bottom_edge)) {
		App->render->camera.y -= App->player->speed;
		top_edge+= App->player->speed;
		bottom_edge+= App->player->speed;
	}

	//camera manual control --------------

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 3;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 3;

	if((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)&&(App->render->camera.x < 0))
		App->render->camera.x += 3;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 3;


	//App->render->Blit(img, 0, 0);
	App->map->Draw();

	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());
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

void j1Scene::Reset_Camera() {
	App->render->camera.x = App->render->initial_camera_x;
	App->render->camera.y = App->render->initial_camera_y;
	top_edge = App->render->camera.y + App->render->camera.h / 4;
	bottom_edge = App->render->camera.y + App->render->camera.h * 3 / 4;
	left_edge = App->render->camera.x + App->render->camera.w / 4;
	right_edge = App->render->camera.x + App->render->camera.w * 3 / 4;
}