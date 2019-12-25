#include "GuiText.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"

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

void GuiText::Init(iPoint g_position, p2SString g_text, char* g_font) {
	screen_position = g_position;
	text = g_text;
	font = g_font;

	if (font == DEFAULT_FONT)
	{
		texture = App->font->Print(text.GetString());
	}
	else
	{
		texture = App->font->Print(text.GetString(), { (255),(255),(255),(255) }, App->font->console_font);
	}

	rect.x = screen_position.x;
	rect.y = screen_position.y;
	
	if (g_font == DEFAULT_FONT)
	{
		App->font->CalcSize(text.GetString(), rect.w, rect.h);
	}
	else
	{
		App->font->CalcSize(text.GetString(), rect.w, rect.h,App->font->console_font);
	}


	if (parent != nullptr)
	{
		local_position.x = screen_position.x - parent->screen_position.x;
		local_position.y = screen_position.y - parent->screen_position.y;
	}
}

bool GuiText::CleanUp() {
	bool ret = true;
	App->tex->UnLoad(texture);
	texture = nullptr;
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
	/*
	if (isStatic)
	{
		rect.x = screen_position.x - App->render->camera.x;
		rect.y = screen_position.y - App->render->camera.y;
	}
	*/
	return ret;
}

bool GuiText::Draw() {

	if (text.Length() > 0)
	{
		if (font == DEFAULT_FONT)
		{
			texture = App->font->Print(text.GetString());
		}
		else
		{
			texture = App->font->Print(text.GetString(), { (255),(255),(255),(255) }, App->font->console_font);
		}

		App->render->Blit(texture, rect.x, rect.y);
	}
	return true;
}