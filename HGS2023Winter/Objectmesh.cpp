//======================================================================================================================
//
//	メッシュ処理 [mesh.cpp]
//	Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Objectmesh.h"
#include "mesh_manager.h"
#include "useful.h"
#include "texture.h"

#include "mesh_field.h"
#include "mesh_wall.h"
#include "mesh_cylinder.h"
#include "mesh_dome.h"

//----------------------------------------------------------------------------------------------------------------------
// マクロ定義
//----------------------------------------------------------------------------------------------------------------------
#define MESH_TXT			"data/TXT/Mesh.txt"			// メッシュのテキスト

//================================
// コンストラクタ
//================================
CMesh::CMesh() : CObject(CObject::TYPE_FIELD, CObject::PRIORITY_BG)
{
	// コンストラクタの箱
	Box();
}

//================================
// オーバーロードコンストラクタ
//================================
CMesh::CMesh(CObject::TYPE type, CObject::PRIORITY priority) : CObject(type, priority)
{
	// コンストラクタの箱
	Box();
}

//================================
// デストラクタ
//================================
CMesh::~CMesh()
{

}

//================================
// コンストラクタの箱
//================================
void CMesh::Box(void)
{
	// 全ての値をクリアする
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// サイズ
	m_sizeDivi = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 1つ当たりの面のサイズ
	ZeroMemory(&m_mtxWorld, sizeof(D3DXMATRIX));	// ワールドマトリックス
	m_pVtxBuff = nullptr;							// 頂点バッファのポインタ
	m_pIdxBuff = nullptr;							// インデックスバッファへのポインタ
	m_type = TYPE_FIELD;							// 種類
	m_vtx.x = 0;									// 横幅の頂点数
	m_vtx.y = 0;									// 縦幅の頂点数
	m_vtx.z = 0;									// 奥幅の頂点数
	m_divi.x = 0;									// 横幅の分割数
	m_divi.y = 0;									// 縦幅の分割数
	m_divi.z = 0;									// 奥幅の分割数
	m_nNumVtx = 0;									// 総頂点数
	m_nNumIdx = 0;									// 総インデックス数
	m_nTexIdx = NONE_TEXIDX;						// テクスチャのインデックス
	m_bLightOff = false;							// ライティング状況

	// 全ての値をクリアする
	m_pPrev = nullptr;		// 前のアウトボールへのポインタ
	m_pNext = nullptr;		// 次のアウトボールへのポインタ

	if (CMeshManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// マネージャーへの登録処理
		CMeshManager::Get()->Regist(this);
	}
}

//============================
// 前のポインタの設定処理
//============================
void CMesh::SetPrev(CMesh* pPrev)
{
	// 前のポインタを設定する
	m_pPrev = pPrev;
}

//============================
// 後のポインタの設定処理
//============================
void CMesh::SetNext(CMesh* pNext)
{
	// 次のポインタを設定する
	m_pNext = pNext;
}

//============================
// 前のポインタの設定処理
//============================
CMesh* CMesh::GetPrev(void) const
{
	// 前のポインタを返す
	return m_pPrev;
}

//============================
// 次のポインタの設定処理
//============================
CMesh* CMesh::GetNext(void) const
{
	// 次のポインタを返す
	return m_pNext;
}

//================================
// 初期化処理
//================================
HRESULT CMesh::Init(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * m_nNumVtx,	// 必要頂点数
		D3DUSAGE_WRITEONLY,				// 使用方法
		FVF_VERTEX_3D,					// 頂点フォーマット
		D3DPOOL_MANAGED,				// メモリの指定
		&m_pVtxBuff,					// 頂点バッファへのポインタ
		NULL
	)))
	{ // 頂点の生成に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// インデックスバッファの生成
	if (FAILED(pDevice->CreateIndexBuffer
	( // 引数
		sizeof(WORD) * m_nNumIdx,		// 必要インデックス数
		D3DUSAGE_WRITEONLY,				// 使用方法
		D3DFMT_INDEX16,					// インデックスバッファのフォーマット
		D3DPOOL_MANAGED,				// メモリの指定
		&m_pIdxBuff,					// インデックスバッファへのポインタ
		NULL
	)))
	{ // インデックスの生成に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	switch (m_type)
	{
	case TYPE_FIELD:	// フィールド

		// フィールドの頂点設定処理
		SetVertexField();

		// インデックスの設定処理
		SetIndex(m_vtx.z, m_vtx.x);

		break;

	case TYPE_WALL:		// 壁

		// 壁の頂点設定処理
		SetVertexWall();

		// インデックスの設定処理
		SetIndex(m_vtx.y, m_vtx.x);

		break;

	case TYPE_CYLINDER:	// 筒

		// 筒の頂点設定処理
		SetVertexCylinder();

		// インデックスの設定処理
		SetIndex(m_vtx.y, m_vtx.x);

		break;

	case TYPE_DOME:		// ドーム

		// ドームの頂点設定処理
		SetVertexDome();

		// インデックスの設定処理
		SetIndex(m_vtx.y, m_vtx.x);

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 成功を返す
	return S_OK;
}

//================================
// 終了処理
//================================
void CMesh::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{ // 変数 (g_pVtxBuffMeshField) がNULLではない場合

		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// インデックスバッファの破棄
	if (m_pIdxBuff != nullptr)
	{ // 変数 (g_pIdxBuffMeshField) がNULLではない場合

		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	// 破棄処理
	Release();

	if (CMeshManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// リスト構造の引き抜き処理
		CMeshManager::Get()->Pull(this);
	}

	// リスト構造関係のポインタを NULL にする
	m_pPrev = nullptr;
	m_pNext = nullptr;
}

//================================
// 更新処理
//================================
void CMesh::Update(void)
{

}

//================================
// 描画処理
//================================
void CMesh::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	if (m_bLightOff == true)
	{ // ライティング状況が true の場合

		// ライティングをOFFにする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// 描画処理の中身
		DrawProcess();

		// ライティングをONにする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
	else
	{ // 上記以外

		// 描画処理の中身
		DrawProcess();
	}
}

//================================
// 描画処理の中身
//================================
void CMesh::DrawProcess(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

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
		m_pVtxBuff,					// 頂点バッファへのポインタ
		0,
		sizeof(VERTEX_3D));			// 頂点情報構造体のサイズ

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_nTexIdx));

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,													// 描画する最初の頂点インデックス
		0,
		m_nNumVtx,											// 用意した頂点の数
		0,
		m_nNumIdx - 2);										// 描画するプリミティブ数
}

//================================
// フィールドの情報設定処理
//================================
void CMesh::SetDataField(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const SGrid& Divi)
{
	// 全ての値を初期化する
	m_pos = pos;						// 位置
	m_rot = rot;						// 向き
	m_size = size;						// 大きさ
	m_divi.x = Divi.x;					// 横幅の分割数
	m_divi.y = 0;						// 縦幅の分割数
	m_divi.z = Divi.z;					// 奥幅の分割数
	m_vtx.x = m_divi.x + 1;				// 横幅の頂点数
	m_vtx.y = 0;						// 縦幅の頂点数
	m_vtx.z = m_divi.z + 1;				// 奥幅の頂点数
	m_type = TYPE_FIELD;				// 種類
	m_nNumVtx = m_vtx.x * m_vtx.z;		// 総頂点数
	m_nNumIdx = (2 * (m_vtx.x * m_divi.z)) + ((m_vtx.z - 2) * 2);		// 総インデックス数
	m_nTexIdx = NONE_TEXIDX;			// テクスチャのインデックス
	m_sizeDivi = D3DXVECTOR3			// 1つ当たりの面のサイズ
	(
		m_size.x / (float)(m_divi.x),
		0.0f,
		m_size.z / (float)(m_divi.z)
	);
}

//================================
// 壁の情報設定処理
//================================
void CMesh::SetDataWall(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const SGrid& Divi)
{
	// 引数を設定
	m_pos = pos;						// 位置
	m_rot = rot;						// 向き
	m_size = size;						// 大きさ
	m_divi.x = Divi.x;					// 横幅の分割数
	m_divi.y = Divi.y;					// 縦幅の分割数
	m_divi.z = 0;						// 奥幅の分割数
	m_vtx.x = m_divi.x + 1;				// 横幅の頂点数
	m_vtx.y = m_divi.y + 1;				// 横幅の頂点数
	m_vtx.z = 0;						// 横幅の頂点数
	m_type = TYPE_WALL;					// 種類
	m_nNumVtx = m_vtx.x * m_vtx.y;		// 総頂点数
	m_nNumIdx = (2 * (m_vtx.x * m_divi.y)) + ((m_vtx.y - 2) * 2);		// 総インデックス数
	m_nTexIdx = NONE_TEXIDX;			// テクスチャのインデックス
	m_sizeDivi = D3DXVECTOR3			// 1つ当たりの面のサイズ
	(
		m_size.x / (float)(m_divi.x),
		m_size.y / (float)(m_divi.y),
		0.0f
	);
}

//================================
// 筒の情報設定処理
//================================
void CMesh::SetDataCylinder(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fCircumSize, const float fHeightSize, const int nCircum, const int nHeight)
{
	// 情報の設定
	m_pos = pos;					// 位置
	m_rot = rot;					// 向き
	m_size.x = fCircumSize;			// 円周のサイズ
	m_size.y = fHeightSize;			// 高さのサイズ
	m_size.z = fCircumSize;			// 高さのサイズ
	m_divi.x = nCircum;				// 円周の分割数
	m_divi.y = nHeight;				// 高さの分割数
	m_divi.z = nCircum;				// 高さの分割数
	m_vtx.x = m_divi.x + 1;			// 円周の頂点数
	m_vtx.y = m_divi.y + 1;			// 高さの頂点数
	m_vtx.z = m_divi.z + 1;			// 円周の頂点数
	m_type = TYPE_CYLINDER;			// 種類
	m_nNumVtx = m_vtx.x * m_vtx.y;	// 総頂点数
	m_nNumIdx = (2 * (m_vtx.x * m_divi.y)) + ((m_vtx.y - 2) * 2);			// 総インデックス数
	m_nTexIdx = NONE_TEXIDX;		// テクスチャのインデックス
	m_sizeDivi = D3DXVECTOR3		// 1つ当たりの面のサイズ
	(
		m_size.x / (float)(m_divi.x),
		m_size.y / (float)(m_divi.y),
		m_size.z / (float)(m_divi.z)
	);
}

//================================
// ドームの情報設定処理
//================================
void CMesh::SetDataDome(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fCircumSize, const float fHeightSize, const int nCircum, const int nHeight)
{
	// 情報の設定
	m_pos = pos;			// 位置
	m_rot = rot;			// 向き
	m_size.x = fCircumSize;			// 円周のサイズ
	m_size.y = fHeightSize;			// 高さのサイズ
	m_size.z = fCircumSize;			// 高さのサイズ
	m_divi.x = nCircum;				// 円周の分割数
	m_divi.y = nHeight;				// 高さの分割数
	m_divi.z = nCircum;				// 高さの分割数
	m_vtx.x = m_divi.x + 1;			// 円周の頂点数
	m_vtx.y = m_divi.y + 1;			// 高さの頂点数
	m_vtx.z = m_divi.z + 1;			// 円周の頂点数
	m_type = TYPE_DOME;				// 種類
	m_nNumVtx = m_vtx.x * m_vtx.y;	// 総頂点数
	m_nNumIdx = (2 * (m_vtx.x * m_divi.y)) + ((m_vtx.y - 2) * 2);				// 総インデックス数
	m_nTexIdx = NONE_TEXIDX;		// テクスチャのインデックス
	m_sizeDivi = D3DXVECTOR3		// 1つ当たりの面のサイズ
	(
		m_size.x / (float)(m_divi.x),
		m_size.y / (float)(m_divi.y),
		m_size.z / (float)(m_divi.z)
	);
}

//================================
// テクスチャの割り当て処理
//================================
void CMesh::BindTexture(int nIdx)
{
	// テクスチャを割り当てる
	m_nTexIdx = nIdx;
}

//================================
// 位置の設定処理
//================================
void CMesh::SetPos(const D3DXVECTOR3& pos)
{
	// 位置を設定する
	m_pos = pos;
}

//================================
// 位置の取得処理
//================================
D3DXVECTOR3 CMesh::GetPos(void) const
{
	// 位置を返す
	return m_pos;
}

//================================
// 向きの設定処理
//================================
void CMesh::SetRot(const D3DXVECTOR3& rot)
{
	// 向きを設定する
	m_rot = rot;
}

//================================
// 向きの取得処理
//================================
D3DXVECTOR3 CMesh::GetRot(void) const
{
	// 向きを返す
	return m_rot;
}

//================================
// 拡大率の設定処理
//================================
void CMesh::SetSize(const D3DXVECTOR3& size)
{
	// サイズを設定する
	m_size = size;
}

//================================
// 拡大率の取得処理
//================================
D3DXVECTOR3 CMesh::GetSize(void) const
{
	// サイズを返す
	return m_size;
}

//================================
// ライティングの設定処理
//================================
void CMesh::SetLighting(const bool bLighting)
{
	// ライティング状況を設定する
	m_bLightOff = bLighting;
}

//================================
// 頂点の設定処理
//================================
void CMesh::SetVertexField(void)
{
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDep = 0; nCntDep < m_vtx.z; nCntDep++)
	{
		for (int nCntWid = 0; nCntWid < m_vtx.x; nCntWid++)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3
			( // 引数
				(nCntWid * m_sizeDivi.x) - (m_size.x / 2.0f),	// X座標
				0.0f,											// Y座標
				(m_size.z / 2.0f) - (nCntDep * m_sizeDivi.z)	// Z座標
			);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(nCntWid * (1.0f / (float)(m_divi.x)), nCntDep * (1.0f / (float)(m_divi.z)));

			pVtx++;				// 頂点データを進める
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================
// 壁の設定処理
//================================
void CMesh::SetVertexWall(void)
{
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDep = 0; nCntDep < m_vtx.y; nCntDep++)
	{
		for (int nCntWid = 0; nCntWid < m_vtx.x; nCntWid++)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3
			( // 引数
				(nCntWid * m_sizeDivi.x) - (m_size.x / 2.0f),	// X座標
				m_size.y - (nCntDep * m_sizeDivi.y),			// Y座標
				0.0f											// Z座標
			);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(nCntWid * (1.0f / 5.0f), nCntDep * (1.0f / 5.0f));

			pVtx++;				// 頂点データを進める
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================
// 筒の設定処理
//================================
void CMesh::SetVertexCylinder(void)
{
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ
	float fAngle;			// 角度算出用変数

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHeight = 0; nCntHeight < m_vtx.y; nCntHeight++)
	{
		for (int nCntCircum = 0; nCntCircum < m_vtx.x; nCntCircum++)
		{
			// 角度を算出する
			fAngle = D3DX_PI * (nCntCircum * (float)(1.0f / (m_divi.x / 2)));

			// 角度の正規化
			useful::RotNormalize(&fAngle);

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3
			(
				(-sinf(fAngle) * m_size.x),
				m_size.y - (m_sizeDivi.y * nCntHeight),
				(-cosf(fAngle) * m_size.z)
			);

			//法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(((float)(1.0f / m_divi.x) * nCntCircum), ((float)(1.0f / m_divi.y) * nCntHeight));

			pVtx++;			// 頂点データを進める
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================
// ドームの設定処理
//================================
void CMesh::SetVertexDome(void)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;		// 頂点情報へのポインタ
	float fAngle;			// 方向
	float fHeiAngle;		// 高さの方向

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHeight = 0; nCntHeight < m_vtx.y; nCntHeight++)
	{
		for (int nCntCircum = 0; nCntCircum < m_vtx.x; nCntCircum++)
		{
			// 角度を算出する
			fAngle = D3DX_PI * (nCntCircum * (float)(1.0f / (m_divi.x / 2)));
			fHeiAngle = D3DX_PI - ((D3DX_PI * 0.5f) / m_divi.y) * nCntHeight;

			// 角度の正規化
			useful::RotNormalize(&fAngle);
			useful::RotNormalize(&fHeiAngle);

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3
			(
				(sinf(fAngle) * ((float)(m_size.x / m_divi.y) * nCntHeight)),
				cosf(fHeiAngle) * -m_size.y,
				(cosf(fAngle) * ((float)(m_size.z / m_divi.y) * nCntHeight))
			);

			//法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((float)((1.0f / m_divi.x) * nCntCircum), (float)((1.0f / m_divi.y) * nCntHeight));

			pVtx++;			// 頂点データを進める
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================
// インデックスの設定処理
//================================
void CMesh::SetIndex(const int nNumVtx1, const int nNumVtx2)
{
	// ローカル変数宣言
	WORD      *pIdx;							// インデックス情報へのポインタ
	int nIdx = 0;								// インデックス数

	//------------------------------------------------------------------------------------------------------------------
	//	インデックス情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// インデックスバッファをロックし、頂点番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntDep = 1; nCntDep < nNumVtx1; nCntDep++)
	{
		if (nCntDep != 1)
		{ // 最初以外

			// インデックス数を設定する
			pIdx[0] = (WORD)(nNumVtx2 * nCntDep);

			// インデックスデータを加算する
			pIdx++;
			nIdx++;
		}

		for (int nCntWid = 0; nCntWid < nNumVtx2; nCntWid++)
		{ // 1層ごとに設定する

			// インデックス数を設定する
			pIdx[0] = (WORD)((nNumVtx2 * nCntDep) + nCntWid);
			pIdx[1] = (WORD)((nNumVtx2 * nCntDep) - (nNumVtx2 - nCntWid));

			// インデックスデータを2つ進める
			pIdx += 2;
			nIdx += 2;

			if (nCntWid == (nNumVtx2 - 1) && nCntDep != (nNumVtx1 - 1))
			{ // 折り返しに入った場合

				// インデックス数を設定する
				pIdx[0] = (WORD)((nNumVtx2 * nCntDep) - (nNumVtx2 - nCntWid));

				// インデックスデータを加算する
				pIdx++;
				nIdx++;
			}
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();
}

//================================
// 生成処理
//================================
CMesh* CMesh::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const SGrid& Divi, const TYPE type, char* texturename, const bool bLighting)
{
	// ローカルオブジェクトを生成
	CMesh* pMesh = nullptr;	// オブジェクト3Dのインスタンスを生成

	if (pMesh == nullptr)
	{ // オブジェクトが NULL の場合

		switch (type)
		{
		case TYPE_FIELD:			// フィールド

			// オブジェクトを生成
			pMesh = new CMeshField;

			break;

		case TYPE::TYPE_WALL:		// ウォール

			// オブジェクトを生成
			pMesh = new CMeshWall;

			break;

		case TYPE::TYPE_CYLINDER:	// シリンダー

			// オブジェクトを生成
			pMesh = new CMeshCylinder;

			break;

		case TYPE::TYPE_DOME:		// ドーム

			// オブジェクトを生成
			pMesh = new CMeshDome;

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

	if (pMesh != nullptr)
	{ // オブジェクトが NULL じゃない場合

		switch (type)
		{
		case TYPE_FIELD:	// フィールド

			// フィールドの設定処理
			pMesh->SetDataField(pos, rot, size, Divi);

			break;

		case TYPE_WALL:		// 壁

			// 壁の設定処理
			pMesh->SetDataWall(pos, rot, size, Divi);

			break;

		case TYPE_CYLINDER:	// 筒

			// 筒の設定処理
			pMesh->SetDataCylinder(pos, rot, size.x, size.y, Divi.x, Divi.y);

			break;

		case TYPE_DOME:

			// ドームの設定処理
			pMesh->SetDataDome(pos, rot, size.x, size.y, Divi.x, Divi.y);

			break;

		default:

			// 停止
			assert(false);

			break;
		}

		// 初期化処理
		if (FAILED(pMesh->Init()))
		{ // 初期化処理に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// テクスチャの割り当て処理
		pMesh->BindTexture(CManager::Get()->GetTexture()->Regist(texturename));

		// ライティング状況を設定する
		pMesh->m_bLightOff = bLighting;
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// オブジェクト3Dのポインタを返す
	return pMesh;
}

//================================
// テキスト読み込み処理
//================================
void CMesh::TxtSet(void)
{
	// 変数を宣言
	int nEnd;			// テキスト読み込み終了の確認用
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// サイズ
	SGrid divi = { 0,0,0 };			// 分割数
	bool bLighting = false;			// ライティング状況

	// 変数配列を宣言
	char aString[MAX_STRING];		// テキストの文字列の代入用
	char aTextureName[MAX_STRING];	// テクスチャのパス名

	// ポインタを宣言
	FILE  *pFile;							// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(MESH_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_MESHFIELD") == 0)
			{ // 読み込んだ文字列が SET_MESHFIELD の場合

				do
				{ // 読み込んだ文字列が END_SET_MESHFIELD ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が TYPE の場合
						fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
						fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);		// 位置を読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が RADIUS の場合
						fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
						fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);		// 向きを読み込む
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // 読み込んだ文字列が SIZE の場合
						fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
						fscanf(pFile, "%f%f%f", &size.x, &size.y, &size.z);		// 大きさを読み込む
					}
					else if (strcmp(&aString[0], "WIDTH") == 0)
					{ // 読み込んだ文字列が WIDTH の場合
						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &divi.x);					// 横幅の分割数を読み込む
					}
					else if (strcmp(&aString[0], "DEPTH") == 0)
					{ // 読み込んだ文字列が DEPTH の場合
						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &divi.z);					// 奥幅の分割数を読み込む
					}
					else if (strcmp(&aString[0], "TEXTURE") == 0)
					{ // 読み込んだ文字列が TEXTURE の場合
						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%s", &aTextureName[0]);			// パスを読み込む
					}
					else if (strcmp(&aString[0], "LIGHTING") == 0)
					{ // 読み込んだ文字列が LIGHTING の場合
						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%s", &aString[0]);				// OFFを読み込む

						// ライティング状況を設定する
						bLighting = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
					}

				} while (strcmp(&aString[0], "END_SET_MESHFIELD") != 0);	// 読み込んだ文字列が END_SET_MESHFIELD ではない場合ループ

				// フィールドの生成処理
				Create(pos, rot, size, divi, TYPE::TYPE_FIELD, aTextureName, bLighting);
			}
			else if (strcmp(&aString[0], "SET_MESHWALL") == 0)
			{ // 読み込んだ文字列が SET_MESHWALL の場合

				do
				{ // 読み込んだ文字列が END_SET_MESHWALL ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が TYPE の場合
						fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
						fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);		// 位置を読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が RADIUS の場合
						fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
						fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);		// 向きを読み込む
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // 読み込んだ文字列が SIZE の場合
						fscanf(pFile, "%s", &aString[0]);						// = を読み込む (不要)
						fscanf(pFile, "%f%f%f", &size.x, &size.y, &size.z);		// 大きさを読み込む
					}
					else if (strcmp(&aString[0], "WIDTH") == 0)
					{ // 読み込んだ文字列が WIDTH の場合
						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &divi.x);					// 横幅の分割数を読み込む
					}
					else if (strcmp(&aString[0], "HEIGHT") == 0)
					{ // 読み込んだ文字列が HEIGHT の場合
						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &divi.y);					// 縦幅の分割数を読み込む
					}
					else if (strcmp(&aString[0], "TEXTURE") == 0)
					{ // 読み込んだ文字列が TEXTURE の場合
						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%s", &aTextureName[0]);			// パスを読み込む
					}
					else if (strcmp(&aString[0], "LIGHTING") == 0)
					{ // 読み込んだ文字列が LIGHTING の場合
						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%s", &aString[0]);				// OFFを読み込む

						// ライティング状況を設定する
						bLighting = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
					}

				} while (strcmp(&aString[0], "END_SET_MESHWALL") != 0);	// 読み込んだ文字列が END_SET_MESHWALL ではない場合ループ

				// ウォールの生成処理
				Create(pos, rot, size, divi, TYPE::TYPE_WALL, aTextureName, bLighting);
			}
			else if (strcmp(&aString[0], "SET_MESHCYLINDER") == 0)
			{ // 読み込んだ文字列が SET_MESHCYLINDER の場合

				do
				{ // 読み込んだ文字列が END_SET_MESHCYLINDER ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合
						fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
						fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// 位置を読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合
						fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
						fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// 向きを読み込む
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // 読み込んだ文字列が CIRCUMSIZE の場合
						fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
						fscanf(pFile, "%f%f", &size.x, &size.y);			// 円周のサイズを読み込む

						// Z軸のサイズを設定する
						size.z = size.x;
					}
					else if (strcmp(&aString[0], "CIRCUM") == 0)
					{ // 読み込んだ文字列が CIRCUM の場合
						fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
						fscanf(pFile, "%d", &divi.x);						// 円周の分割数を読み込む

						// Z軸の分割数を設定する
						divi.z = divi.x;
					}
					else if (strcmp(&aString[0], "HEIGHT") == 0)
					{ // 読み込んだ文字列が HEIGHT の場合
						fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
						fscanf(pFile, "%d", &divi.y);						// 高さの分割数を読み込む
					}
					else if (strcmp(&aString[0], "TEXTURE") == 0)
					{ // 読み込んだ文字列が TEXTURE の場合
						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%s", &aTextureName[0]);			// パスを読み込む
					}
					else if (strcmp(&aString[0], "LIGHTING") == 0)
					{ // 読み込んだ文字列が LIGHTING の場合
						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%s", &aString[0]);				// OFFを読み込む

						// ライティング状況を設定する
						bLighting = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
					}

				} while (strcmp(&aString[0], "END_SET_MESHCYLINDER") != 0);	// 読み込んだ文字列が END_SET_MESHCYLINDER ではない場合ループ

				// 筒の生成処理
				Create(pos, rot, size, divi, TYPE::TYPE_CYLINDER, aTextureName, bLighting);
			}
			else if (strcmp(&aString[0], "SET_MESHDOME") == 0)
			{ // 読み込んだ文字列が SET_MESHDOME の場合

				do
				{ // 読み込んだ文字列が END_SET_MESHDOME ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合
						fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
						fscanf(pFile, "%f%f%f", &pos.x, &pos.y, &pos.z);	// 位置を読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合
						fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
						fscanf(pFile, "%f%f%f", &rot.x, &rot.y, &rot.z);	// 向きを読み込む
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // 読み込んだ文字列が CIRCUMSIZE の場合
						fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
						fscanf(pFile, "%f%f", &size.x, &size.y);			// 円周のサイズを読み込む

						// Z軸のサイズを設定する
						size.z = size.x;
					}
					else if (strcmp(&aString[0], "CIRCUM") == 0)
					{ // 読み込んだ文字列が CIRCUM の場合
						fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
						fscanf(pFile, "%d", &divi.x);						// 円周の分割数を読み込む

						// Z軸の分割数を設定する
						divi.z = divi.x;
					}
					else if (strcmp(&aString[0], "HEIGHT") == 0)
					{ // 読み込んだ文字列が HEIGHT の場合
						fscanf(pFile, "%s", &aString[0]);					// = を読み込む (不要)
						fscanf(pFile, "%d", &divi.y);						// 高さの分割数を読み込む
					}
					else if (strcmp(&aString[0], "TEXTURE") == 0)
					{ // 読み込んだ文字列が TEXTURE の場合
						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%s", &aTextureName[0]);			// パスを読み込む
					}
					else if (strcmp(&aString[0], "LIGHTING") == 0)
					{ // 読み込んだ文字列が LIGHTING の場合
						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%s", &aString[0]);				// OFFを読み込む

						// ライティング状況を設定する
						bLighting = (strcmp(&aString[0], "TRUE") == 0) ? true : false;
					}

				} while (strcmp(&aString[0], "END_SET_MESHDOME") != 0);	// 読み込んだ文字列が END_SET_MESHDOME ではない場合ループ

				// ドームの生成処理
				Create(pos, rot, size, divi, TYPE::TYPE_DOME, aTextureName, bLighting);
			}
		} while (nEnd != EOF);														// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);
	}
}