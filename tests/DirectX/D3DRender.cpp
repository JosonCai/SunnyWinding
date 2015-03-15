#include "D3DRender.h"
#include "Object.h"


void D3DRender::CleanUp()
{
	m_pDirect3DDevice ->Release();
}

bool D3DRender::Init()
{
	// 获取3D图形的物理硬件设备
	IDirect3D9 *pD3D9 = NULL;
	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);

	// 检查设备性能（D3DCAPS9）
	D3DCAPS9 d3dCaps9;
	pD3D9 ->GetDeviceCaps(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		&d3dCaps9);

	int vp = 0;
	if (d3dCaps9.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING; // 使用硬件顶点处理
	}
	else {
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; // 使用软件顶点处理
	}

	// 填充显示参数（d3d present parameter)
	D3DPRESENT_PARAMETERS d3dPP;
	d3dPP.BackBufferWidth				= 1280;
	d3dPP.BackBufferHeight				= 720;
	d3dPP.BackBufferFormat				= D3DFMT_A8R8G8B8;
	d3dPP.BackBufferCount				= 1;
	d3dPP.MultiSampleType				= D3DMULTISAMPLE_NONE;
	d3dPP.MultiSampleQuality			= 0;
	d3dPP.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	d3dPP.hDeviceWindow					= m_hwnd;
	d3dPP.Windowed						= true;
	d3dPP.EnableAutoDepthStencil		= true;
	d3dPP.AutoDepthStencilFormat		= D3DFMT_D24S8;
	d3dPP.Flags							= 0;
	d3dPP.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	d3dPP.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;

	//
	HRESULT hr;
	hr = pD3D9 ->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dPP,
		&m_pDirect3DDevice);

	if (FAILED (hr)) {
		MessageBox (0, "CreateDevice() - FAILED", 0, 0);
		return 0;
	}

	InitObjects(m_pDirect3DDevice);
	return true;
}

void D3DRender::Render()
{
	m_pDirect3DDevice ->Clear (0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		0x00000000, 1.0f, 0);

	m_pDirect3DDevice ->BeginScene();

	ViewTransform(m_pDirect3DDevice);

	m_pDirect3DDevice ->SetStreamSource(0, g_pVertexBuffer, 0, sizeof (COLOR_VERTEX));
	m_pDirect3DDevice ->SetFVF(COLORVERTEX);

	m_pDirect3DDevice ->SetIndices(g_pIndexBuffer); // 设置索引缓存
	m_pDirect3DDevice ->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	m_pDirect3DDevice ->EndScene();
	m_pDirect3DDevice ->Present (0, 0, 0, 0);
}

void D3DRender::Logic()
{

}