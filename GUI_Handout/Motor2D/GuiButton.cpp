#include "GuiButton.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "j1Render.h"
#include "GuiText.h"
#include "p2SString.h"

GuiButton::GuiButton(j1Module* g_callback){
	callback = g_callback;
	text = new GuiText();
	click_rect = {0,0,0,0};
	tex = nullptr;
}

void GuiButton::Init(iPoint g_position, SDL_Rect g_normal_rect, SDL_Rect g_hover_rect, SDL_Rect g_click_rect, p2SString g_text) {
	screen_position = g_position;
	tex = (SDL_Texture*)App->gui->GetAtlas();
	normal_rect = g_normal_rect;
	hover_rect = g_normal_rect;
	click_rect = g_click_rect;

	if (parent != nullptr)
	{
		local_position.x = screen_position.x - parent->screen_position.x;
		local_position.y = screen_position.y - parent->screen_position.y;
	}

	iPoint text_position;
	text_position.x = screen_position.x + rect.w * 0.5f; //text width has to be deducted
	text_position.y = screen_position.y + rect.h * 0.5f; //text height has to be deducted

	text->Init(text_position, g_text);
	text->parent = this;
}

bool GuiButton::Input() {
	if (current_rect != nullptr)
	{
		rect.w = current_rect->w;
		rect.h = current_rect->h;
	}
	iPoint mouse_motion;
	if (!MouseHovering())
	{
		current_rect = &normal_rect;
	}
	else {
		current_rect = &hover_rect;
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			current_rect = &click_rect;
		}
	}


	return true;
}

bool GuiButton::Update(float dt) {
	bool ret = true;
	if (parent != nullptr)
	{
		screen_position.x = parent->screen_position.x + local_position.x;
		screen_position.y = parent->screen_position.y + local_position.y;
	}
	rect.x = screen_position.x;
	rect.y = screen_position.y;

	text->Update(dt);

	return true;
}

bool GuiButton::Draw() {
	App->render->Blit(tex, screen_position.x, screen_position.y, current_rect);
		text->Draw();
	return true;
}