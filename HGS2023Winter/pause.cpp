//=======================================
//
// ポーズのメイン処理[pause.cpp]
// Author 小原立暉
//
//=======================================
#include "main.h"
#include "manager.h"
#include "object2D.h"
#include "game.h"
#include "pause.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "texture.h"
#include "useful.h"

//---------------------------------------
// マクロ定義
//---------------------------------------
// 位置関係
#define BACK_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))		// 背景の位置
#define PAUSEMENU_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))		// メニューの位置
#define CONTINUE_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 260.0f, 0.0f))					// コンティニューの位置
#define RESET_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 410.0f, 0.0f))					// リセットの位置
#define TITLE_POS				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 560.0f, 0.0f))					// タイトルの位置

// サイズ関係
#define BACK_SIZE				(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))		// 背景のサイズ
#define MENU_SIZE				(D3DXVECTOR3(SCREEN_WIDTH * 0.45f, SCREEN_HEIGHT * 0.45f, 0.0f))	// メニューのサイズ
#define CONTINUE_SIZE			(D3DXVECTOR3(360.0f, 64.0f, 0.0f))			// コンティニューのサイズ
#define RESET_SIZE				(D3DXVECTOR3(220.0f, 64.0f, 0.0f))			// リセットのサイズ
#define TITLE_SIZE				(D3DXVECTOR3(200.0f, 64.0f, 0.0f))			// タイトルのサイズ

// その他
#define PAUSE_BACK_COL			(D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.4f))			// 背景の色
#define PAUSE_ALPHA				(0.02f)					// 透明度の変化の数値
#define DECIDE_ALPHA			(0.7f)					// 決定した後の透明度の変化
#define NOCHOICE_ALPHA			(0.5f)					// 選んでいない選択肢の透明度
#define SIZEDEST_MAGNI			(1.2f)					// 目標のサイズの倍率
#define SIZEDEST_CORRECT_VALUE	(0.7f)					// 目標のサイズの補正の倍率
#define CHOICE_ALPHA_MIN		(0.4f)					// 選択肢の透明度の最小値
#define SELECT_REPEAT_COUNT		(15)					// 選択の時のリピートのカウント数

// 静的メンバ変数宣言
const char*CPause::c_apFilename[CPause::POLYGON_MAX] =	// テクスチャファイル名
{
	nullptr,								// 背景ポリゴン
	"data\\TEXTURE\\PauseMenu.png",			// ポーズメニューのテクスチャ
	"data\\TEXTURE\\PauseContinue.png",		// コンティニューのテクスチャ
	"data\\TEXTURE\\PauseReset.png",		// リセットのテクスチャ
	"data\\TEXTURE\\PauseTitle.png",		// タイトルのテクスチャ
};

const CPause::Info CPause::c_aPauseInfo[POLYGON_MAX] = 	// ポーズの情報
{
	{ BACK_POS ,BACK_SIZE },			// 背景ポリゴン
	{ PAUSEMENU_POS ,MENU_SIZE },		// ポーズメニュー
	{ CONTINUE_POS ,CONTINUE_SIZE },	// コンティニュー
	{ RESET_POS ,RESET_SIZE },			// リトライ
	{ TITLE_POS ,TITLE_SIZE },			// タイトル
};

//==========================================
// コンストラクタ
//==========================================
CPause::CPause() : CObject(TYPE_PAUSE, PRIORITY_PAUSE)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < POLYGON_MAX; nCnt++)
	{
		m_apObject[nCnt] = nullptr;						// ポリゴンの情報
	}

	m_PauseColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 選択中の選択肢の色
	sizeDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 目標のサイズ
	m_PauseMenu = MENU_CONTINUE;;						// ポーズメニュー
	m_nPauseCounter = 0;								// カウンター
	m_fPauseAlpha = 0.0f;								// 透明度を変化させる変数
	m_bPause = false;									// ポーズ状況
	m_bDisp = true;										// 描画状況
}

//==========================================
// デストラクタ
//==========================================
CPause::~CPause()
{

}

//==========================================
// ポーズの初期化処理
//==========================================
HRESULT CPause::Init(void)
{
	for (int nCnt = 0; nCnt < POLYGON_MAX; nCnt++)
	{
		if (m_apObject[nCnt] == nullptr)
		{ // オブジェクトが NULL の場合

			// ポリゴンの生成処理
			m_apObject[nCnt] = m_apObject[nCnt]->Create(CObject2D::TYPE_NONE, CObject::TYPE_NONE, PRIORITY_PAUSE);

			// 情報を設定する
			m_apObject[nCnt]->SetPos(c_aPauseInfo[nCnt].pos);			// 位置
			m_apObject[nCnt]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 向き
			m_apObject[nCnt]->SetSize(c_aPauseInfo[nCnt].size);			// サイズ
			m_apObject[nCnt]->SetLength();								// 長さ
			m_apObject[nCnt]->SetAngle();								// 方向

			// 頂点を設定する
			m_apObject[nCnt]->SetVertex();

			if (nCnt == POLYGON_BACK)
			{ // 背景ポリゴンの場合

				// 頂点カラーを設定する
				m_apObject[nCnt]->SetVtxColor(PAUSE_BACK_COL);
			}

			// テクスチャの割り当て処理
			m_apObject[nCnt]->BindTexture(CManager::Get()->GetTexture()->Regist(c_apFilename[nCnt]));
		}
	}

	// 全ての値を初期化する
	m_PauseMenu = MENU_CONTINUE;						// メニュー
	m_PauseColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 選択肢の色
	sizeDest = D3DXVECTOR3(c_aPauseInfo[m_PauseMenu].size.x * SIZEDEST_MAGNI, c_aPauseInfo[m_PauseMenu].size.y * SIZEDEST_MAGNI, 0.0f);			// 目標のサイズ
	m_nPauseCounter = 0;								// カウンター
	m_fPauseAlpha = PAUSE_ALPHA;						// 透明度の変化量
	m_bPause = false;									// ポーズ状況
	m_bDisp = true;										// 描画状況

	// 成功を返す
	return S_OK;
}

//========================================
// ポーズの終了処理
//========================================
void CPause::Uninit(void)
{
	for (int nCnt = 0; nCnt < POLYGON_MAX; nCnt++)
	{
		// 終了処理
		m_apObject[nCnt]->Uninit();
	}

	// 破棄処理
	Release();

	// ポーズのNULL化処理
	CGame::DeletePause();
}

//========================================
// ポーズの更新処理
//========================================
void CPause::Update(void)
{
	// カウンターを加算する
	m_nPauseCounter++;

	// ポーズの選択処理
	PauseSelect();

	// ポーズの決定処理
	if (PauseDecide() == true)
	{ // 別のモードになった場合

		// この先の処理を行わない
		return;
	}

	if (CManager::Get()->GetFade()->GetFade() != CFade::FADE_NONE &&
		CManager::Get()->GetFade()->GetFade() != CFade::FADE_IN)
	{ // フェードアウトしている場合

		// 透明度の変化を設定する
		m_fPauseAlpha = DECIDE_ALPHA;
	}

	// 透明度補正処理
	PauseAlphaCorrect();

	// 選択中の選択肢の透明度を変化させる
	m_PauseColor.a -= m_fPauseAlpha;

	// 頂点設定処理
	PauseVertex();
}

//=====================================
// ポーズの描画処理
//=====================================
void CPause::Draw(void)
{
// デバッグ用
#ifdef _DEBUG

	if (m_bDisp == true)
	{ // 描画状況が true だった場合

		for (int nCnt = 0; nCnt < POLYGON_MAX; nCnt++)
		{
			// 描画処理
			m_apObject[nCnt]->Draw();
		}
	}

#else

	for (int nCnt = 0; nCnt < POLYGON_MAX; nCnt++)
	{
		// 描画処理
		m_apObject[nCnt]->Draw();
	}

#endif
}

//=====================================
// サイズ補正処理
//=====================================
void CPause::SizeCorrect(int nIdx)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = m_apObject[nIdx]->GetSize();		// サイズを取得する

	// サイズの補正
	useful::Correct(sizeDest.x, &size.x, SIZEDEST_CORRECT_VALUE);
	useful::Correct(sizeDest.y, &size.y, SIZEDEST_CORRECT_VALUE);

	//サイズを更新する
	m_apObject[nIdx]->SetSize(size);
}

//=====================================
// ポーズ状況の設定処理
//=====================================
void CPause::SetPause(const bool bPause)
{
	// ポーズの状況を設定する
	m_bPause = bPause;

	if (m_bPause == false)
	{ // ポーズが false の場合

		// 選択しているものをコンティニューにする
		m_PauseMenu = MENU::MENU_CONTINUE;
	}
}

//=====================================
// ポーズ状況の取得処理
//=====================================
bool CPause::GetPause(void)
{
	// ポーズの状況を返す
	return m_bPause;
}

//=====================================
// 描画状況の切り替え処理
//=====================================
void CPause::ChangeDisp(void)
{
	// 描画状況を切り替える
	m_bDisp = m_bDisp ? false : true;
}

//=====================================
// ポーズの選択処理
//=====================================
void CPause::PauseSelect(void)
{
	if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_W) == true ||
		CManager::Get()->GetInputKeyboard()->GetRepeat(DIK_W, SELECT_REPEAT_COUNT) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_UP, 0) == true)
	{ // Wキーを押した場合

		// ポーズの設定
		m_PauseMenu = (MENU)((m_PauseMenu + (MENU_MAX - 1)) % MENU_MAX);

		// 透明度の変化を設定する
		m_fPauseAlpha = PAUSE_ALPHA;

		// 選択音を鳴らす
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);
	}

	if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_S) == true ||
		CManager::Get()->GetInputKeyboard()->GetRepeat(DIK_S, SELECT_REPEAT_COUNT) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_DOWN, 0) == true)
	{ // Sキーを押した場合

		// ポーズの設定
		m_PauseMenu = (MENU)((m_PauseMenu + 1) % MENU_MAX);

		// 透明度の変化を設定する
		m_fPauseAlpha = PAUSE_ALPHA;

		// 選択音を鳴らす
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);
	}

	// 目標のサイズの設定処理
	sizeDest = D3DXVECTOR3(c_aPauseInfo[m_PauseMenu + POLYGON_CONTINUE].size.x * SIZEDEST_MAGNI, c_aPauseInfo[m_PauseMenu + POLYGON_CONTINUE].size.y * SIZEDEST_MAGNI, 0.0f);
}

//=====================================
// ポーズの決定処理
//=====================================
bool CPause::PauseDecide(void)
{
	if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true)
	{ // ENTERキーを押した場合

		if (CManager::Get()->GetFade()->GetFade() == CFade::FADE_NONE)
		{ // フェードが行われていないとき

			switch (m_PauseMenu)
			{
			case MENU_CONTINUE:		// コンティニューを指していた場合

				break;

			case MENU_RESET:			// リトライを指していた場合

				// 終了処理
				Uninit();

				// モード設定(ゲームに移行)
				CManager::Get()->GetFade()->SetFade(CScene::MODE_GAME);

				break;

			case MENU_TITLE:			// タイトルを指していた場合

				// 終了処理
				Uninit();

				// モード設定(タイトルに移行)
				CManager::Get()->GetFade()->SetFade(CScene::MODE_TITLE);

				break;

			default:

				// 停止
				assert(false);

				break;
			}

			// ポーズを解除する
			m_bPause = false;
		}

		// 決定音を鳴らす
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_DECIDE);

		// true を返す
		return true;
	}

	// false を返す
	return false;
}

//=====================================
// ポーズの透明度補正処理
//=====================================
void CPause::PauseAlphaCorrect(void)
{
	if (m_PauseColor.a > 1.0f)
	{ // アルファ値が1.0fより上だった場合

		// アルファ値を1.0fに設定する
		m_PauseColor.a = 1.0f;

		// アルファ値の符号を逆に変える
		m_fPauseAlpha *= -1;
	}
	else if (m_PauseColor.a < CHOICE_ALPHA_MIN)
	{ // アルファ値が一定数未満だった場合

		// アルファ値を最小値に設定する
		m_PauseColor.a = CHOICE_ALPHA_MIN;

		// アルファ値の符号を逆に変える
		m_fPauseAlpha *= -1;
	}
}

//=====================================
// ポーズの頂点設定処理
//=====================================
void CPause::PauseVertex(void)
{
	for (int nPauseCnt = 0; nPauseCnt < MENU_MAX; nPauseCnt++)
	{
		if (m_PauseMenu == nPauseCnt)
		{ // 選択中の選択肢の場合

			// 色を設定する
			m_apObject[nPauseCnt + POLYGON_CONTINUE]->SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_PauseColor.a));

			// サイズの補正処理
			SizeCorrect(nPauseCnt + POLYGON_CONTINUE);
		}
		else
		{ // 選択していない選択肢の場合

			// 色を設定する
			m_apObject[nPauseCnt + POLYGON_CONTINUE]->SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, NOCHOICE_ALPHA));

			// サイズの設定処理
			m_apObject[nPauseCnt + POLYGON_CONTINUE]->SetSize(c_aPauseInfo[nPauseCnt + POLYGON_CONTINUE].size);
		}

		// 頂点座標の設定処理
		m_apObject[nPauseCnt + POLYGON_CONTINUE]->SetVertex();
	}
}

//=====================================
// 生成処理
//=====================================
CPause* CPause::Create(void)
{
	// ローカルオブジェクトを生成
	CPause* pPause = nullptr;	// プレイヤーのインスタンスを生成

	if (pPause == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pPause = new CPause;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pPause != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pPause->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// ポーズのポインタを返す
	return pPause;
}