//============================================
//
// マネージャーのヘッダー[manager.h]
// Author：小原立暉
//
//============================================
#ifndef _MANAGER_H_			//このマクロ定義がされていなかったら
#define _MANAGER_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"
#include "scene.h"
#include "camera.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CRenderer;		// レンダラー
class CInputKeyboard;	// キーボード入力
class CInputGamePad;	// ゲームパッド
class CSound;			// サウンド
class CFile;			// ファイル
class CDebugProc;		// デバッグ表示
class CCamera;			// カメラ
class CLight;			// ライト
class CTexture;			// テクスチャ
class CFade;			// フェード

//--------------------------------------------
// クラス(マネージャークラス)
//--------------------------------------------
class CManager
{
public:						// 誰でもアクセスできる

	CManager();				// コンストラクタ
	~CManager();			// デストラクタ

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);		// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

	HRESULT TexLoad(void);						// テクスチャの読み込み処理
	void TexUnload(void);						// テクスチャの破棄処理

	// セット・ゲット関係
	CRenderer* GetRenderer(void);			// レンダラーの取得処理
	CInputKeyboard* GetInputKeyboard(void);	// キーボードの取得処理
	CInputGamePad* GetInputGamePad(void);	// ゲームパッドの取得処理
	CSound* GetSound(void);					// サウンドの取得処理
	CFile* GetFile(void);					// ファイルの取得処理
	CDebugProc* GetDebugProc(void);			// デバッグ表示の取得処理
	CCamera* GetCamera(void);				// カメラ表示の取得処理
	CLight* GetLight(void);					// ライトの取得処理
	CTexture* GetTexture(void);				// テクスチャの取得処理
	CFade* GetFade(void);					// フェードの取得処理
	void SetMode(const CScene::MODE mode);	// モードの設定処理
	CScene::MODE GetMode(void);				// モードの取得処理

	// 静的メンバ関数
	static CManager* Create(void);			// マネージャーの生成処理
	static CManager* Get(void);				// マネージャーの取得処理

private:					// 自分だけアクセスできる

	// メンバ変数
	CRenderer* m_pRenderer;					// レンダラーの情報
	CInputKeyboard* m_pInputKeyboard;		// キーボードの情報
	CInputGamePad* m_pInputGamePad;			// ゲームパッドの情報
	CSound* m_pSound;						// サウンドの情報
	CFile* m_pFile;							// ファイルの情報
	CDebugProc* m_pDebugProc;				// デバッグ表示の情報
	CCamera* m_pCamera;						// カメラの情報
	CLight* m_pLight;						// ライトの情報
	CTexture* m_pTexture;					// テクスチャの情報
	CScene* m_pScene;						// シーンの情報
	CFade* m_pFade;							// フェードの情報

#ifdef _DEBUG		// デバッグ時のみ

	bool m_bWire;						// ワイヤーフレーム状況

#endif

	// 静的メンバ変数
	static CManager* m_pManager;			// マネージャーの情報
};

#endif