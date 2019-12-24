#include "GuiButton.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "j1Render.h"
#include "GuiText.h"
#include "p2SString.h"
#include "j1Fonts.h"

GuiButton::GuiButton(j1Module* g_callback, bool g_isStatic){
	callback = g_callback;
	text = new GuiText();
	click_rect = {0,0,0,0};
	tex = nullptr;
	current_rect = &normal_rect;
	isStatic = g_isStatic;
}

GuiButton::~GuiButton() {
	delete text;
}

void GuiButton::Init(iPoint g_position, SDL_Rect g_normal_rect, SDL_Rect g_hover_rect, SDL_Rect g_click_rect, p2SString g_text, ButtonAction g_action) {
	screen_position = g_position;
	tex = (SDL_Texture*)App->gui->GetAtlas();
	normal_rect = g_normal_rect;
	hover_rect = g_normal_rect;
	click_rect = g_click_rect;
	action = g_action;

	if (parent != nullptr)
	{
		local_position.x = screen_position.x - parent->screen_position.x;
		local_position.y = screen_position.y - parent->screen_position.y;
	}

	rect = normal_rect;

	SDL_Rect text_rect;

	App->font->CalcSize(g_text.GetString(), text_rect.w, text_rect.h);
	text_rect.x = screen_position.x + rect.w * 0.5f - text_rect.w * 0.5f; 
	text_rect.y = screen_position.y + rect.h * 0.5f - text_rect.h * 0.5f;

	text->parent = this;
	text->Init({text_rect.x, text_rect.y}, g_text);
}

bool GuiButton::CleanUp() {
	bool ret = true;
	
	return ret;
}

bool GuiButton::Input() {
	
	current_rect = &click_rect;
	callback->OnEvent(this, FocusEvent::CLICKED);
	return true;
}

bool GuiButton::Update(float dt) {
	bool ret = true;

	if (OnHover())
	{
		current_rect = &hover_rect;
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			current_rect = &click_rect;
		}
	}
	else
	{
		current_rect = &normal_rect;
	}
	
	if (parent != nullptr)
	{
		screen_position.x = parent->screen_position.x + local_position.x;
		screen_position.y = parent->screen_position.y + local_position.y;
	}
	rect.x = screen_position.x;
	rect.y = screen_position.y;

	if (text->text.Length() > 0)
	{
		text->Update(dt);
	}

	return true;
}

bool GuiButton::Draw() {

	if (isStatic)
	{
		App->render->Blit(tex, rect.x, rect.y, current_rect, SDL_FLIP_NONE, 0);
	}
	else
	{
		App->render->Blit(tex, rect.x, rect.y, current_rect);
	}

	if (text->text.Length() > 0) { text->Draw(); }

	return true;
}

