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
#include "j1Pathfinding.h"
#include "j1Collision.h"
#include "brofiler/Brofiler/Brofiler.h"

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
	camera_frame.x = camera_frame_x_margin = config.child("camera_frame").attribute("x").as_int();
	camera_frame.y = camera_frame_y_margin = config.child("camera_frame").attribute("y").as_int();
	camera_frame.w = config.child("camera_frame").attribute("w").as_int();
	camera_frame.h = config.child("camera_frame").attribute("h").as_int();
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	//initial map
	//App->map->Load("hello2.tmx");
	if (App->map->Load("Level1.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path2.png");

	//App->map->Load("Level2.tmx");

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("SceneUpdate", Profiler::Color::HotPink)

	SDL_Rect*	camera = &App->render->camera;
	fPoint*		player_position = &App->player->position;
	float		camera_frame_x_center = ceil(camera_frame.x + camera_frame.w * 0.5f);
	float		camera_frame_y_center = ceil(camera_frame.y + camera_frame.h * 0.5f);
	bool		camera_manual_control = false;

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
		if (!camera_manual_control)
		{
			if (((player_position->x < camera_frame_x_center)) && (-camera->x > 0)) {
				App->render->camera.x += floor(CAMERA_SPEED * dt);
				camera_frame.x -= floor(CAMERA_SPEED * dt);
			}

			if ((player_position->x + App->player->current_animation->GetCurrentFrame().w > camera_frame_x_center) && (-camera->x + camera->w < App->map->data.width * App->map->data.tile_width)) {
				App->render->camera.x -= floor(CAMERA_SPEED * dt);
				camera_frame.x += floor(CAMERA_SPEED * dt);
			}

			if (((player_position->y < camera_frame_y_center)) && (camera_frame.y - camera_frame_y_margin > 0)) {
				App->render->camera.y += floor(CAMERA_SPEED * dt);
				camera_frame.y -= floor(CAMERA_SPEED * dt);
			}

			if (((player_position->y + App->player->current_animation->GetCurrentFrame().h > camera_frame_y_center)) && (-camera->y + camera->h < App->map->data.height * App->map->data.tile_height)) {
				App->render->camera.y -= floor(CAMERA_SPEED * dt);
				camera_frame.y += floor(CAMERA_SPEED * dt);
			}
		}
	}
	//camera manual control --------------

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		App->render->camera.y += floor(CAMERA_SPEED * dt);
		camera_manual_control = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		App->render->camera.y -= floor(CAMERA_SPEED * dt);
		camera_manual_control = true;
	}

	if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) && (App->render->camera.x < 0)) {
		App->render->camera.x += floor(CAMERA_SPEED * dt);
		camera_manual_control = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		App->render->camera.x -= floor(CAMERA_SPEED * dt);
		camera_manual_control = true;
	}

	App->map->Draw();

	// Debug pathfinding ------------------------------
	int x, y;
	SDL_Rect Debug_rect = { 0,0,32,32 };
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	//App->render->Blit(debug_tex, p.x, p.y);
	Debug_rect.x = p.x;
	Debug_rect.y = p.y;
	if (App->collision->debug)App->render->DrawQuad(Debug_rect, 0, 0, 255,80);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		Debug_rect.x = pos.x;
		Debug_rect.y = pos.y;
		if (App->collision->debug)App->render->DrawQuad(Debug_rect, 90, 850, 230, 80);
		//App->render->Blit(debug_tex, pos.x, pos.y);
	}

	//SDL_Rect test_rect = { 1000, App->map->data.height * App->map->data.tile_height, 10,10 };
	//App->render->DrawQuad(test_rect, 255, 0, 0, 255);

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("ScenePostUpdate", Profiler::Color::Indigo);
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	ret = false;

	return ret;
}

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
	camera_frame.x = -App->render->camera.x + camera_frame_x_margin;
	camera_frame.y = -App->render->camera.y + camera_frame_y_margin;
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

	pugi::xml_node cam_frame = data.append_child("camera_frame");
	pugi::xml_node level = data.append_child("level");

	cam_frame.append_attribute("x") = camera_frame.x;
	cam_frame.append_attribute("y") = camera_frame.y;

	if (current_level == LEVEL_1) level.append_attribute("number") = 1;
	if (current_level == LEVEL_2) level.append_attribute("number") = 2;

	return true;
}

bool j1Scene::Load(pugi::xml_node& data)
{
	camera_frame.x = data.child("position").attribute("x").as_int();
	camera_frame.y = data.child("position").attribute("y").as_int();

	if ((current_level == LEVEL_1) && (data.child("level").attribute("number").as_int() == 2)) LevelChange(LEVEL_2, LEVEL_1);
	if ((current_level == LEVEL_2) && (data.child("level").attribute("number").as_int() == 1)) LevelChange(LEVEL_1, LEVEL_2);

	return true;
}
