/**
 *	@file		Main.cpp
 *	@brief		アプリに関する処理を行うプログラムソース
 *	@author		kkllPreciel
 *	@date		2017/02/07
 *	@version	1.0
 */

// include
#include "Debugger.h"
#include <Windows.h>
#include "Direct3D12Device.h"

// 特定の警告の無視
#pragma warning ( disable : 4996 )
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

// マクロ( M_PI使用の為 )
#define _USE_MATH_DEFINES

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
#ifdef _CRTDBG_MAP_ALLOC
	// メモリリークを検出させる
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// TODO:メモリリークテスト
	Sein::Direct3D12::Device* pDevice = new Sein::Direct3D12::Device();

	pDevice->Create();

	// HelloFrameBuffering sample(1280, 720, L"HelloWindow");
	// return Win32Frame::Run(&sample, hInstance, nCmdShow);

	return S_OK;
}