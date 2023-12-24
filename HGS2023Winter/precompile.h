//============================================
//
// プリコンパイルヘッダー[precompile.h]
// Author：小原立暉
//
//============================================
#ifndef _PRECOMPILE_H_					// このマクロ定義がされていなかったら
#define _PRECOMPILE_H_					// 2重インクルード防止のマクロを定義する

// 必要なインクルードファイル
#include <windows.h>
#include <stdio.h>		// デバッグ表示に必要
#include "d3dx9.h"		// 描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)	//ビルド時の警告対処用マクロ

#ifdef _DEBUG

// メモリリーク出力用マクロ定義
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>			// メモリリーク検出用に必要

#endif

#endif