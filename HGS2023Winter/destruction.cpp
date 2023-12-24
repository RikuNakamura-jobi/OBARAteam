//=======================================
//
// 飛沫処理[destruction.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "destruction.h"
#include "renderer.h"
#include "texture.h"

//=======================================
// 無名名前空間
//=======================================
namespace
{
	const char* TEXTURE[CDestruction::TYPE_MAX] =		// テクスチャ
	{
		"data\\TEXTURE\\Platform\\Dstr001.png",		// トゲトゲ
		"data\\TEXTURE\\Platform\\Dstr002.png",		// フワフワ
		"data\\TEXTURE\\Platform\\Dstr003.png",		// 爆発
	};
}

//=========================
// コンストラクタ
//=========================
CDestruction::CDestruction() : CBillboard(CObject::TYPE_DESTRUCTION, CObject::PRIORITY_BLOCK)
{
	// 全ての値をクリアする
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 色
	m_state = STATE_ADD;						// 状態
	m_fAddAlpha = 0.0f;							// 加算する透明度
	m_fAlphaDest = 0.0f;						// 目的の透明度
}

//=========================
// デストラクタ
//=========================
CDestruction::~CDestruction()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CDestruction::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 色
	m_state = STATE_ADD;						// 状態
	m_fAddAlpha = 0.0f;							// 加算する透明度
	m_fAlphaDest = 0.0f;						// 目的の透明度

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CDestruction::Uninit(void)
{
	// 終了
	CBillboard::Uninit();
}

//=========================
// 更新処理
//=========================
void CDestruction::Update(void)
{
	// 透明度を加算する
	m_col.a += m_fAddAlpha;

	switch (m_state)
	{
	case STATE_ADD:		// 加算状態

		if (m_col.a >= m_fAlphaDest)
		{ // 目的の透明度を超えた場合

			// 透明度を補正する
			m_col.a = m_fAlphaDest;

			// 加算する透明度を反転させる
			m_fAddAlpha *= -1;

			// 減算状態にする
			m_state = STATE_SUB;
		}

		break;

	case STATE_SUB:		// 減算状態

		if (m_col.a <= 0.0f)
		{ // 寿命が一定数以下になった場合

			// 終了処理
			Uninit();

			// この先の処理を行わない
			return;
		}

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 頂点カラーの設定処理
	SetVtxColor(m_col);
}

//=========================
// 描画処理
//=========================
void CDestruction::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//αブレンディングを加算処理に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 描画処理
	CBillboard::DrawLightOff();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=========================
// 情報の設定処理
//=========================
void CDestruction::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col, const TYPE type, const int nLife)
{
	// スクロールの設定処理
	SetPos(pos);		// 位置設定
	SetSize(size);		// サイズ設定

	// 全ての値を設定する
	m_col = col;			// 色
	m_state = STATE_ADD;	// 状態
	m_fAlphaDest = col.a;	// 目的の透明度
	m_col.a = 0.0f;			// 透明度
	m_fAddAlpha = (m_fAlphaDest - m_col.a) / (nLife / 2);		// 加算する透明度

	// テクスチャの読み込み処理
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[type]));

	// 頂点情報の初期化
	SetVertex();

	// 頂点カラーの設定処理
	SetVtxColor(m_col);
}

//=========================
// 生成処理
//=========================
CDestruction* CDestruction::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col, const TYPE type, const int nLife)
{
	// ローカルオブジェクトを生成
	CDestruction* pDest = nullptr;	// プレイヤーのインスタンスを生成

	if (pDest == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pDest = new CDestruction;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pDest != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pDest->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pDest->SetData(pos, size, col, type, nLife);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pDest;
}