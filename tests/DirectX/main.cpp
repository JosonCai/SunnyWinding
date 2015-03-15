#include <d3dx9.h>
#include "CommonFunction.h"
#include "WindWindow.h"

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "winmm.lib")

#ifdef WIN32
int WINAPI WinMain ( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
#endif
{
	Window mainWindow (hInstance, nShowCmd);

	mainWindow.Run();

	return 0;
}