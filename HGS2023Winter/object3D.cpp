//============================================
//
// オブジェクト3Dのメイン処理[object3D.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "object3D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "useful.h"

//=========================================
// コンストラクタ
//=========================================
CObject3D::CObject3D() : CObject(CObject::TYPE_NONE, CObject::PRIORITY_BG)
{
	// コンストラクタの箱
	Box();
}

//=========================================
// オーバーロードコンストラクタ
//=========================================
CObject3D::CObject3D(CObject::TYPE type, PRIORITY priority) : CObject(type, priority)
{
	// コンストラクタの箱
	Box();
}

//=========================================
// デストラクタ
//=========================================
CObject3D::~CObject3D()
{

}

//=========================================
// コンストラクタの箱
//=========================================
void CObject3D::Box(void)
{
	// 全ての値をクリアする
	m_pVtxBuff = nullptr;							// 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_posOld = m_pos;								// 前回の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// サイズ
	ZeroMemory(&m_mtxWorld, sizeof(D3DXMATRIX));	// ワールドマトリックス
	m_nTexIdx = NONE_TEXIDX;						// テクスチャのインデックス
}

//===========================================
// オブジェクト3Dの初期化処理
//===========================================
HRESULT CObject3D::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
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

	VERTEX_3D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, +m_size.y, +m_size.z);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, +m_size.y, +m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, -m_size.y, -m_size.z);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
// オブジェクト3Dの終了処理
//===========================================
void CObject3D::Uninit(void)
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
// オブジェクト3Dの更新処理
//===========================================
void CObject3D::Update(void)
{
	// 頂点の設定処理
	SetVertex();
}

//===========================================
// オブジェクト3Dの描画処理
//===========================================
void CObject3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;			// 計算用マトリックス
	D3DXMATRIX mtxView;						// ビューマトリックス取得用

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		m_pVtxBuff,									// 頂点バッファへのポインタ
		0,
		sizeof(VERTEX_3D));							// 頂点情報構造体のサイズ

	// テクスチャの設定
	pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_nTexIdx));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,											// 描画する最初の頂点インデックス
		2);											// 描画するプリミティブ数
}

//===========================================
// 描画処理(ライティング無し)
//===========================================
void CObject3D::DrawLightOff(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;			// 計算用マトリックス
	D3DXMATRIX mtxView;						// ビューマトリックス取得用

	// ライティングをOFFにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		m_pVtxBuff,									// 頂点バッファへのポインタ
		0,
		sizeof(VERTEX_3D));							// 頂点情報構造体のサイズ

	// テクスチャの設定
	pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_nTexIdx));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,											// 描画する最初の頂点インデックス
		2);											// 描画するプリミティブ数

	// ライティングをONにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//===========================================
// テクスチャの割り当て処理
//===========================================
void CObject3D::BindTexture(int nIdx)
{
	// 番号を割り当てる
	m_nTexIdx = nIdx;
}

//===========================================
// 位置設定処理
//===========================================
void CObject3D::SetPos(const D3DXVECTOR3& pos)
{
	// 位置を設定する
	m_pos = pos;
}

//===========================================
// 位置取得処理
//===========================================
D3DXVECTOR3 CObject3D::GetPos(void) const
{
	// 位置を返す
	return m_pos;
}

//===========================================
// 前回の位置設定処理
//===========================================
void CObject3D::SetPosOld(const D3DXVECTOR3& posOld)
{
	// 前回の位置を設定する
	m_posOld = posOld;
}

//===========================================
// 前回の位置取得処理
//===========================================
D3DXVECTOR3 CObject3D::GetPosOld(void) const
{
	// 前回の位置を返す
	return m_posOld;
}

//===========================================
// 向き設定処理
//===========================================
void CObject3D::SetRot(const D3DXVECTOR3& rot)
{
	// 向きを設定する
	m_rot = rot;
}

//===========================================
// 向き取得処理
//===========================================
D3DXVECTOR3 CObject3D::GetRot(void) const
{
	// 向きを返す
	return m_rot;
}

//===========================================
// サイズ設定処理
//===========================================
void CObject3D::SetSize(const D3DXVECTOR3& size)
{
	// サイズを設定する
	m_size = size;
}

//===========================================
// サイズ取得処理
//===========================================
D3DXVECTOR3 CObject3D::GetSize(void) const
{
	// サイズを返す
	return m_size;
}

//===========================================
// マトリックスの取得処理
//===========================================
D3DXMATRIX CObject3D::GetMatrix(void) const
{
	// ワールドマトリックスのデータを返す
	return m_mtxWorld;
}

//===========================================
// テクスチャのインデックスの取得処理
//===========================================
int CObject3D::GetTexIdx(void) const
{
	// テクスチャのインデックスを返す
	return m_nTexIdx;
}

//===========================================
// 頂点情報の設定
//===========================================
void CObject3D::SetVertex(void)
{
	VERTEX_3D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, +m_size.y, +m_size.z);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, +m_size.y, +m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, -m_size.y, -m_size.z);

	D3DXVECTOR3 nor[4] = {};

	// 法線の計算(正規化)
	useful::NormalizeVector(nor[0], pVtx[2].pos, pVtx[1].pos, pVtx[0].pos);
	useful::NormalizeVector(nor[1], pVtx[0].pos, pVtx[3].pos, pVtx[1].pos);
	useful::NormalizeVector(nor[2], pVtx[3].pos, pVtx[0].pos, pVtx[2].pos);
	useful::NormalizeVector(nor[3], pVtx[1].pos, pVtx[2].pos, pVtx[3].pos);

	// 法線座標の設定
	pVtx[0].nor = nor[0];
	pVtx[1].nor = nor[1];
	pVtx[2].nor = nor[2];
	pVtx[3].nor = nor[3];

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点情報の設定(決め打ち)
//===========================================
void CObject3D::SetVertexHardCoding(const D3DXVECTOR3& VtxMax, const D3DXVECTOR3& VtxMin)
{
	VERTEX_3D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(+VtxMin.x, +VtxMax.y, VtxMax.z);
	pVtx[1].pos = D3DXVECTOR3(+VtxMax.x, +VtxMax.y, VtxMin.z);
	pVtx[2].pos = D3DXVECTOR3(+VtxMin.x, +VtxMin.y, VtxMax.z);
	pVtx[3].pos = D3DXVECTOR3(+VtxMax.x, +VtxMin.y, VtxMin.z);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 起伏のある頂点情報の設定
//===========================================
void CObject3D::SetVertexElev(float fHeight)
{
	VERTEX_3D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, +m_size.z);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, fHeight, +m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, fHeight, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, 0.0f, -m_size.z);

	D3DXVECTOR3 nor[4] = {};

	// 法線の計算(正規化)
	useful::NormalizeVector(nor[0], pVtx[2].pos, pVtx[1].pos, pVtx[0].pos);
	useful::NormalizeVector(nor[1], pVtx[0].pos, pVtx[3].pos, pVtx[1].pos);
	useful::NormalizeVector(nor[2], pVtx[3].pos, pVtx[0].pos, pVtx[2].pos);
	useful::NormalizeVector(nor[3], pVtx[1].pos, pVtx[2].pos, pVtx[3].pos);

	// 法線座標の設定
	pVtx[0].nor = nor[0];
	pVtx[1].nor = nor[1];
	pVtx[2].nor = nor[2];
	pVtx[3].nor = nor[3];

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点カラーの設定処理
//===========================================
void CObject3D::SetVtxColor(const D3DXCOLOR& col)
{
	VERTEX_3D * pVtx;			//頂点情報へのポインタ

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
// 頂点テクスチャの設定処理(縦の一定間隔のテクスチャ配分)
//===========================================
void CObject3D::SetVtxTextureWidth(const D3DXVECTOR2& size)
{
	// ローカル変数宣言
	float fTexWidth = (float)(size.x / size.y);

	VERTEX_3D * pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTexWidth, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fTexWidth, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点テクスチャの設定処理(横の一定間隔のテクスチャ配分)
//===========================================
void CObject3D::SetVtxTextureHeight(const D3DXVECTOR2& size)
{
	// ローカル変数宣言
	float fTexHeight = (float)(size.y / size.x);

	VERTEX_3D * pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, fTexHeight);
	pVtx[3].tex = D3DXVECTOR2(1.0f, fTexHeight);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 起伏のあるポリゴンの当たり判定
//===========================================
float CObject3D::ElevationCollision(const D3DXVECTOR3 pos)
{
	// ローカル変数宣言
	D3DXVECTOR3 nor, vec1, vec2;	// 位置
	float fHeight = 0.0f;			// 対象の高さ

	VERTEX_3D * pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (useful::LineOuterProductXZ(pVtx[0].pos, pVtx[1].pos, pos) >= 0 &&
		useful::LineOuterProductXZ(pVtx[1].pos, pVtx[2].pos, pos) >= 0 &&
		useful::LineOuterProductXZ(pVtx[2].pos, pVtx[0].pos, pos) >= 0)
	{ // 真ん中の境界線より右側に居た場合

		// 法線の計算(正規化)
		useful::NormalizeVector(nor, pVtx[2].pos, pVtx[1].pos, pVtx[0].pos);

		if (nor.y != 0.0f)
		{ // 法線のYが0.0f以外の場合

			// 高さを設定する
			fHeight = -((((pos.x - pVtx[0].pos.x) * nor.x + (pos.z - pVtx[0].pos.z) * nor.z) - pVtx[0].pos.y) / nor.y);
		}
	}
	else if(useful::LineOuterProductXZ(pVtx[1].pos, pVtx[3].pos, pos) >= 0 &&
		useful::LineOuterProductXZ(pVtx[2].pos, pVtx[1].pos, pos) >= 0 &&
		useful::LineOuterProductXZ(pVtx[3].pos, pVtx[2].pos, pos) >= 0)
	{ // 真ん中の境界線より左側に居た場合

		// 法線の計算(正規化)
		useful::NormalizeVector(nor, pVtx[1].pos, pVtx[2].pos, pVtx[3].pos);

		if (nor.y != 0.0f)
		{ // 法線のYが0.0f以外の場合

			// 高さを設定する
			fHeight = -((((pos.x - pVtx[3].pos.x) * nor.x + (pos.z - pVtx[3].pos.z) * nor.z) - pVtx[3].pos.y) / nor.y);
		}
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 高さを返す
	return fHeight;
}

//===========================================
// 生成処理
//===========================================
CObject3D* CObject3D::Create(const TYPE type, const PRIORITY priority)
{
	// ローカルオブジェクトを生成
	CObject3D* pObject3D = nullptr;	// オブジェクト3Dのインスタンスを生成

	if (pObject3D == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pObject3D = new CObject3D(type, priority);
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pObject3D != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		pObject3D->Init();
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// オブジェクト3Dのポインタを返す
	return pObject3D;
}