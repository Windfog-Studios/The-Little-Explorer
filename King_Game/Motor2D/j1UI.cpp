#include "j1App.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1UI.h"
#include "j1Render.h"
#include "j1Scene.h"

j1UI::j1UI() : j1Module() {
	
	name.create("ui");

}

j1UI :: ~j1UI() {}

bool j1UI::Awake() {
	LOG("Loading UI");
	return true;
}

bool j1UI::Start() {
	//transition values
	camera = &App->render->camera;
	left_square = { camera->x - camera->w / 2,camera->y,camera->w / 2,camera->h };
	right_square = { camera->x + camera->w,camera->y,camera->w * 3 / 4,camera->h };
	return true;
}

bool j1UI::CleanUp() {
	LOG("Freeing UI");
	return true;
}

bool j1UI::Update(float dt) {
	bool ret = true;
	if (transition == true) LevelTransition();
	return ret;
}

void j1UI::LevelTransition() {

	switch (direction)
	{
	case CLOSE:
		if (left_square.x < camera->x)
		{
			left_square.x += 3;
			right_square.x -= 3;
			App->render->DrawQuad(left_square, 0, 0, 0, 255);
			App->render->DrawQuad(right_square, 0, 0, 0, 255);
		}
		else
		{
			direction = STATIC;
		}
		break;
	case OPEN:
		if (left_square.x + left_square.w > camera->x)
		{
			left_square.x -= 3;
			right_square.x += 3;
			App->render->DrawQuad(left_square, 0, 0, 0, 255);
			App->render->DrawQuad(right_square, 0, 0, 0, 255);
		}
		else
		{
			transition = CLOSE;
		}
		break;
	case STATIC:
		if (SDL_GetTicks() - transition_moment >= transition_time * 1000)
		{
			direction = OPEN;
		}
		else
		{
			App->render->DrawQuad(left_square, 0, 0, 0, 255);
			App->render->DrawQuad(right_square, 0, 0, 0, 255);

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
		break;
	default:
		break;
	}
}

void j1UI::ResetTransition() {
	camera = &App->render->camera;
	left_square = { camera->x - camera->w / 2,camera->y,camera->w / 2,camera->h };
	right_square = { camera->x + camera->w,camera->y,camera->w * 3 / 4,camera->h };

	LOG("camera x: %i", camera->x);
}