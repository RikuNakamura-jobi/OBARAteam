//============================================
//
// マネージャーのメイン処理[manager.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "file.h"
#include "debugproc.h"
#include "texture.h"
#include "objectX.h"
#include "camera.h"
#include "light.h"
#include "fade.h"

#include "elevation_manager.h"
#include "mesh_manager.h"

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
CManager* CManager::m_pManager = nullptr;			// マネージャーの情報

//=========================================
// コンストラクタ
//=========================================
CManager::CManager()
{

}

//=========================================
// デストラクタ
//=========================================
CManager::~CManager()
{

}

//=========================================
//初期化処理
//=========================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// マネージャーの生成処理
	CElevationManager::Create();		// 起伏マネージャー
	CMeshManager::Create();				// メッシュマネージャー

	if (m_pFile == nullptr)
	{ // ファイルへのポインタが nullptr の場合

		// ファイルのメモリを確保する
		m_pFile = new CFile;
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pDebugProc == nullptr)
	{ // デバッグ表示へのポインタが nullptr の場合

		// デバッグ表示のメモリを確保する
		m_pDebugProc = new CDebugProc;
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pRenderer == nullptr)
	{ // レンダラーへのポインタが NULL の場合

		// レンダラーのメモリを確保する
		m_pRenderer = new CRenderer;
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pInputKeyboard == nullptr)
	{ // キーボードへのポインタが NULL の場合

		// キーボードのメモリを確保する
		m_pInputKeyboard = new CInputKeyboard;
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pInputGamePad == nullptr)
	{ // ゲームパッドへのポインタが NULL の場合

		// ゲームパッドのメモリを確保する
		m_pInputGamePad = new CInputGamePad;
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pSound == nullptr)
	{ // サウンドへのポインタが NULL の場合

		// サウンドのメモリを確保する
		m_pSound = new CSound;
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pCamera == nullptr)
	{ // カメラへのポインタが NULL の場合

		// カメラのメモリを確保する
		m_pCamera = new CCamera;
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pLight == nullptr)
	{ // ライトへのポインタが NULL の場合

		// ライトのメモリを確保する
		m_pLight = new CLight;
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pTexture == nullptr)
	{ // テクスチャへのポインタが NULL の場合

		// メモリを確保する
		m_pTexture = new CTexture;
	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	if (m_pRenderer != nullptr)
	{ // 確保に成功していた場合

		// 初期化処理
		if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
		{ // 初期化処理が失敗した場合

			// 失敗を返す
			return E_FAIL;
		}
	}
	else
	{ // 確保に失敗していた場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pInputKeyboard != nullptr)
	{ // 確保に成功していた場合

		// 初期化処理
		if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
		{ // 初期化処理が失敗した場合

			// 失敗を返す
			return E_FAIL;
		}
	}
	else
	{ // 確保に失敗していた場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pInputGamePad != nullptr)
	{ // 確保に成功していた場合

		// 初期化処理
		if (FAILED(m_pInputGamePad->Init(hInstance, hWnd)))
		{ // 初期化処理が失敗した場合

			// 失敗を返す
			return E_FAIL;
		}
	}
	else
	{ // 確保に失敗していた場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pSound != nullptr)
	{ // 確保に成功していた場合

		// 初期化処理
		if (FAILED(m_pSound->Init(hWnd)))
		{ // 初期化処理が失敗した場合

			// 失敗を返す
			return E_FAIL;
		}
	}
	else
	{ // 確保に失敗していた場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pDebugProc != nullptr)
	{ // 確保に成功していた場合

		// 初期化処理
		m_pDebugProc->Init();
	}
	else
	{ // 確保に失敗していた場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pCamera != nullptr)
	{ // 確保に成功していた場合

		// 初期化処理
		if (FAILED(m_pCamera->Init()))
		{ // カメラの初期化に失敗した場合

			// 失敗を返す
			return E_FAIL;
		}
	}
	else
	{ // 確保に失敗していた場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pLight != nullptr)
	{ // 確保に成功していた場合

		// 初期化処理
		if (FAILED(m_pLight->Init()))
		{ // ライトの初期化に失敗した場合

			// 失敗を返す
			return E_FAIL;
		}
	}
	else
	{ // 確保に失敗していた場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pTexture != nullptr)
	{ // 確保に成功していた場合

		// 初期化処理
		if (FAILED(m_pTexture->Init()))
		{ // テクスチャの初期化に失敗した場合

			// 失敗を返す
			return E_FAIL;
		}
	}
	else
	{ // 確保に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// テクスチャの読み込み処理
	if (FAILED(TexLoad()))
	{ // 読み込みに失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	if (m_pFade == nullptr)
	{ // フェードへのポインタが nullptr の場合

#ifdef _DEBUG

		// フェードのメモリを確保する
		m_pFade = m_pFade->Create(CScene::MODE_LOGO);
#else

		// シーンのメモリを確保する
		m_pFade = m_pFade->Create(CScene::MODE_LOGO);
#endif 

	}
	else
	{ // ポインタが使われていた場合

		// 停止
		assert(false);
	}

	// 成功を返す
	return S_OK;
}

//=============================================
//終了処理
//=============================================
void CManager::Uninit(void)
{
	if (m_pSound != nullptr)
	{ // サウンドのメモリが確保されていた場合

		// サウンドの停止
		m_pSound->Stop();
	}

	// テクスチャの破棄処理
	TexUnload();

	if (m_pRenderer != nullptr)
	{ // レンダラーが NULL じゃない場合

		// 終了処理
		m_pRenderer->Uninit();

		// メモリを開放する
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	if (m_pInputKeyboard != nullptr)
	{ // キーボードが NULL じゃない場合

		// 終了処理
		m_pInputKeyboard->Uninit();

		// メモリを開放する
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	if (m_pInputGamePad != nullptr)
	{ // ゲームパッドが NULL じゃない場合

		// 終了処理
		m_pInputGamePad->Uninit();

		// メモリを開放する
		delete m_pInputGamePad;
		m_pInputGamePad = nullptr;
	}

	if (m_pSound != nullptr)
	{ // サウンドが NULL じゃない場合

		// 終了処理
		m_pSound->Uninit();

		// メモリを開放する
		delete m_pSound;
		m_pSound = nullptr;
	}

	if (m_pFile != nullptr)
	{ // ファイルが NULL じゃない場合

		// 終了処理
		m_pFile->Uninit();

		// メモリを開放する
		delete m_pFile;
		m_pFile = nullptr;
	}

	if (m_pDebugProc != nullptr)
	{ // デバッグ表示が NULL じゃない場合

		// 終了処理
		m_pDebugProc->Uninit();

		// メモリを開放する
		delete m_pDebugProc;
		m_pDebugProc = nullptr;
	}

	if (m_pCamera != nullptr)
	{ // カメラが NULL じゃない場合

		// 終了処理
		m_pCamera->Uninit();

		// メモリを開放する
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pLight != nullptr)
	{ // ライトが NULL じゃない場合

		// 終了処理
		m_pLight->Uninit();

		// メモリを開放する
		delete m_pLight;
		m_pLight = nullptr;
	}

	if (m_pTexture != nullptr)
	{ // テクスチャが NULL じゃない場合

		// テクスチャの破棄処理
		m_pTexture->Unload();

		// メモリを開放する
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pFade != nullptr)
	{ // フェードが NULL じゃない場合

		// フェードの破棄処理
		m_pFade->Uninit();

		// メモリを開放する
		delete m_pFade;
		m_pFade = nullptr;
	}

	// シーンへのポインタを NULL にする
	m_pScene = nullptr;

	// マネージャーの終了処理
	CElevationManager::Get()->Uninit();		// 起伏マネージャー
	CMeshManager::Get()->Uninit();			// メッシュマネージャー

	// マネージャーのメモリを解放する
	delete m_pManager;
	m_pManager = nullptr;
}

//======================================
//更新処理
//======================================
void CManager::Update(void)
{
	if (m_pFade != nullptr)
	{ // フェードが NULL じゃない場合

		// フェードの更新
		m_pFade->Update();
	}

	if (m_pInputKeyboard != nullptr)
	{ // キーボードが NULL じゃない場合

		// キーボードの更新
		m_pInputKeyboard->Update();
	}

	if (m_pInputGamePad != nullptr)
	{ // ゲームパッドが NULL じゃない場合

		// ゲームパッドの更新
		m_pInputGamePad->Update();
	}

	if (m_pCamera != nullptr)
	{ // カメラが NULL じゃない場合

		// カメラの更新
		m_pCamera->Update();
	}

	if (m_pLight != nullptr)
	{ // ライトが NULL じゃない場合

		// ライトの更新
		m_pLight->Update();
	}

#ifdef _DEBUG		// デバッグ用

	if (m_pDebugProc != nullptr)
	{ // デバッグ表示が NULL じゃない場合

		// デバッグ表示の更新
		m_pDebugProc->Update();
	}

	if (m_pInputKeyboard->GetTrigger(DIK_F6) == true)
	{ // F6キーを押した場合

		// ワイヤーフレームのON/OFF
		m_bWire = m_bWire ? false : true;

		if (m_bWire == true)
		{ // ワイヤーモードONの場合

			// ワイヤーフレームをONにする
			m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else
		{ // ワイヤーモードOFFの場合

			// ワイヤーフレームをOFFにする
			m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
	}

#endif

	if (m_pScene != nullptr)
	{ // シーンが NULL じゃない場合

		// シーンの更新処理
		m_pScene->Update();
	}

	//if (m_pRenderer != nullptr)
	//{ // レンダラーが NULL じゃない場合

	//	// 更新処理
	//	m_pRenderer->Update();
	//}

	m_pDebugProc->Print("モード：%d", m_pScene->GetMode());
}

//======================================
//描画処理
//======================================
void CManager::Draw(void)
{
	if (m_pRenderer != nullptr)
	{ // レンダラーが NULL じゃない場合

		// 描画処理
		m_pRenderer->Draw();
	}

	if (m_pScene != nullptr)
	{ // シーンが NULL じゃない場合

		// シーンの描画処理
		m_pScene->Draw();
	}
}

//======================================
// テクスチャの読み込み処理
//======================================
HRESULT CManager::TexLoad(void)
{
	// Xファイルの初期化処理
	CXFile::Init();

	// テクスチャのロード処理
	m_pTexture->Load();

	// 成功を返す
	return S_OK;
}

//======================================
// テクスチャの破棄処理
//======================================
void CManager::TexUnload(void)
{
	// Xファイルの終了処理
	CXFile::Uninit();

	// 全てのオブジェクトの破棄処理
	CObject::ReleaseAll();
}

//======================================
// レンダラーの取得処理
//======================================
CRenderer* CManager::GetRenderer(void)
{
	// レンダラーの情報を返す
	return m_pRenderer;
}

//======================================
// キーボードの取得処理
//======================================
CInputKeyboard* CManager::GetInputKeyboard(void)
{
	// キーボードの情報を返す
	return m_pInputKeyboard;
}

//======================================
// ゲームパッドの取得処理
//======================================
CInputGamePad* CManager::GetInputGamePad(void)
{
	// ゲームパッドの情報を返す
	return m_pInputGamePad;
}

//======================================
// サウンドの取得処理
//======================================
CSound* CManager::GetSound(void)
{
	// サウンドの情報を返す
	return m_pSound;
}

//======================================
// ファイルの取得処理
//======================================
CFile* CManager::GetFile(void)
{
	// ファイルの情報を返す
	return m_pFile;
}

//======================================
// デバッグ表示の取得処理
//======================================
CDebugProc* CManager::GetDebugProc(void)
{
	// デバッグ表示の情報を返す
	return m_pDebugProc;
}

//======================================
// カメラ表示の取得処理
//======================================
CCamera* CManager::GetCamera(void)
{
	// カメラの情報を返す
	return m_pCamera;
}

//======================================
// ライトの取得処理
//======================================
CLight* CManager::GetLight(void)
{
	// ライトの情報を返す
	return m_pLight;
}

//======================================
// テクスチャの取得処理
//======================================
CTexture* CManager::GetTexture(void)
{
	// テクスチャの情報を返す
	return m_pTexture;
}

//======================================
// フェードの取得処理
//======================================
CFade* CManager::GetFade(void)
{
	// フェードの情報を返す
	return m_pFade;
}

//======================================
// モードの設定処理
//======================================
void CManager::SetMode(const CScene::MODE mode)
{
	if (m_pSound != nullptr)
	{ // サウンドが NULL じゃない場合

		// サウンドの停止処理
		m_pSound->Stop();
	}

	// 全てのオブジェクトの破棄処理
	CObject::ReleaseAll();

	// シーンを NULL にする
	m_pScene = nullptr;

	if (m_pScene == nullptr)
	{ // シーンが NULL じゃない場合

		// 生成処理
		m_pScene = m_pScene->Create(mode);
	}
}

//======================================
// モードの取得処理
//======================================
CScene::MODE CManager::GetMode(void)
{
	// モードを返す
	return m_pScene->GetMode();
}

//======================================
// マネージャーの生成処理
//======================================
CManager* CManager::Create(void)
{
	if (m_pManager == nullptr)
	{ // マネージャーが NULL の場合

		// マネージャーを生成する
		m_pManager = new CManager;

		// マネージャーの情報を返す
		return m_pManager;
	}
	else
	{ // 上記以外

		// マネージャーの情報を返す
		return m_pManager;
	}
}

//======================================
// マネージャーの取得処理
//======================================
CManager* CManager::Get(void)
{
	if (m_pManager != nullptr)
	{ // マネージャーの NULL じゃない場合

		// マネージャーの情報を返す
		return m_pManager;
	}
	else
	{ // 上記以外

		// 停止
		assert(false);

		// マネージャーを生成する
		return CManager::Create();
	}
}