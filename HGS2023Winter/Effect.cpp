//=======================================
//
//エフェクトのメイン処理[Effect.cpp]
//Author 小原立暉
//
//=======================================
#include "Effect.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//=======================================
// 無名名前空間
//=======================================
namespace
{
	const char* TEXTURE[CEffect::TYPE_MAX] =
	{
		"data\\TEXTURE\\Platform\\effect000.jpg",		// 通常
		"data\\TEXTURE\\Platform\\Smoke.tga",			// 炎系
		"data\\TEXTURE\\Platform\\Dust.tga",			// 埃
		"data\\TEXTURE\\Platform\\Rupture.tga",			// 破裂
		"data\\TEXTURE\\Platform\\Smoke.tga",			// 煙
		"data\\TEXTURE\\Platform\\PopSmoke.tga",		// 煙(アニメ調)
	};
	const float DUST_GRAVITY = 0.4f;		// 埃の重力
}

//=======================================
// マクロ定義
//=======================================


//=========================
// コンストラクタ
//=========================
CEffect::CEffect() : CBillboard(CObject::TYPE_EFFECT, CObject::PRIORITY_EFFECT)
{
	// 全ての値をクリアする
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
	m_nLife = 0;									// 寿命
	m_nLifeOrigin = 0;								// 元々の寿命
	m_fSub = 0.0f;									// 透明になる間隔
	m_fContra = 0.0f;								// 半径の縮む間隔
	m_type = TYPE_NONE;								// 種類
	m_bAdd = false;									// 加算合成状況
}

//=========================
// デストラクタ
//=========================
CEffect::~CEffect()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CEffect::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
	m_nLife = 0;									// 寿命
	m_nLifeOrigin = 0;								// 元々の寿命
	m_fSub = 0.0f;									// 透明になる間隔
	m_fContra = 0.0f;								// 半径の縮む間隔
	m_type = TYPE_NONE;								// 種類
	m_bAdd = false;									// 加算合成状況

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CEffect::Uninit(void)
{
	// 終了
	CBillboard::Uninit();
}

//=========================
// 更新処理
//=========================
void CEffect::Update(void)
{
	switch (m_type)
	{
	case TYPE_NONE:			// 無し

		// 移動処理
		Move();

		// 透明度の減算処理
		SubAlpha();

		// サイジング処理
		Sizing();

		break;

	case TYPE_FIRE:			// 炎

		// 炎の更新処理
		FireProcess();

		// 移動処理
		Move();

		// 透明度の減算処理
		SubAlpha();

		// サイジング処理
		Sizing();

		break;

	case TYPE_DUST:			// 埃

		// 重力を足す
		m_move.y -= DUST_GRAVITY;

		// 移動処理
		Move();

		// 透明度の減算処理
		SubAlpha();

		// サイジング処理
		Sizing();

		break;

	case TYPE_RUPTURE:		// 破裂

		// 破裂の更新処理
		RuptureProcess();

		// 移動処理
		Move();

		// 透明度の減算処理
		SubAlpha();

		// サイジング処理
		Sizing();

		break;

	case TYPE_SMOKE:		// 煙

		// 移動処理
		Move();

		// 透明度の減算処理
		SubAlpha();

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 寿命を減らしていく
	m_nLife--;

	if (m_nLife <= 0)
	{ // エフェクトの寿命が尽きた時

		// 破棄処理
		Uninit();

		// この先の処理を行わない
		return;
	}

	// 頂点座標の設定処理
	SetVertex();

	// 色の設定処理
	SetVtxColor(m_col);
}

//=========================
// 描画処理
//=========================
void CEffect::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	if (m_bAdd == true)
	{ // 加算合成する場合

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
	else
	{ // 上記以外

		// 描画処理
		CBillboard::DrawLightOff();
	}
}

//=========================
// 情報の設定処理
//=========================
void CEffect::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int nLife, const float fRadius, const TYPE type, const D3DXCOLOR& col, const bool bAdd)
{
	// スクロールの設定処理
	SetPos(pos);			// 位置設定
	SetSize(D3DXVECTOR3(fRadius, fRadius, 0.0f));	// サイズ設定

	// 全ての値を初期化する
	m_type = type;			// 種類
	m_nLife = nLife;		// 寿命
	m_nLifeOrigin = nLife;	// 元々の寿命
	m_move = move;			// 移動量
	m_col = col;			// 色
	m_bAdd = bAdd;			// 加算合成状況

	m_fSub = m_col.a * (1.0f / m_nLife);			// 透明になる間隔
	m_fContra = fRadius * (1.0f / m_nLife);			// 半径の縮む間隔

	// 頂点情報の初期化
	SetVertex();

	// 色の設定処理
	SetVtxColor(m_col);

	// テクスチャの読み込み処理
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[m_type]));
}

//=========================
// 生成処理
//=========================
CEffect* CEffect::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int nLife, const float fRadius, const TYPE type, const D3DXCOLOR& col, const bool bAdd)
{
	// ローカルオブジェクトを生成
	CEffect* pEffect = nullptr;	// プレイヤーのインスタンスを生成

	if (pEffect == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pEffect = new CEffect;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pEffect != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pEffect->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pEffect->SetData(pos, move, nLife, fRadius, type, col, bAdd);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pEffect;
}

//=========================
// 移動処理
//=========================
void CEffect::Move(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する

	// 移動する
	pos += m_move;

	// 位置を更新する
	SetPos(pos);
}

//=========================
// 透明度減算処理
//=========================
void CEffect::SubAlpha(void)
{
	// 透明度を減算させる
	m_col.a -= m_fSub;

	if (m_col.a <= 0.0f)
	{ // 透明度が0.0f以下の場合

		// 透明度を補正する
		m_col.a = 0.0f;
	}
}

//=========================
// サイジング処理
//=========================
void CEffect::Sizing(void)
{
	// ローカル変数を宣言
	D3DXVECTOR3 size = GetSize();	// サイズを取得する

	// サイズを縮める
	size.x -= m_fContra;
	size.y -= m_fContra;

	if (size.x <= 0.0f)
	{ // サイズが0.0f以下の場合

		// サイズを補正する
		size.x = 0.0f;
	}

	if (size.y <= 0.0f)
	{ // サイズが0.0f以下の場合

		// サイズを補正する
		size.y = 0.0f;
	}

	// サイズを更新する
	SetSize(size);
}

//=========================
// 爆発の処理
//=========================
void CEffect::FireProcess(void)
{
	// G値に加算する
	m_col.g += m_fSub;

	if (m_col.g >= 1.0f)
	{ // 色が1.0f以上になった場合

		// G値を固定する
		m_col.g = 1.0f;
	}
}

//=========================
// 破裂の処理
//=========================
void CEffect::RuptureProcess(void)
{
	if (m_nLife >= m_nLifeOrigin / 2)
	{ // 寿命が一定以外の場合

		float f;

		f = m_move.x / ((m_nLifeOrigin / 2) - (m_nLifeOrigin - m_nLife));

		m_move.x -= f;

		f = m_move.y / ((m_nLifeOrigin / 2) - (m_nLifeOrigin - m_nLife));

		m_move.y -= f;

		f = m_move.z / ((m_nLifeOrigin / 2) - (m_nLifeOrigin - m_nLife));

		m_move.z -= f;
	}
}