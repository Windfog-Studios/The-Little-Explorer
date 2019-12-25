#ifndef _j1COMMAND_H_
#define _j1COMMAND_H_
#include "j1Module.h"

struct p2SString;
class j1Module;

#define MAX_COMMAND_LENGTH 32

class j1Command 
{
public:
	j1Command(const char* g_command, j1Module* g_callback, uint min_arg, uint max_args, const char* explanation);
	~j1Command();
	bool CleanUp();

public:
	p2SString text;
	j1Module* callback;
	uint max_args;
	uint min_args;
	const char* explanation;
};

#endif // !_j1COMMAND_H_	
