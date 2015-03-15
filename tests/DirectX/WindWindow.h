#pragma once
#include <d3dx9.h>
#include "CommonFunction.h"
#include "D3DRender.h"

class Window
{
public:
	Window (HINSTANCE hInst, int nShowCmd) : m_inst (hInst), m_showCmd (nShowCmd), m_d3dRender(NULL) {}
	~Window() {}

public:
	bool Init ();
	void Run ();
	void CleanUp ();

protected:
	bool Create ();
	void Show ();
	int EnterMsgLoop ();
	bool Display (float timeDelta);
	static LRESULT CALLBACK WndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE m_inst;
	int m_showCmd;
	HWND m_windowHandle;
	D3DRender *m_d3dRender;
};