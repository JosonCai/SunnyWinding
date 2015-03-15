#pragma once
#include "DataType.h"
#include <d3dx9.h>
struct COLOR_VERTEX
{
	WIND::Float32	x, y, z;
	WIND::UInt32	color;
};

#define COLORVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

IDirect3DVertexBuffer9 *g_pVertexBuffer = NULL;
IDirect3DIndexBuffer9 *g_pIndexBuffer = NULL;

bool InitObjects (IDirect3DDevice9 *pDirect3DDevice9)
{
	// �������㻺�����������
	if (FAILED (pDirect3DDevice9 ->CreateVertexBuffer(8 * sizeof (COLOR_VERTEX), 0, COLORVERTEX, D3DPOOL_DEFAULT, &g_pVertexBuffer, NULL))) {
		MessageBox (NULL, "CreateVertexBuffer - FAILED", NULL, MB_OK);
		return false;
	}
	if (FAILED (pDirect3DDevice9 ->CreateIndexBuffer(36 * sizeof (WORD), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIndexBuffer, NULL))) {
		MessageBox (NULL, "CreateIndexBuffer - FAILED", NULL, MB_OK);
		return false;
	}

	// ��䶥������
	srand(timeGetTime());
	COLOR_VERTEX vertices[] = 
	{
		{-20.0f, 20.0f, -20.0f, D3DCOLOR_XRGB (rand() % 256, rand() % 256, rand() % 256)},
		{-20.0f, 20.0f, 20.0f, D3DCOLOR_XRGB (rand() % 256, rand() % 256, rand() % 256)},
		{20.0f, 20.0f, 20.0f, D3DCOLOR_XRGB (rand() % 256, rand() % 256, rand() % 256)},
		{20.0f, 20.0f, -20.0f, D3DCOLOR_XRGB (rand() % 256, rand() % 256, rand() % 256)},
		{-20.0f, -20.0f, -20.0f, D3DCOLOR_XRGB (rand() % 256, rand() % 256, rand() % 256)},
		{-20.0f, -20.0f, 20.0f, D3DCOLOR_XRGB (rand() % 256, rand() % 256, rand() % 256)},
		{20.0f, -20.0f, 20.0f, D3DCOLOR_XRGB (rand() % 256, rand() % 256, rand() % 256)},
		{20.0f, -20.0f, -20.0f, D3DCOLOR_XRGB (rand() % 256, rand() % 256, rand() % 256)}
	};

	// ���涥��
	void *pVertices;
	if (FAILED (g_pVertexBuffer ->Lock(0, sizeof (vertices), (void**)&pVertices, 0))) {
		MessageBox (NULL, "LOCK VERTEXS - FAILED", NULL, MB_OK);
		return false;
	}
	memcpy (pVertices, vertices, sizeof (vertices));
	g_pVertexBuffer ->Unlock();

	// �����������
	WORD *pIndeices = NULL;
	g_pIndexBuffer ->Lock(0, 0, (void**)&pIndeices, 0);
	// ����
	pIndeices[0] = 0, pIndeices[1] = 1, pIndeices[2] = 2;
	pIndeices[3] = 0, pIndeices[4] = 2, pIndeices[5] = 3;
	// ����
	pIndeices[6] = 0, pIndeices[7] = 3, pIndeices[8] = 7;
	pIndeices[9] = 0, pIndeices[10] = 7, pIndeices[11] = 4;
	// �����
	pIndeices[12] = 0, pIndeices[12] = 4, pIndeices[14] = 5;
	pIndeices[15] = 0, pIndeices[16] = 5, pIndeices[17] = 1;
	// �Ҳ���
	pIndeices[18] = 2, pIndeices[19] = 6, pIndeices[20] = 7;
	pIndeices[21] = 2, pIndeices[22] = 7, pIndeices[23] = 3;
	// ����
	pIndeices[24] = 2, pIndeices[25] = 5, pIndeices[26] = 6;
	pIndeices[27] = 2, pIndeices[28] = 1, pIndeices[29] = 5;
	// ����
	pIndeices[30] = 4, pIndeices[31] = 6, pIndeices[32] = 5;
	pIndeices[33] = 4, pIndeices[34] = 7, pIndeices[35] = 6;
	g_pIndexBuffer ->Unlock();

	// ������Ⱦ״̬--W.S.R �ⲻӦ�������⺯������������
	pDirect3DDevice9 ->SetRenderState(D3DRS_LIGHTING, FALSE); //�رչ���

	return true;
}


void ViewTransform (IDirect3DDevice9 *pDirect3DDevice9)
{
	// ����任
	D3DXMATRIX worldMatrix;
	D3DXMatrixTranslation(&worldMatrix, 1.0f, 2.0f, -1.0f);
	D3DXMATRIX rX, rY, rZ;
	D3DXMatrixRotationX(&rX, 3.141592653f * 0.333f);
	D3DXMatrixRotationY(&rY, 3.141592653f * 0.333f);
	D3DXMatrixRotationZ(&rZ, 3.141592653f * 0.333f);
	worldMatrix = rX * rY * rZ * worldMatrix;
	pDirect3DDevice9 ->SetTransform(D3DTS_WORLD, &worldMatrix);

	// ȡ���任
	D3DXVECTOR3 position (0.0f, 0.0f, -200.0f);
	D3DXVECTOR3 targetPoint (0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 worldUp (0.0f, 1.0f, 0.0f);
	D3DXMATRIX lookMatrix;
	D3DXMatrixLookAtLH(&lookMatrix, &position, &targetPoint, &worldUp);
	pDirect3DDevice9 ->SetTransform(D3DTS_VIEW, &lookMatrix);

	// ��������
	pDirect3DDevice9 ->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ͶӰ�任
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, 3.141592653f * 0.5f, 16.0f / 9.0f, 1.0f, 1000.0f);
	pDirect3DDevice9 ->SetTransform(D3DTS_PROJECTION, &proj);

	// �ӿڱ任
	D3DVIEWPORT9 vp = {0, 0, 1280, 720, 0, 1};
	pDirect3DDevice9 ->SetViewport(&vp);
}