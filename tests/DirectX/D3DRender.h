#pragma once
#include <d3dx9.h>

class D3DRender
{
public:
	D3DRender (HWND hwnd): m_hwnd (hwnd), m_pDirect3DDevice(NULL) {}
	~D3DRender() {}

public:
	bool Init ();
	void Logic ();
	void Render ();
	void CleanUp ();
	IDirect3DDevice9* GetDevice () { return m_pDirect3DDevice; }

private:
	HWND m_hwnd;
	IDirect3DDevice9 *m_pDirect3DDevice;
};
