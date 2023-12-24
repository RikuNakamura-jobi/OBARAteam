//============================================
//
// オブジェクト2Dのメイン処理[object2D.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "object2D.h"
#include "renderer.h"
#include "manager.h"
#include "objectAnim.h"
#include "objectScroll.h"
#include "texture.h"

//=========================================
// コンストラクタ
//=========================================
CObject2D::CObject2D() : CObject(CObject::TYPE_NONE, CObject::PRIORITY_BG)
{
	// コンストラクタの箱
	Box();
}

//=========================================
// オーバーロードコンストラクタ
//=========================================
CObject2D::CObject2D(CObject::TYPE type, PRIORITY priority) : CObject(type, priority)
{
	// コンストラクタの箱
	Box();
}

//=========================================
// デストラクタ
//=========================================
CObject2D::~CObject2D()
{

}

//=========================================
// コンストラクタの箱
//=========================================
void CObject2D::Box(void)
{
	// 全ての値をクリアする
	m_pVtxBuff = nullptr;		// 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_posOld = m_pos;							// 前回の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズ
	m_fAngle = 0.0f;							// 方向
	m_fLength = 0.0f;							// 長さ
	m_nTexIdx = NONE_TEXIDX;					// テクスチャのインデックス
}

//===========================================
// オブジェクト2Dの初期化処理
//===========================================
HRESULT CObject2D::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{ // バッファの生成に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_posOld = m_pos;							// 前回の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズ
	m_fAngle = 0.0f;							// 方向
	m_fLength = 0.0f;							// 長さ

	// 頂点情報の設定処理
	SetVertexAll();

	if (m_pVtxBuff == nullptr)
	{ // 頂点バッファのポインタがNULLの場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 値を返す
	return S_OK;
}

//===========================================
// オブジェクト2Dの終了処理
//===========================================
void CObject2D::Uninit(void)
{
	if (m_pVtxBuff != nullptr)
	{ // 頂点バッファの破棄

		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// 破棄処理
	Release();
}

//===========================================
// オブジェクト2Dの更新処理
//===========================================
void CObject2D::Update(void)
{
	// 頂点設定処理
	SetVertexRot();

	// テクスチャの設定処理
	SetVtxTexture();
}

//===========================================
// オブジェクト2Dの描画処理
//===========================================
void CObject2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		m_pVtxBuff,								// 頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D));						// 頂点情報構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_nTexIdx));

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,										// 描画する最初の頂点インデックス
		2);										// 描画するプリミティブ数
}

//===========================================
// テクスチャの割り当て処理
//===========================================
void CObject2D::BindTexture(const int nIdx)
{
	// テクスチャを割り当てる
	m_nTexIdx = nIdx;
}

//===========================================
// 位置設定処理
//===========================================
void CObject2D::SetPos(const D3DXVECTOR3& pos)
{
	// 位置を設定する
	m_pos = pos;
}

//===========================================
// 位置取得処理
//===========================================
D3DXVECTOR3 CObject2D::GetPos(void) const
{
	// 位置を返す
	return m_pos;
}

//===========================================
// 前回の位置設定処理
//===========================================
void CObject2D::SetPosOld(const D3DXVECTOR3& posOld)
{
	// 前回の位置を設定する
	m_posOld = posOld;
}

//===========================================
// 前回の位置取得処理
//===========================================
D3DXVECTOR3 CObject2D::GetPosOld(void) const
{
	// 前回の位置を返す
	return m_posOld;
}

//===========================================
// 向き設定処理
//===========================================
void CObject2D::SetRot(const D3DXVECTOR3& rot)
{
	// 向きを設定する
	m_rot = rot;
}

//===========================================
// 向き取得処理
//===========================================
D3DXVECTOR3 CObject2D::GetRot(void) const
{
	// 向きを返す
	return m_rot;
}

//===========================================
// サイズ設定処理
//===========================================
void CObject2D::SetSize(const D3DXVECTOR3& size)
{
	// サイズを設定する
	m_size = size;
}

//===========================================
// サイズ取得処理
//===========================================
D3DXVECTOR3 CObject2D::GetSize(void) const
{
	// サイズを返す
	return m_size;
}

//===========================================
// 長さ設定処理
//===========================================
void CObject2D::SetLength(void)
{
	//対角線の長さを算出する
	m_fLength = sqrtf((m_size.x * 2) * (m_size.x * 2) + (m_size.y * 2) * (m_size.y * 2)) * 0.5f;
}

//===========================================
// 長さ設定処理
//===========================================
float CObject2D::GetLength(void) const
{
	// 長さを返す
	return m_fLength;
}

//===========================================
// 方向設定処理
//===========================================
void CObject2D::SetAngle(void)
{
	//対角線の角度を算出する
	m_fAngle = atan2f((m_size.x * 2), (m_size.y * 2));
}

//===========================================
// 方向設定処理
//===========================================
float CObject2D::GetAngle(void) const
{
	// 方向を設定する
	return m_fAngle;
}

//===========================================
// テクスチャのインデックスの取得処理
//===========================================
int CObject2D::GetTexIdx(void) const
{
	// テクスチャのインデックスを返す
	return m_nTexIdx;
}

//===========================================
// 頂点情報の設定
//===========================================
void CObject2D::SetVertex(void)
{
	VERTEX_2D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点情報の設定(傾き有りヴァージョン)
//===========================================
void CObject2D::SetVertexRot(void)
{
	VERTEX_2D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - D3DX_PI + m_fAngle) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - D3DX_PI + m_fAngle) * m_fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点情報の設定(横ゲージ表示バージョン)
//===========================================
void CObject2D::SetVtxWidthGage(void)
{
	VERTEX_2D* pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点情報の設定(下中心ゲージ表示バージョン)
//===========================================
void CObject2D::SetVtxUnderHeightGage(void)
{
	VERTEX_2D* pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点情報の設定(下中心傾き有りバージョン)
//===========================================
void CObject2D::SetVtxUnderRot(void)
{
	VERTEX_2D* pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + (-D3DX_PI * 0.5f)) * m_size.x + sinf(m_rot.z + D3DX_PI) * m_size.y * 2.0f;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + (-D3DX_PI * 0.5f)) * m_size.x + cosf(m_rot.z + D3DX_PI) * m_size.y * 2.0f;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI * 0.5f)) * m_size.x + sinf(m_rot.z + D3DX_PI) * m_size.y * 2.0f;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI * 0.5f)) * m_size.x + cosf(m_rot.z + D3DX_PI) * m_size.y * 2.0f;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + (-D3DX_PI * 0.5f)) * m_size.x;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + (-D3DX_PI * 0.5f)) * m_size.x;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI * 0.5f)) * m_size.x;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI * 0.5f)) * m_size.x;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点情報の設定(ベタ打ち)
//===========================================
void CObject2D::SetVtxHardCoding(const D3DXVECTOR3& LeftUp, const D3DXVECTOR3& RightUp, const D3DXVECTOR3& LeftDown, const D3DXVECTOR3& RightDown)
{
	VERTEX_2D* pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + LeftUp.x, m_pos.y + LeftUp.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + RightUp.x, m_pos.y + RightUp.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + LeftDown.x, m_pos.y + LeftDown.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + RightDown.x, m_pos.y + RightDown.y, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点の色設定処理
//===========================================
void CObject2D::SetVtxColor(const D3DXCOLOR& col)
{
	VERTEX_2D* pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// テクスチャの設定
//===========================================
void CObject2D::SetVtxTexture(void)
{
	VERTEX_2D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// テクスチャの設定(アニメーションバージョン)
//===========================================
void CObject2D::SetVtxTextureAnim(float fTexPattern,int nPattern)
{
	VERTEX_2D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(nPattern * fTexPattern, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(nPattern * fTexPattern + fTexPattern, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nPattern * fTexPattern, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(nPattern * fTexPattern + fTexPattern, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// テクスチャの設定(スクロールバージョン)
//===========================================
void CObject2D::SetVtxTextureScroll(D3DXVECTOR2& tex)
{
	VERTEX_2D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(tex.x, tex.y);
	pVtx[1].tex = D3DXVECTOR2(tex.x + 1.0f, tex.y);
	pVtx[2].tex = D3DXVECTOR2(tex.x, tex.y + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(tex.x + 1.0f, tex.y + 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// テクスチャの設定(横のテクスチャ任意設定)
//===========================================
void CObject2D::SetVtxTextureWidth(const float tex)
{
	VERTEX_2D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(tex, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(tex, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// テクスチャの設定(下中心の縦のテクスチャ任意設定)
//===========================================
void CObject2D::SetVtxTextureUnderHeight(const float tex)
{
	VERTEX_2D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, tex);
	pVtx[1].tex = D3DXVECTOR2(1.0f, tex);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 全ての頂点情報の設定
//===========================================
void CObject2D::SetVertexAll(void)
{
	VERTEX_2D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - D3DX_PI + m_fAngle) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - D3DX_PI + m_fAngle) * m_fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 生成処理
//===========================================
CObject2D* CObject2D::Create(const TYPE type2D, const CObject::TYPE type, const PRIORITY priority)
{
	// ローカルオブジェクトを生成
	CObject2D* pObject2D = nullptr;	// オブジェクト2Dのインスタンスを生成

	if (pObject2D == nullptr)
	{ // オブジェクトが NULL の場合

		switch (type2D)
		{
		case TYPE_NONE:		// 通常

			// オブジェクトを生成
			pObject2D = new CObject2D(type, priority);

			break;

		case TYPE_ANIM:		// アニメーション

			// オブジェクトを生成
			pObject2D = new CAnim(type, priority);

			break;

		case TYPE_SCROLL:	// スクロール

			// オブジェクトを生成
			pObject2D = new CScroll(type, priority);

			break;

		default:

			// 停止
			assert(false);

			break;
		}
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pObject2D != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		pObject2D->Init();
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pObject2D;
}