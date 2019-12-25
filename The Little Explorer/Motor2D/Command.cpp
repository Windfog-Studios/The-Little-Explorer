#include "j1Command.h"
#include "p2SString.h"

j1Command::j1Command(const char* g_command, j1Module* g_callback, const char* g_explanation) {
	text = g_command;
	callback = g_callback;
	explanation = g_explanation;
}

j1Command::~j1Command() {}