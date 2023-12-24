//============================================
//
// アニメーションのメイン処理[anim.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "objectAnim.h"
#include "renderer.h"
#include "manager.h"

//===========================================
// コンストラクタ
//===========================================
CAnim::CAnim() : CObject2D(CObject::TYPE_NONE, CObject::PRIORITY_BG)
{
	// コンストラクタの箱
	Box();
}

//===========================================
// オーバーロードコンストラクタ
//===========================================
CAnim::CAnim(CObject::TYPE type, PRIORITY priority) : CObject2D(type, priority)
{
	// コンストラクタの箱
	Box();
}

//===========================================
// デストラクタ
//===========================================
CAnim::~CAnim()
{

}

//===========================================
// コンストラクタの箱
//===========================================
void CAnim::Box(void)
{
	// 全ての値をクリアする
	m_nCounter = 0;				// アニメーションカウンター
	m_nPattern = 0;				// アニメーションパターン
	m_nReplayCount = 0;			// 再生カウント
	m_nReplayPattern = 0;		// 再生パターン
	m_fTexPattern = 0.0f;		// 再生パターン
}

//===========================================
// 初期化処理
//===========================================
HRESULT CAnim::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_nCounter = 0;				// アニメーションカウンター
	m_nPattern = 0;				// アニメーションパターン
	m_nReplayCount = 0;				// 再生カウント
	m_nReplayPattern = 0;			// 再生パターン
	m_fTexPattern = 0.0f;			// テクスチャの再生パターン

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CAnim::Uninit(void)
{
	// 終了
	CObject2D::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CAnim::Update(void)
{
	// カウントを加算する
	m_nReplayCount++;

	if (m_nReplayCount % m_nCounter == 0)
	{ // カウントが規定値に達した場合

		// 再生カウントを0にする
		m_nReplayCount = 0;

		// パターン数を移動する
		m_nReplayPattern = (m_nReplayPattern + 1) % m_nPattern;
	}

	// 頂点情報の設定処理
	SetVertexRot();

	// テクスチャの設定処理(アニメーションバージョン)
	SetVtxTextureAnim(m_fTexPattern, m_nReplayPattern);
}

//===========================================
// 描画処理
//===========================================
void CAnim::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//===========================================
// パターンの取得処理
//===========================================
int CAnim::GetPattern(void) const
{
	// 現在のパターンを返す
	return m_nReplayPattern;
}

//===========================================
// テクスチャのパターンの取得処理
//===========================================
float CAnim::GetTexPattern(void) const
{
	// テクスチャのパターンを返す
	return m_fTexPattern;
}

//===========================================
// アニメーションの設定処理
//===========================================
void CAnim::SetAnim(const int nCount, const int nPattern)
{
	// 情報を設定する
	m_nCounter = nCount;			// アニメーションカウンター
	m_nPattern = nPattern;			// アニメーションパターン
	m_fTexPattern = 1.0f / nPattern;	// テクスチャの再生パターン
}

//===========================================
// アニメーションの生成処理
//===========================================
CAnim* CAnim::Create(const CObject::TYPE type, const PRIORITY priority)
{
	// ローカルオブジェクトを生成
	CAnim* pAnim = nullptr;	// オブジェクト2Dのインスタンスを生成

	if (pAnim == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pAnim = new CAnim(type, priority);
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pAnim != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pAnim->Init()))
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

	// アニメーションのポインタを返す
	return pAnim;
}