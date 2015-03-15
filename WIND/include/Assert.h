#ifndef	ASSERT_H_
#define ASSERT_H_
#include <cassert>

#if defined (_DEBUG)
extern bool AssertFunction (bool, char*, int, char*, bool*);

#define Assert(exp, description) \
	do {	\
	static bool alwaysIgnore = false;	\
	if (!alwaysIgnore)	{	\
	if (AssertFunction ( (int)(exp), description, __LINE__, __FILE__, &alwaysIgnore) )	{\
	_asm { int 3 }	\
	}	\
	} \
	}while(0)

#else
#define Assert(exp, description)	assert( (exp) && (description) )
#endif

#endif