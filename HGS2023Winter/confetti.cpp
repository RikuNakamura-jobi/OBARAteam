//=======================================
//
// 紙吹雪処理[confetti.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "confetti.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//=========================
// コンストラクタ
//=========================
CConfetti::CConfetti() : CObject3D(CObject::TYPE_CONFETTI, CObject::PRIORITY_BLOCK)
{
	// 全ての値をクリアする
	m_rotMove = NONE_D3DXVECTOR3;		// 向きの移動量
	m_nLife = 0;						// 寿命
	m_fGravity = 0.0f;					// 重力
}

//=========================
// デストラクタ
//=========================
CConfetti::~CConfetti()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CConfetti::Init(void)
{
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_rotMove = NONE_D3DXVECTOR3;		// 向きの移動量
	m_nLife = 0;						// 寿命
	m_fGravity = 0.0f;					// 重力

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CConfetti::Uninit(void)
{
	// 終了
	CObject3D::Uninit();
}

//=========================
// 更新処理
//=========================
void CConfetti::Update(void)
{
	// 向きを取得する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// 位置を下げる
	pos.y -= m_fGravity;

	// 向きを加算する
	rot += m_rotMove;

	// 向きの正規化
	useful::RotNormalize(&rot.x);
	useful::RotNormalize(&rot.y);
	useful::RotNormalize(&rot.z);

	// 向きを設定する
	SetPos(pos);
	SetRot(rot);

	if (m_nLife > 0)
	{ // 寿命が 0 超過の場合

		// 寿命を減算する
		m_nLife--;
	}

	if (m_nLife <= 0)
	{ // 寿命が0以下の場合

		// 終了処理
		Uninit();

		// この先の処理を行わない
		return;
	}

	// 頂点座標の設定処理
	SetVertex();
}

//=========================
// 描画処理
//=========================
void CConfetti::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// カリングの設定をOFFにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 描画処理
	CObject3D::DrawLightOff();

	// カリングの設定をONにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=========================
// 情報の設定処理
//=========================
void CConfetti::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXVECTOR3& rotMove, const float fGravity, const D3DXCOLOR& col, const int nLife)
{
	// スクロールの設定処理
	SetPos(pos);					// 位置設定
	SetPosOld(pos);					// 位置設定
	SetRot(NONE_D3DXVECTOR3);		// 向き設定
	SetSize(size);					// サイズ設定

	// 全ての値を設定する
	m_rotMove = rotMove;		// 向きの移動量
	m_nLife = nLife;			// 寿命
	m_fGravity = fGravity;		// 重力

	// 頂点座標の設定処理
	SetVertex();

	// 頂点カラーの設定処理
	SetVtxColor(col);
}

//=========================
// 生成処理
//=========================
CConfetti* CConfetti::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXVECTOR3& rotMove, const float fGravity, const D3DXCOLOR& col, const int nLife)
{
	// ローカルオブジェクトを生成
	CConfetti* pConfetti = nullptr;	// プレイヤーのインスタンスを生成

	if (pConfetti == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pConfetti = new CConfetti;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pConfetti != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pConfetti->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pConfetti->SetData(pos, size, rotMove, fGravity, col, nLife);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 紙吹雪のポインタを返す
	return pConfetti;
}