#include "j1App.h"
#include "j1Gui.h"
#include "GuiImage.h"
#include "j1Render.h"

GuiImage::GuiImage(j1Module* g_callback, bool g_isStatic) {
	tex = (SDL_Texture*)App->gui->GetAtlas();
	callback = g_callback;
	isStatic = g_isStatic;
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
	return ret;
}


void GuiImage::EmptyInit(SDL_Rect empty_rect) {
	screen_position.x = rect.x = empty_rect.x;
	screen_position.y = rect.y = empty_rect.y;
	rect.w = empty_rect.w;
	rect.h = empty_rect.h;

	if (parent != nullptr)
	{
		local_position.x = screen_position.x - parent->screen_position.x;
		local_position.y = screen_position.y - parent->screen_position.y;
	}
}

bool GuiImage::Update(float dt) {
	bool ret = true;
	if (parent != nullptr)
	{
		screen_position.x = parent->screen_position.x + local_position.x;
		screen_position.y = parent->screen_position.y + local_position.y;
	}
	rect.x = screen_position.x;
	rect.y = screen_position.y;
	return ret;
}

bool GuiImage::Draw() {

	if (isStatic) 
	{
		App->render->Blit(tex, rect.x, rect.y, &section, SDL_FLIP_NONE, 0);
	}
	else
	{
		App->render->Blit(tex, rect.x, rect.y, &section);
	}
	return true;
}