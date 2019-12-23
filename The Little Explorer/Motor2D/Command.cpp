#include "j1Command.h"
#include "p2SString.h"

j1Command::j1Command(p2SString command_text, j1Module* g_callback) {
	text = command_text;
	callback = g_callback;
}

j1Command::~j1Command() {}