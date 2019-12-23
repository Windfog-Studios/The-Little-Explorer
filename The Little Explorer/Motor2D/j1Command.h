#ifndef _j1COMMAND_H_
#define _j1COMMAND_H_
#include "j1Module.h"

struct p2SString;
class j1Module;

#define MAX_COMMAND_LENGTH 32

class j1Command 
{
public:
	j1Command(p2SString command_text, j1Module* callback);
	~j1Command();

public:
	p2SString text;
	j1Module* callback;
};

#endif // !_j1COMMAND_H_	
