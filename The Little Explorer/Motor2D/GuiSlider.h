#ifndef _GUISLIDER_H_
#define _GUISLIDER_H_
#include "p2Point.h"
#include "j1UI_Element.h"
#include "GuiButton.h"
#include "SDL_image/include/SDL_image.h"

class GuiSlider : public j1UI_Element
{
public:

	GuiSlider(j1Module* callback);
	~GuiSlider();

	void Init();
	bool Update(float dt);
	bool CleanUp();
	bool Input();
	bool Draw();

	GuiButton* scrollButton;
	float value;

private:
	SDL_Texture* texture;
	SDL_Rect* current_rect;
	SDL_Rect		normal_rect;
	SDL_Rect		hover_rect;
	SDL_Rect		click_rect;
	GuiText* text;
public:
	ButtonAction	action;
};


#endif // !_GUISLIDER_H_
