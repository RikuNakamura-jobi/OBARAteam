//=======================================
//
// 2Dエフェクトのメイン処理[effect2D.cpp]
// Author 小原立暉
//
//=======================================
#include "effect2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//=======================================
// マクロ定義
//=======================================
#define NORMAL_EFFECT_TEXTURE		"data\\TEXTURE\\Platform\\effect000.jpg"	// 通常のエフェクトのテクスチャ
#define STAR_EFFECT_TEXTURE			"data\\TEXTURE\\Platform\\SpeedEffect.tga"	// 星のエフェクトのテクスチャ
#define BOOST_EFFECT_TEXTURE		"data\\TEXTURE\\Platform\\smoke.tga"		// 煙のエフェクトのテクスチャ
#define GLASS_ROT_MOVE				(0.04f)								// ガラスの向きの移動量

//=========================
// コンストラクタ
//=========================
CEffect2D::CEffect2D() : CObject2D(CObject2D::TYPE_EFFECT, CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
	m_type = TYPE_NONE;								// 種類
	m_nLife = 0;									// 寿命
	m_fSub = 0.0f;									// 透明になる間隔
	m_fContra = 0.0f;								// 半径の縮む間隔
	m_bAdd = false;									// 加算合成状況
}

//=========================
// デストラクタ
//=========================
CEffect2D::~CEffect2D()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CEffect2D::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
	m_type = TYPE_NONE;								// 種類
	m_nLife = 0;									// 寿命
	m_fSub = 0.0f;									// 透明になる間隔
	m_fContra = 0.0f;								// 半径の縮む間隔
	m_bAdd = false;									// 加算合成状況

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CEffect2D::Uninit(void)
{
	// 終了
	CObject2D::Uninit();
}

//=========================
// 更新処理
//=========================
void CEffect2D::Update(void)
{
	// ローカル変数を宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	D3DXVECTOR3 size = GetSize();	// サイズを取得する

	// 寿命を減らしていく
	m_nLife--;

	// 移動する
	pos += m_move;

	// 透明度を減算させる
	m_col.a -= m_fSub;

	// サイズを縮める
	size.x -= m_fContra;
	size.y -= m_fContra;

	if (m_col.a <= 0.0f)
	{ // 透明度が0.0f以下の場合

		// 透明度を補正する
		m_col.a = 0.0f;
	}

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

	// 位置を更新する
	SetPos(pos);

	// サイズを更新する
	SetSize(size);

	// 種類ごとの処理
	TypeProcess();

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
void CEffect2D::Draw(void)
{
	if (m_bAdd == true)
	{ // 通常種類の場合

		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		//αブレンディングを加算処理に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		// 描画処理
		CObject2D::Draw();

		//αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	else
	{ // 上記以外

		// 描画処理
		CObject2D::Draw();
	}
}

//=========================
// 情報の設定処理
//=========================
void CEffect2D::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int nLife, const float fRadius, const TYPE type)
{
	// スクロールの設定処理
	SetPos(pos);			// 位置設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));			// 向き設定
	SetSize(D3DXVECTOR3(fRadius, fRadius, 0.0f));	// サイズ設定
	SetLength();			// 長さ設定
	SetAngle();				// 方向設定

	// 全ての値を初期化する
	m_type = type;			// 種類
	m_nLife = nLife;		// 寿命
	m_move = move;			// 移動量

	switch (m_type)
	{
	case TYPE_NONE:			// 通常

		// 色を設定する
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 加算合成状況を true にする
		m_bAdd = true;

		break;

	case TYPE_SHINING:		// 輝き

		// 色を設定する
		m_col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

		// 加算合成状況を false にする
		m_bAdd = false;

		break;

	case TYPE_INFIRE:		// 内側の炎

		// 色を設定する
		m_col = D3DXCOLOR(1.0f, 0.7f, 0.2f, 1.0f);

		// 加算合成状況を true にする
		m_bAdd = true;

		break;

	case TYPE_OUTFIRE:		// 外側の炎

		// 色を設定する
		m_col = D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f);

		// 加算合成状況を false にする
		m_bAdd = false;

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	m_fSub = m_col.a * (1.0f / m_nLife);			// 透明になる間隔
	m_fContra = fRadius * (1.0f / m_nLife);			// 半径の縮む間隔

	// 頂点情報の初期化
	SetVertex();

	// 頂点カラーの設定処理
	SetVtxColor(m_col);

	if (m_type == TYPE_SHINING)
	{ // 種類が星関係の場合

		// テクスチャの読み込み処理
		BindTexture(CManager::Get()->GetTexture()->Regist(STAR_EFFECT_TEXTURE));
	}
	else if (m_type == TYPE_INFIRE ||
		m_type == TYPE_OUTFIRE)
	{ // 種類が炎・ブーストの場合

		// テクスチャの読み込み処理
		BindTexture(CManager::Get()->GetTexture()->Regist(BOOST_EFFECT_TEXTURE));
	}
	else
	{ // 上記以外

		// テクスチャの読み込み処理
		BindTexture(CManager::Get()->GetTexture()->Regist(NORMAL_EFFECT_TEXTURE));
	}
}

//=========================
// 種類による処理
//=========================
void CEffect2D::TypeProcess(void)
{
	switch (m_type)
	{
	case TYPE_NONE:			// 通常

		break;

	case TYPE_SHINING:		// 輝き

		break;

	case TYPE_INFIRE:		// 内側の炎

		break;

	case TYPE_OUTFIRE:		// 外側の炎

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//=========================
// 生成処理
//=========================
CEffect2D* CEffect2D::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int nLife, const float fRadius, const TYPE type)
{
	// ローカルオブジェクトを生成
	CEffect2D* pEffect = nullptr;	// プレイヤーのインスタンスを生成

	if (pEffect == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pEffect = new CEffect2D;
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
		pEffect->SetData(pos, move, nLife, fRadius, type);
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