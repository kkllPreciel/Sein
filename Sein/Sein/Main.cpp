/**
 *	@file		Main.cpp
 *	@brief		�A�v���Ɋւ��鏈�����s���v���O�����\�[�X
 *	@author		kkllPreciel
 *	@date		2017/02/07
 *	@version	1.0
 */

// include
#include "Debugger.h"
#include <Windows.h>
#include "Direct3D12Device.h"

// ����̌x���̖���
#pragma warning ( disable : 4996 )
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

// �}�N��( M_PI�g�p�̈� )
#define _USE_MATH_DEFINES

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
#ifdef _CRTDBG_MAP_ALLOC
	// ���������[�N�����o������
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// TODO:���������[�N�e�X�g
	Sein::Direct3D12::Device* pDevice = new Sein::Direct3D12::Device();

	pDevice->Create();

	// HelloFrameBuffering sample(1280, 720, L"HelloWindow");
	// return Win32Frame::Run(&sample, hInstance, nCmdShow);

	return S_OK;
}