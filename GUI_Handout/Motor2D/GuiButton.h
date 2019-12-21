#ifndef _GUIBUTTON_H_
#define _GUIBUTTON_H_
#include "j1UI_Element.h"
#include "SDL_image/include/SDL_image.h"

class p2SString;
class GuiText;

class GuiButton : public j1UI_Element
{
public:
	GuiButton(j1Module* callback);
	~GuiButton();

	void Init(iPoint position, SDL_Rect normal_rect, SDL_Rect hover_rect, SDL_Rect click_rect, p2SString text);
	bool Update(float dt);
	bool Input();
	bool Draw();

private:
	SDL_Texture*	tex;
	SDL_Rect*		current_rect;
	SDL_Rect		normal_rect;
	SDL_Rect		hover_rect;
	SDL_Rect		click_rect;
	GuiText*		text;
};

#endif // !_GUIBUTTON_H_
