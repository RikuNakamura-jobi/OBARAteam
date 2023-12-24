//============================================
//
// ドーナツ2Dのメイン処理[donut2D.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "objectDonut2D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "useful.h"

//=========================================
// コンストラクタ
//=========================================
CDonut2D::CDonut2D() : CObject(CObject::TYPE_NONE, CObject::PRIORITY_BG)
{
	// コンストラクタの箱
	Box();
}

//=========================================
// オーバーロードコンストラクタ
//=========================================
CDonut2D::CDonut2D(CObject::TYPE type, PRIORITY priority) : CObject(type, priority)
{
	// コンストラクタの箱
	Box();
}

//=========================================
// デストラクタ
//=========================================
CDonut2D::~CDonut2D()
{

}

//=========================================
// コンストラクタの箱
//=========================================
void CDonut2D::Box(void)
{
	// 全ての値をクリアする
	m_pVtxBuff = nullptr;						// 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_posOld = m_pos;							// 前回の位置
	m_fSize = 0.0f;								// サイズ
	m_fDistance = 0.0f;							// 距離
	m_nDivi = 0;								// 分割数
	m_nNumVtx = 0;								// 総頂点数
	m_nTexIdx = NONE_TEXIDX;					// テクスチャのインデックス
}

//===========================================
// オブジェクト2Dの初期化処理
//===========================================
HRESULT CDonut2D::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * m_nNumVtx,
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
void CDonut2D::Uninit(void)
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
void CDonut2D::Update(void)
{

}

//===========================================
// オブジェクト2Dの描画処理
//===========================================
void CDonut2D::Draw(void)
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
		m_nNumVtx - 2);							// 描画するプリミティブ数
}

//===========================================
// テクスチャの割り当て処理
//===========================================
void CDonut2D::BindTexture(const int nIdx)
{
	// テクスチャを割り当てる
	m_nTexIdx = nIdx;
}

//===========================================
// 位置設定処理
//===========================================
void CDonut2D::SetPos(const D3DXVECTOR3& pos)
{
	// 位置を設定する
	m_pos = pos;
}

//===========================================
// 位置取得処理
//===========================================
D3DXVECTOR3 CDonut2D::GetPos(void) const
{
	// 位置を返す
	return m_pos;
}

//===========================================
// 前回の位置設定処理
//===========================================
void CDonut2D::SetPosOld(const D3DXVECTOR3& posOld)
{
	// 前回の位置を設定する
	m_posOld = posOld;
}

//===========================================
// 前回の位置取得処理
//===========================================
D3DXVECTOR3 CDonut2D::GetPosOld(void) const
{
	// 前回の位置を返す
	return m_posOld;
}

//===========================================
// サイズ設定処理
//===========================================
void CDonut2D::SetSize(const float size)
{
	// サイズを設定する
	m_fSize = size;
}

//===========================================
// サイズ取得処理
//===========================================
float CDonut2D::GetSize(void) const
{
	// サイズを返す
	return m_fSize;
}

//===========================================
// 距離の設定処理
//===========================================
void CDonut2D::SetDistance(const float fDist)
{
	// 距離を設定する
	m_fDistance = fDist;
}

//===========================================
// 距離の取得処理
//===========================================
float CDonut2D::GetDistance(void) const
{
	// 距離を返す
	return m_fDistance;
}

//===========================================
// 分割数の設定処理
//===========================================
void CDonut2D::SetDivi(const int nDivi)
{
	// 分割数を設定する
	m_nDivi = nDivi;
}

//===========================================
// 分割数の取得処理
//===========================================
int CDonut2D::GetDivi(void) const
{
	// 分割数を返す
	return m_nDivi;
}

//===========================================
// 総頂点数の設定処理
//===========================================
void CDonut2D::SetNumVtx(void)
{
	// 総頂点数を設定する
	m_nNumVtx = (m_nDivi + 1) * 2;
}

//===========================================
// 総頂点数の取得処理
//===========================================
int CDonut2D::GetNumVtx(void) const
{
	// 総頂点数を返す
	return m_nNumVtx;
}

//===========================================
// テクスチャのインデックスの取得処理
//===========================================
int CDonut2D::GetTexIdx(void) const
{
	// テクスチャのインデックスを返す
	return m_nTexIdx;
}

//===========================================
// 頂点情報の設定
//===========================================
void CDonut2D::SetVertex(void)
{
	// ローカル変数宣言
	float fAngle;		// 方向
	float fLength;		// 長さ

	VERTEX_2D * pVtx;											// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWidth = 0; nCntWidth < m_nDivi + 1; nCntWidth++)
	{
		for (int nCntHeight = 0; nCntHeight < 2; nCntHeight++)
		{
			// 角度を算出する
			fAngle = D3DX_PI * (nCntWidth * (float)(1.0f / ((m_nDivi) / 2)));

			// 幅を算出する
			fLength = m_fDistance + (m_fSize * nCntHeight);

			// 角度の正規化
			useful::RotNormalize(&fAngle);

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3
			(
				m_pos.x + (sinf(fAngle) * fLength),
				m_pos.y - (cosf(fAngle) * fLength),
				0.0f
			);

			// 頂点のポインタを進める
			pVtx++;
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点の色設定処理
//===========================================
void CDonut2D::SetVtxColor(const D3DXCOLOR& col)
{
	VERTEX_2D* pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_nNumVtx; nCnt++)
	{
		//テクスチャ座標の設定
		pVtx[0].col = col;

		// 頂点データを進める
		pVtx++;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点の色(一定範囲)設定処理
//===========================================
void CDonut2D::SetVtxColor(const int nVtx, const D3DXCOLOR& col1, const D3DXCOLOR& col2)
{
	VERTEX_2D* pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_nNumVtx; nCnt++)
	{
		if (nVtx > nCnt)
		{ // 頂点数が一定超過の場合

			//テクスチャ座標の設定
			pVtx[0].col = col1;
		}
		else
		{ // 上記以外

			//テクスチャ座標の設定
			pVtx[0].col = col2;
		}

		// 頂点データを進める
		pVtx++;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// テクスチャ座標の設定処理
//===========================================
void CDonut2D::SetVtxTexture(void)
{
	// ローカル変数宣言
	VERTEX_2D * pVtx;				// 頂点情報へのポインタ
	D3DXVECTOR2 tex;				// テクスチャ座標

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWidth = 0; nCntWidth < m_nDivi + 1; nCntWidth++)
	{
		for (int nCntHeight = 0; nCntHeight < 2; nCntHeight++)
		{
			tex.x = (float)(nCntHeight);
			tex.y = (float)((1.0f / (float)(m_nDivi)) * nCntWidth);

			//頂点座標の設定
			pVtx[0].tex = tex;

			// 頂点のポインタを進める
			pVtx++;
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 全ての頂点情報の設定
//===========================================
void CDonut2D::SetVertexAll(void)
{
	// 頂点座標の設定処理
	SetVertex();

	VERTEX_2D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_nNumVtx; nCnt++)
	{
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		
		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 頂点データを進める
		pVtx++;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// テクスチャ座標の設定処理
	SetVtxTexture();
}

//===========================================
// 生成処理
//===========================================
CDonut2D* CDonut2D::Create(const D3DXVECTOR3& pos, const float fSize, const float fDist, const int nDivi)
{
	// ローカルオブジェクトを生成
	CDonut2D* pDonut = nullptr;	// オブジェクト2Dのインスタンスを生成

	if (pDonut == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pDonut = new CDonut2D;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pDonut != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 情報の設定処理
		pDonut->m_pos = pos;
		pDonut->m_fSize = fSize;
		pDonut->m_fDistance = fDist;
		pDonut->m_nDivi = nDivi;

		// 初期化処理
		if (FAILED(pDonut->Init()))
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

	// オブジェクト2Dのポインタを返す
	return pDonut;
}