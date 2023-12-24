//============================================
//
// リザルトのメイン処理[result.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "renderer.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "Objectmesh.h"

#include "file.h"
#include "camera.h"
#include "skybox.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------

//=========================================
// コンストラクタ
//=========================================
CResult::CResult() : CScene(TYPE_NONE, PRIORITY_BG)
{

}

//=========================================
// デストラクタ
//=========================================
CResult::~CResult()
{

}

//=========================================
//初期化処理
//=========================================
HRESULT CResult::Init(void)
{
	// シーンの初期化
	CScene::Init();

	// スカイボックスの生成処理
	CSkyBox::Create();

	// テキスト読み込み処理
	CMesh::TxtSet();

	// 成功を返す
	return S_OK;
}

//=============================================
//終了処理
//=============================================
void CResult::Uninit(void)
{
	// 破棄処理
	Release();
}

//======================================
//更新処理
//======================================
void CResult::Update(void)
{
	if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A,0) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true)
	{ // ENTERキーを押した場合

		// ランキングに遷移する
		CManager::Get()->GetFade()->SetFade(CScene::MODE_RANKING);

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
void CResult::Draw(void)
{

}