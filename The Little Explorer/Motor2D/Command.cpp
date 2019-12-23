#include "j1Command.h"
#include "p2SString.h"

j1Command::j1Command(const char* g_command, j1Module* g_callback, uint g_min_arg, uint g_max_args, const char* g_explanation) {
	text = g_command;
	callback = g_callback;
	min_args = g_min_arg;
	max_args = g_max_args;
	explanation = g_explanation;
}

j1Command::~j1Command() {}