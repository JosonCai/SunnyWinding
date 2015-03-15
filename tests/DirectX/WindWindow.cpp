#include "WindWindow.h"

bool Window::Create()
{
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof (WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = m_inst;
	wndClass.hIcon = ::LoadIcon (NULL, IDI_APPLICATION);
	wndClass.hCursor = ::LoadCursor (NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)::GetStockObject(GRAY_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = "SunWind";
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;

	if (!RegisterClassEx (&wndClass)) {
		return false;
	}

	m_windowHandle = CreateWindow ("SunWind", "SunWind", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, m_inst, NULL);

	m_d3dRender = new D3DRender (m_windowHandle);
	if (m_d3dRender == NULL) {
		::MessageBox (NULL, "CreateD3DRender - failed", NULL, MB_OK);
		return false;
	}
	m_d3dRender ->Init();
	
	return true;
}

void Window::Show ()
{
	::ShowWindow(m_windowHandle, m_showCmd);
	::UpdateWindow(m_windowHandle);
}


int Window::EnterMsgLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof (MSG));

	static float lastTime = (float) ::timeGetTime();

	while (msg.message != WM_QUIT) {
		if (::PeekMessage (&msg, 0, 0, 0, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage (&msg);
		}
		else {
			float currTime = (float) ::timeGetTime ();
			float timeDelta = (currTime - lastTime) * 0.001f;

			Display (timeDelta);

			lastTime = currTime;
		}
	}

	return msg.wParam;
}


bool Window::Display(float timeDelta)
{
	m_d3dRender ->Render();

	return true;
}


void Window::CleanUp()
{
	m_d3dRender ->CleanUp();
}


void Window::Run()
{
	if ( !Create() ) {
		::MessageBox (NULL, "Create - FAILED", 0, 0);
		return;
	}

	Show();

	EnterMsgLoop();

	CleanUp();
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			::DestroyWindow(hwnd);
		}
		break;
	}

	return ::DefWindowProc (hwnd, msg, wParam, lParam);
}