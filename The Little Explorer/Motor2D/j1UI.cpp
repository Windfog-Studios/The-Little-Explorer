#include "j1App.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1UI.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "brofiler/Brofiler/Brofiler.h"

j1UI::j1UI() : j1Module() {
	
	name.create("ui");

}

j1UI :: ~j1UI() {}

bool j1UI::Awake(pugi::xml_node& config) {
	LOG("Loading UI");
	transition_speed = config.child("transition_speed").attribute("value").as_float();
	return true;
}

bool j1UI::Start() {
	//transition values
	camera = &App->render->camera;
	left_square = { -camera->x - camera->w / 2,camera->y,camera->w / 2 + (int)transition_speed,camera->h };
	right_square = { camera->x + camera->w,camera->y,camera->w * 3 / 4,camera->h };
	return true;
}

bool j1UI::CleanUp() {
	LOG("Freeing UI");
	return true;
}

bool j1UI::Update(float dt) {
	BROFILER_CATEGORY("UIUpdate", Profiler::Color::BlueViolet)
	bool ret = true;
	SDL_Rect rect = { 10,10,10,10 };
	rect.x = -camera->x;
	if (transition) LevelTransition(dt);
	//App->render->DrawQuad(rect, 255, 0, 0, 255);
	return ret;
}
bool j1UI::PostUpdate() {
	BROFILER_CATEGORY("UIPostUpdate", Profiler::Color::GoldenRod)
	if (transition) {
		App->render->DrawQuad(left_square, 0, 0, 0, 255);
		App->render->DrawQuad(right_square, 0, 0, 0, 255);
	}
	return true;
}
void j1UI::LevelTransition(float dt) {
	SDL_Rect screen = { 0, 0, camera->w,camera->h };
	switch (direction)
	{
	case CLOSE:
		if (left_square.x <= -camera->x)
		{
			left_square.x += transition_speed * dt;
			right_square.x -= transition_speed * dt;
			App->render->DrawQuad(left_square, 0, 0, 0, 255);
			App->render->DrawQuad(right_square, 0, 0, 0, 255);
		}
		else
		{
			direction = STATIC;
			App->scene->Reset_Camera(0);
			left_square.x = -camera->x;
			right_square.x = -camera->x + camera->w * 0.5f;
			left_square.y = -camera->y;
			right_square.y = -camera->y;
			App->scene->ResetLevel();
			App->entities->player->state = IDLE;
			App->entities->player->isVisible = true;
		}
		break;
	case OPEN:
		if (left_square.x + left_square.w > camera->x)
		{
			left_square.x -= transition_speed * dt;
			right_square.x += transition_speed * dt;
			App->render->DrawQuad(left_square, 0, 0, 0, 255);
			App->render->DrawQuad(right_square, 0, 0, 0, 255);
		}
		else
		{
			direction = CLOSE;
			transition = false;
			App->scene->blocked_camera = false;
			App->entities->player->controls_blocked = false;
		}
		break;
	case STATIC:
		time = SDL_GetTicks();
		if (transition_moment - time <= - transition_time * 1000)
		{
			left_square.x = -camera->x;
			right_square.x = -camera->x + camera->w * 0.5f;
			direction = OPEN;
		}
		else 
		{
			if (loaded == false) {
				if (App->scene->current_level == App->scene->want_to_load)
				{
					App->scene->ResetLevel();
				}
				else if ((App->scene->current_level == LEVEL_1) && (App->scene->want_to_load == LEVEL_2)) {
					App->scene->LevelChange(LEVEL_2, LEVEL_1);
				}
				else if ((App->scene->current_level == LEVEL_2) && (App->scene->want_to_load == LEVEL_1)) {
					App->scene->LevelChange(LEVEL_1, LEVEL_2);
				}
			}
			loaded = true;
			App->scene->Reset_Camera(0);
			ResetTransition(STATIC);
			App->scene->ResetLevel();
		}
		break;
	default:
		break;
	}
}

void j1UI::ResetTransition(TransitionDirection state) {
	left_square.y = -camera->y;
	right_square.y = -camera->y;
	if (state == CLOSE)
	{
		left_square.x = -camera->x - left_square.w;
		right_square.x = -camera->x + camera->w;
	}
	else
	{
		left_square.x = -camera->x;
		right_square.x = -camera->x + camera->w * 0.5f;
	}
	App->scene->blocked_camera = true;
}