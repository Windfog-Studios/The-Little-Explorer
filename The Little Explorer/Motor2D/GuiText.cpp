#include "GuiText.h"
#include "j1App.h"
#include "j1Render.h"

GuiText::GuiText() {
	callback = nullptr;
	font = DEFAULT_FONT;
	parent = nullptr;
	texture = nullptr;
	draggable = false;
	interactable = false;
}

GuiText::GuiText(j1Module* g_callback, bool g_isStatic) {
	callback = g_callback;
	font = DEFAULT_FONT;
	isStatic = g_isStatic;
}

GuiText::~GuiText() {}

void GuiText::Init(iPoint g_position, p2SString g_text) {
	screen_position = g_position;
	text = g_text;
	if (text.Length() > 0)
	{
		texture = App->font->Print(text.GetString());
	}
	rect.x = screen_position.x;
	rect.y = screen_position.y;
	App->font->CalcSize(text.GetString(), rect.w, rect.h);

	if (parent != nullptr)
	{
		local_position.x = screen_position.x - parent->screen_position.x;
		local_position.y = screen_position.y - parent->screen_position.y;
	}
}

bool GuiText::CleanUp() {
	bool ret = true;
	return ret;
}

bool GuiText::Update(float dt) {
	bool ret = true;
	if (parent != nullptr)
	{
		screen_position.x = parent->screen_position.x + local_position.x;
		screen_position.y = parent->screen_position.y + local_position.y;
	}
	rect.x = screen_position.x;
	rect.y = screen_position.y;

	if (isStatic)
	{
		rect.x = screen_position.x - App->render->camera.x;
		rect.y = screen_position.y - App->render->camera.y;
	}
	return ret;
}

bool GuiText::Draw() {

	if (text.Length() > 0)
	{
		texture = App->font->Print(text.GetString());
		App->render->Blit(texture, rect.x, rect.y);
	}
	return true;
}