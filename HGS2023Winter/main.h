//============================================
//
// メインヘッダー[main.h]
// Author：小原立暉
//
//============================================
#ifndef _MAIN_H_//このマクロ定義がされていなかったら
#define _MAIN_H_//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include <time.h>		// ランダムに必要
#include "dinput.h"		// 入力処理に必要
#include "xaudio2.h"	// サウンド処理に必要
#include <assert.h>		// アサート処理に必要

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")		// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")	// [d3d9.lib]
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")	// システム時刻取得に必要
#pragma comment(lib,"dinput8.lib")	// 入力処理に必要

//マクロ定義
#define SCREEN_WIDTH				(1280)								// ウインドウの幅
#define SCREEN_HEIGHT				(720)								// ウインドウの高さ
#define FVF_VERTEX_2D				(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)	// 頂点フォーマット[2D]
#define FVF_VERTEX_3D				(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)	//頂点フォーマット[3D]
#define MAX_STRING					(256)								// 文字列の最大数
#define INIT_SHADOW					(-1)								// 影の初期化処理
#define NONE_TEXIDX					(-1)								// テクスチャのインデックスの初期値
#define NONE_COLLIDX				(-1)								// 当たり判定ポリゴンのインデックスの初期値
#define ALPHA_TEST_NUMBER			(0)									// アルファテストを有効にしたときの数値
#define MAX_MATERIAL				(16)								// マテリアルの種類
#define NONE_PARENT					(-1)								// 親無しの数値
#define NONE_D3DXVECTOR3			(D3DXVECTOR3(0.0f,0.0f,0.0f))		// D3DXVECTOR3の初期値
#define NONE_D3DXCOLOR				(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// D3DXCOLORの初期値

// 頂点データ(2D)の構造体
struct VERTEX_2D
{
	D3DXVECTOR3 pos;			//頂点座標
	float rhw;					//座標変換用係数(1.0fで固定)
	D3DCOLOR col;				//頂点カラー
	D3DXVECTOR2 tex;			//テクスチャ座標
};

//頂点情報[3D]の構造体を定義
struct VERTEX_3D
{
	D3DXVECTOR3 pos;			//頂点座標
	D3DXVECTOR3 nor;			//法線ベクトル
	D3DCOLOR col;				//頂点カラー
	D3DXVECTOR2 tex;			//テクスチャ座標
};

// プロトタイプ宣言
int GetFPS(void);				// FPSの取得処理

#endif