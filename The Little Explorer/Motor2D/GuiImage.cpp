#include "j1App.h"
#include "j1Gui.h"
#include "GuiImage.h"
#include "j1Render.h"
#include "j1Textures.h"

GuiImage::GuiImage(j1Module* g_callback) {
	texture = (SDL_Texture*)App->gui->GetAtlas();
	callback = g_callback;
	parent = nullptr;
	to_delete = false;
}

void GuiImage::Init(iPoint position, SDL_Rect g_section) {
	screen_position = position;
	section = g_section;
	rect.x = screen_position.x;
	rect.y = screen_position.y;
	rect.w = section.w;
	rect.h = section.h;

	if (parent != nullptr)
	{
		local_position.x = screen_position.x - parent->screen_position.x;
		local_position.y = screen_position.y - parent->screen_position.y;
	}
}

bool GuiImage::CleanUp() {
	bool ret = true;
	texture = nullptr;
	return ret;
}

bool GuiImage::Update(float dt) {
	bool ret = true;
	if ((parent != nullptr)&&(parent->draggable))
	{
		screen_position.x = parent->screen_position.x + local_position.x;
		screen_position.y = parent->screen_position.y + local_position.y;
	}

	rect.x = screen_position.x - App->render->camera.x;
	rect.y = screen_position.y - App->render->camera.y;
	return ret;
}

bool GuiImage::Draw() {
	App->render->Blit(texture, rect.x, rect.y, &section);
	return true;
}