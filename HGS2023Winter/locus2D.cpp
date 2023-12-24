//=======================================
//
// 2D残像のメイン処理[locus2D.cpp]
// Author 小原立暉
//
//=======================================
#include "locus2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//---------------------------------------
// マクロ定義
//---------------------------------------

//=========================
// コンストラクタ
//=========================
CLocus2D::CLocus2D() : CObject2D(CObject2D::TYPE_LOCUS, CObject::PRIORITY_PLAYER)
{
	// 全ての値をクリアする
	m_nLife = 0;			// 寿命
	m_fSub = 0.0f;			// 透明度の減衰量
	m_fAlpha = 0.0f;		// 透明度
}

//=========================
// デストラクタ
//=========================
CLocus2D::~CLocus2D()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CLocus2D::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_nLife = 0;			// 寿命
	m_fSub = 0.0f;			// 透明度の減衰量
	m_fAlpha = 0.0f;		// 透明度

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CLocus2D::Uninit(void)
{
	// 終了
	CObject2D::Uninit();
}

//=========================
// 更新処理
//=========================
void CLocus2D::Update(void)
{
	// 寿命を減算する
	m_nLife--;

	// 透明度を減算する
	m_fAlpha -= m_fSub;

	if (m_fAlpha <= 0.0f)
	{ // 透明度が 0.0f 以下になった場合

		// 透明度を補正する
		m_fAlpha = 0.0f;
	}

	if (m_nLife <= 0)
	{ // 寿命が 0 以下になった場合

		// 終了処理
		Uninit();

		// この先の処理を行わない
		return;
	}

	// 頂点情報の初期化
	SetVertex();

	// 頂点カラーの設定処理
	SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
}

//=========================
// 描画処理
//=========================
void CLocus2D::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}
//=========================
// 情報の設定処理
//=========================
void CLocus2D::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fAlpha, const int nLife, const int nTexIdx)
{
	// スクロールの設定処理
	SetPos(pos);			// 位置設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));			// 向き設定
	SetSize(size);			// サイズ設定
	SetLength();			// 長さ設定
	SetAngle();				// 方向設定

	// 全ての値を初期化する
	m_nLife = nLife;		// 寿命
	m_fSub = fAlpha / (float)(nLife);	// 透明度の減衰量
	m_fAlpha = fAlpha;		// 透明度

	// 頂点情報の初期化
	SetVertex();

	// テクスチャ情報の設定処理
	SetVtxTexture();

	// テクスチャの読み込み処理
	BindTexture(nTexIdx);
}

//=========================
// 生成処理
//=========================
CLocus2D* CLocus2D::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fAlpha, const int nLife, const int nTexIdx)
{
	// ローカルオブジェクトを生成
	CLocus2D* pLocus2D = nullptr;	// プレイヤーのインスタンスを生成

	if (pLocus2D == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pLocus2D = new CLocus2D;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pLocus2D != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pLocus2D->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pLocus2D->SetData(pos, size, fAlpha, nLife, nTexIdx);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pLocus2D;
}