#include "GuiSlider.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "j1Render.h"
#include "GuiText.h"
#include "p2SString.h"
#include "j1Fonts.h"

GuiSlider::GuiSlider(j1Module* g_callback) {
	callback = g_callback;
	texture = nullptr;
	to_delete = false;
}

GuiSlider::~GuiSlider() {}

void GuiSlider::Init(iPoint position, SDL_Rect scrollbar_section, SDL_Rect scroll_thumb_section) {
	iPoint scroll_thumb_position;

	scrollbar = (GuiImage*) App->gui->CreateUIElement(UI_Type::IMAGE, nullptr);
	scrollbar->Init(position, scrollbar_section);

	scroll_thumb = (GuiImage*)App->gui->CreateUIElement(UI_Type::IMAGE, callback, this, true, true);
	scroll_thumb_position.x = position.x + scrollbar_section.w * 0.5f - scroll_thumb_section.w * 0.5f;
	scroll_thumb_position.y = position.y - scroll_thumb_section.h * 0.5f + scrollbar_section.h * 0.5f ;
	scroll_thumb->Init(scroll_thumb_position, scroll_thumb_section);
}

bool GuiSlider::CleanUp() {
	App->gui->DestroyUIElement(scrollbar);
	App->gui->DestroyUIElement(scroll_thumb);
	return true;
}

bool GuiSlider::Input() {
	output_value = (scroll_thumb->screen_position.x + scroll_thumb->rect.w * 0.5f - scrollbar->screen_position.x);
	output_value /= scrollbar->rect.w;

	callback->OnEvent(this, FocusEvent::CLICKED);
	return true;
}


bool GuiSlider::Update(float dt) {

	int ret = true;

	if (parent != nullptr)
	{
		screen_position.x = parent->screen_position.x + local_position.x;
		screen_position.y = parent->screen_position.y + local_position.y;
	}

	if (scroll_thumb->rect.x + scroll_thumb->rect.w * 0.5f < scrollbar->rect.x)
		scroll_thumb->screen_position.x = scrollbar->screen_position.x - scroll_thumb->rect.w * 0.5f;

	if (scroll_thumb->rect.x + scroll_thumb->rect.w * 0.5f > scrollbar->rect.x + scrollbar->rect.w)
		scroll_thumb->screen_position.x = scrollbar->rect.x + scrollbar->rect.w - scroll_thumb->rect.w * 0.5f;

	rect.x = screen_position.x - App->render->camera.x;
	rect.y = screen_position.y - App->render->camera.y;

	return ret;
}

