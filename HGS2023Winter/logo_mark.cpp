//============================================
//
// ロゴマーク処理[logo_mark.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "logo_mark.h"
#include "object2D.h"
#include "texture.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define LOGO_SCREEN_SIZE		(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))		// ロゴ画面のスクリーンのサイズ
#define LOGO_MARK_SIZE			(D3DXVECTOR3(200.0f, 200.0f, 0.0f))									// ロゴ画面のマークのサイズ
#define LOGO_MARK_TEXTURE		"data/TEXTURE/ObaraArcadia2.png"									// ロゴマークのテクスチャ

//============================
// コンストラクタ
//============================
CLogoMark::CLogoMark() : CObject(CObject::TYPE_LOGOMARK, PRIORITY_UI)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_aLogoMark[nCnt] = nullptr;			// 2Dオブジェクトの情報
	}
}

//============================
// デストラクタ
//============================
CLogoMark::~CLogoMark()
{

}

//============================
// 初期化処理
//============================
HRESULT CLogoMark::Init(void)
{
	for (int nCntUI = 0; nCntUI < TYPE_MAX; nCntUI++)
	{
		// 生成処理
		m_aLogoMark[nCntUI] = CObject2D::Create(CObject2D::TYPE_NONE, CObject::TYPE_NONE, CObject::PRIORITY_UI);

		// スクロールの設定処理
		m_aLogoMark[nCntUI]->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));				// 位置設定
		m_aLogoMark[nCntUI]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 向き設定
		m_aLogoMark[nCntUI]->SetLength();								// 長さ設定
		m_aLogoMark[nCntUI]->SetAngle();								// 方向設定

		switch (nCntUI)
		{
		case TYPE_SCREEN:

			// サイズ設定
			m_aLogoMark[nCntUI]->SetSize(LOGO_SCREEN_SIZE);

			// 色の設定処理
			m_aLogoMark[nCntUI]->SetVtxColor(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));

			break;

		case TYPE_MARK:

			// サイズ設定
			m_aLogoMark[nCntUI]->SetSize(LOGO_MARK_SIZE);

			// テクスチャの割り当て処理
			m_aLogoMark[nCntUI]->BindTexture(CManager::Get()->GetTexture()->Regist(LOGO_MARK_TEXTURE));

			break;

		default:

			// 停止
			assert(false);

			break;
		}

		// 頂点情報の初期化
		m_aLogoMark[nCntUI]->SetVertex();
	}

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CLogoMark::Uninit(void)
{
	for (int nCntUI = 0; nCntUI < TYPE_MAX; nCntUI++)
	{
		// 終了処理
		m_aLogoMark[nCntUI]->Uninit();
		m_aLogoMark[nCntUI] = nullptr;
	}

	// 破棄処理
	Release();
}

//============================
// 更新処理
//============================
void CLogoMark::Update(void)
{

}

//============================
// 描画処理
//============================
void CLogoMark::Draw(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_aLogoMark[nCnt] != nullptr)
		{ // 2Dオブジェクトの情報が NULL じゃない場合

			// 描画処理
			m_aLogoMark[nCnt]->Draw();
		}
	}
}

//============================
//生成処理
//============================
CLogoMark* CLogoMark::Create(void)
{
	// ローカルオブジェクトを生成
	CLogoMark* pLogoMark = nullptr;	// プレイヤーのインスタンスを生成

	if (pLogoMark == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pLogoMark = new CLogoMark;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pLogoMark != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pLogoMark->Init()))
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

	// ロゴマークのポインタを返す
	return pLogoMark;
}