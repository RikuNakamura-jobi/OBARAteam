//============================================
//
// シーンのメイン処理[scene.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "scene.h"

#include "logo.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "sound.h"

#include "manager.h"
#include "file.h"

//======================================
// コンストラクタ
//======================================
CScene::CScene() : CObject(TYPE_NONE, PRIORITY_BG)
{
	// コンストラクタの箱
	Box();
}

//======================================
// オーバーロードコンストラクタ
//======================================
CScene::CScene(TYPE type, PRIORITY priority) : CObject(type, priority)
{
	// コンストラクタの箱
	Box();
}

//======================================
// デストラクタ
//======================================
CScene::~CScene()
{

}

//======================================
// コンストラクタの箱
//======================================
void CScene::Box(void)
{
	// 全ての値をクリアする
	m_mode = MODE_LOGO;			// 現在のモード
}

//======================================
// 初期化処理
//======================================
HRESULT CScene::Init(void)
{
	// 成功を返す
	return S_OK;
}

//======================================
// 終了処理
//======================================
void CScene::Uninit(void)
{
	// 破棄処理
	Release();
}

//======================================
// 更新処理
//======================================
void CScene::Update(void)
{

}

//======================================
// 描画処理
//======================================
void CScene::Draw(void)
{

}

//======================================
// 情報の設定処理
//======================================
void CScene::SetData(const MODE mode)
{
	// モードを設定する
	m_mode = mode;

	if (CManager::Get()->GetSound() != nullptr)
	{ // サウンドが NULL じゃない場合

		// BGMを流す
		switch (m_mode)
		{
		case MODE_LOGO:

			break;

		case MODE_TITLE:

			// タイトルBGMを流す
			CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_BGM_TITLE);

			break;

		case MODE_TUTORIAL:

			// タイトルBGMを流す
			CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_BGM_TITLE);

			break;

		case MODE_GAME:

			// ゲームBGMを流す
			CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_BGM_GAME);

			break;

		case MODE_RESULT:

			// リザルトBGMを流す
			CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_BGM_RESULT);

			break;

		case MODE_RANKING:

			// ランキングBGMを流す
			CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_BGM_RANKING);

			break;

		default:

			// 停止
			assert(false);

			break;
		}
	}
}

//======================================
// モードの設定処理
//======================================
void CScene::SetMode(const MODE mode)
{
	// モードを設定する
	m_mode = mode;
}

//======================================
// モードの取得処理
//======================================
CScene::MODE CScene::GetMode(void) const
{
	// モードを返す
	return m_mode;
}

//======================================
// 生成処理
//======================================
CScene* CScene::Create(const MODE mode)
{
	// ローカルポインタ宣言
	CScene* pScene = nullptr;		// シーンへのポインタ

	if (pScene == nullptr)
	{ // シーンが NULL の場合

		switch (mode)
		{
		case MODE_LOGO:			// ロゴ画面

			// メモリを確保する
			pScene = new CLogo;

			break;

		case MODE_TITLE:		// タイトル画面

			// メモリを確保する
			pScene = new CTitle;

			break;

		case MODE_TUTORIAL:

			// メモリを確保する
			pScene = new CTutorial;

			break;

		case MODE_GAME:			// ゲーム画面

			// メモリを確保する
			pScene = new CGame;

			break;

		case MODE_RESULT:		// リザルト画面

			// メモリを確保する
			pScene = new CResult;

			break;

		case MODE_RANKING:		// ランキング画面

			// メモリを確保する
			pScene = new CRanking;

			break;

		default:

			// 停止
			assert(false);

			break;
		}

		if (pScene != nullptr)
		{ // シーンへのポインタが NULL じゃない場合

			// 初期化処理
			if (FAILED(pScene->Init()))
			{ // 失敗した場合

				// 停止
				assert(false);

				// NULL を返す
				return nullptr;
			}

			// 情報の設定処理
			pScene->SetData(mode);
		}
		else
		{ // シーンへのポインタが NULL の場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // シーンが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// シーンへのポインタを返す
	return pScene;
}