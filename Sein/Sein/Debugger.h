/**
 *	@file		Debugger.h
 *	@brief		デバッグに関するヘッダファイル
 *	@author		kkllPreciel
 *	@date		2017/02/08
 *	@version	1.0
 */

#pragma once

// デバッグ用(メモリリーク)
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC

// デバッグモード用include
#include <stdlib.h>
#include <crtdbg.h>

#endif