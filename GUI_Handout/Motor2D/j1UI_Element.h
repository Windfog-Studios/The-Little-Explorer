#ifndef _j1UIELEMENT_H_
#define _j1UIELEMENT_H_
#include "p2Point.h"

#include "SDL/include/SDL.h"

enum class UI_Type
{
	BUTTON,
	IMAGE,
	TEXT,
	INPUT_TEXT,
	MAX_UI_ELEMENTS
};

class j1UI_Element
{
public:
	j1UI_Element() {};
	~j1UI_Element() {};

	virtual bool Input() { return true; }
	virtual bool Draw() { return true; }

	//SDL_Rect GetScreenRect() const{}
	//SDL_Rect GetLocalRect() const{}
	iPoint GetScreenPos() const { return screen_position; }
	iPoint GetLocalPos() const { return local_position; }
	void SetLocalPos(iPoint new_position) { local_position = new_position; }
	bool MouseHovering();

public:		
	iPoint			local_position;
	iPoint			screen_position;
	SDL_Rect		rect;
	UI_Type			type;
	j1UI_Element*	parent = nullptr;

public:
	bool draggable;
};

#endif // !j1UIELEMENT_H

