//============================================
//
// マネージャーのメイン処理[manager.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "renderer.h"
#include "logo.h"
#include "logo_mark.h"
#include "input.h"
#include "fade.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define LOGO_TRANSITION_COUNT		(120)		// ロゴ画面の遷移するカウント

//=========================================
// コンストラクタ
//=========================================
CLogo::CLogo() : CScene(TYPE_NONE, PRIORITY_BG)
{
	// 全ての情報をクリアする
	m_nTransCount = 0;			// 遷移カウント
}

//=========================================
// デストラクタ
//=========================================
CLogo::~CLogo()
{

}

//=========================================
//初期化処理
//=========================================
HRESULT CLogo::Init(void)
{
	// シーンの初期化処理
	CScene::Init();

	// 全ての情報を初期化する
	m_nTransCount = 0;			// 遷移カウント

	// ロゴマークの生成処理
	CLogoMark::Create();

	// 成功を返す
	return S_OK;
}

//=============================================
//終了処理
//=============================================
void CLogo::Uninit(void)
{
	// 破棄処理
	Release();
}

//======================================
//更新処理
//======================================
void CLogo::Update(void)
{
	// 遷移カウントを加算する
	m_nTransCount++;

	if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true ||
		m_nTransCount >= LOGO_TRANSITION_COUNT)
	{ // ENTERキーを押した場合

		// タイトルに遷移する
		CManager::Get()->GetFade()->SetFade(CScene::MODE_TITLE);

		// この先の処理を行わない
		return;
	}

	if (CManager::Get()->GetRenderer() != nullptr)
	{ // レンダラーが NULL じゃない場合

		// 更新処理
		CManager::Get()->GetRenderer()->Update();
	}
}

//======================================
//描画処理
//======================================
void CLogo::Draw(void)
{

}