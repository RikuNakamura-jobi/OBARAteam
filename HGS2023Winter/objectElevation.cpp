//======================================================================================================================
//
//	起伏地面処理 [mesh_elevation.cpp]
//	Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "objectElevation.h"
#include "elevation_manager.h"
#include "Effect.h"
#include "input.h"
#include "useful.h"
#include "texture.h"

//----------------------------------------------------------------------------------------------------------------------
// マクロ定義
//----------------------------------------------------------------------------------------------------------------------
#define ELEVATION_TXT			"data/TXT/Elevation.txt"			// 起伏地面のテキスト
#define TEXTURE_DIVI_X			(0.2f)								// テクスチャの分割数(X軸)
#define TEXTURE_DIVI_Z			(1.0f / 6.0f)						// テクスチャの分割数(Z軸)

//================================
// コンストラクタ
//================================
CElevation::CElevation() : CObject(CObject::TYPE_FIELD, CObject::PRIORITY_BG)
{
	// 全ての値をクリアする
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// サイズ
	m_sizeDivi = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 1つ当たりの面のサイズ
	ZeroMemory(&m_mtxWorld, sizeof(D3DXMATRIX));	// ワールドマトリックス
	m_pVtxBuff = nullptr;							// 頂点バッファのポインタ
	m_pIdxBuff = nullptr;							// インデックスバッファへのポインタ
	m_nVtxX = 0;									// 横の頂点数
	m_nVtxZ = 0;									// 奥行の頂点数
	m_nDiviX = 0;									// 横の分割数
	m_nDiviZ = 0;									// 奥行の分割数
	m_nNumVtx = 0;									// 総頂点数
	m_nNumIdx = 0;									// 総インデックス数
	m_nTexIdx = NONE_TEXIDX;						// テクスチャのインデックス

	// 全ての値をクリアする
	m_pPrev = nullptr;		// 前のアウトボールへのポインタ
	m_pNext = nullptr;		// 次のアウトボールへのポインタ

	if (CElevationManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// マネージャーへの登録処理
		CElevationManager::Get()->Regist(this);
	}
}

//================================
// デストラクタ
//================================
CElevation::~CElevation()
{

}

//============================
// 前のポインタの設定処理
//============================
void CElevation::SetPrev(CElevation* pPrev)
{
	// 前のポインタを設定する
	m_pPrev = pPrev;
}

//============================
// 後のポインタの設定処理
//============================
void CElevation::SetNext(CElevation* pNext)
{
	// 次のポインタを設定する
	m_pNext = pNext;
}

//============================
// 前のポインタの設定処理
//============================
CElevation* CElevation::GetPrev(void) const
{
	// 前のポインタを返す
	return m_pPrev;
}

//============================
// 次のポインタの設定処理
//============================
CElevation* CElevation::GetNext(void) const
{
	// 次のポインタを返す
	return m_pNext;
}

//================================
// 初期化処理
//================================
HRESULT CElevation::Init(void)
{
	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

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

	// 頂点情報の頂点設定処理
	SetVertex();

	// インデックスの設定処理
	SetIndex();

	// 成功を返す
	return S_OK;
}

//================================
// 終了処理
//================================
void CElevation::Uninit(void)
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

	if (CElevationManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// リスト構造の引き抜き処理
		CElevationManager::Get()->Pull(this);
	}

	// リスト構造関係のポインタを NULL にする
	m_pPrev = nullptr;
	m_pNext = nullptr;
}

//================================
// 更新処理
//================================
void CElevation::Update(void)
{
	// 法線の設定処理
	SetNormalize();
}

//================================
// 描画処理
//================================
void CElevation::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

	// ライティングをOFFにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

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

	// ライティングをONにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//================================
// 情報の設定処理
//================================
void CElevation::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fSizeX, const float fSizeZ, const int nDiviX, const int nDiviZ)
{
	// 全ての値を初期化する
	m_pos = pos;						// 位置
	m_rot = rot;						// 向き
	m_size.x = fSizeX;					// サイズ(X軸)
	m_size.z = fSizeZ;					// サイズ(Z軸)
	m_nDiviX = nDiviX;					// 横の分割数
	m_nDiviZ = nDiviZ;					// 奥行の分割数
	m_nVtxX = m_nDiviX + 1;				// 横の頂点数
	m_nVtxZ = m_nDiviZ + 1;				// 奥行の頂点数
	m_nNumVtx = m_nVtxX * m_nVtxZ;		// 総頂点数
	m_nNumIdx = (2 * (m_nVtxX * m_nDiviZ)) + ((m_nVtxZ - 2) * 2);		// 総インデックス数
	m_nTexIdx = NONE_TEXIDX;			// テクスチャのインデックス
	m_sizeDivi = D3DXVECTOR3			// 1つ当たりの面のサイズ
	(
		m_size.x / (float)(m_nDiviX),
		0.0f,
		m_size.z / (float)(m_nDiviZ)
	);
}

//================================
// テクスチャの割り当て処理
//================================
void CElevation::BindTexture(int nIdx)
{
	// テクスチャを割り当てる
	m_nTexIdx = nIdx;
}

//================================
// 位置の取得処理
//================================
D3DXVECTOR3 CElevation::GetPos(void) const
{
	// 位置を返す
	return m_pos;
}

//================================
// サイズの取得処理
//================================
D3DXVECTOR3 CElevation::GetSize(void) const
{
	// サイズを返す
	return m_size;
}

//================================
// X軸の頂点数の取得処理
//================================
int CElevation::GetVtxX(void) const
{
	// X軸の頂点数を返す
	return m_nVtxX;
}

//================================
// Z軸の頂点数の取得処理
//================================
int CElevation::GetVtxZ(void) const
{
	// Z軸の頂点数を返す
	return m_nVtxZ;
}

//================================
// 全頂点数の取得処理
//================================
int CElevation::GetVtxNum(void) const
{
	// 全頂点数を返す
	return m_nNumVtx;
}

//================================
// 頂点の設定処理
//================================
void CElevation::SetVtxHeight(const int nNum, const float fHeight)
{
	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の高さを加算する
	pVtx[nNum].pos.y = fHeight;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================
// 頂点の加算処理
//================================
void CElevation::AddVertex(const int nNum, const float fAdd)
{
	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の高さを加算する
	pVtx[nNum].pos.y += fAdd;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================
// 頂点の設定処理
//================================
void CElevation::SetVertex(void)
{
	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDep = 0; nCntDep < m_nVtxZ; nCntDep++)
	{
		for (int nCntWid = 0; nCntWid < m_nVtxX; nCntWid++)
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
			pVtx[0].tex = D3DXVECTOR2(nCntWid * TEXTURE_DIVI_X, nCntDep * TEXTURE_DIVI_Z);

			pVtx++;				// 頂点データを進める
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================
// 法線の設定処理
//================================
void CElevation::SetNormalize(void)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ
	D3DXVECTOR3 nor[6] = {};					// 保存用法線
	D3DXVECTOR3 vec[2] = {};					// 保存用ベクトル
	D3DXVECTOR3 SumNor = {};					// 合計法線
	int nNum = 0;								// 現在の番号

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDep = 0; nCntDep < m_nVtxZ; nCntDep++)
	{
		for (int nCntWid = 0; nCntWid < m_nVtxX; nCntWid++)
		{
			// 現在の頂点の番号を設定する
			nNum = (m_nVtxX * nCntDep) + nCntWid;

			if (nNum == 0)
			{ // 最初(左奥角)の場合

				vec[0] = pVtx[nNum].pos - pVtx[nNum + m_nVtxX].pos;
				vec[1] = pVtx[nNum + m_nVtxX + 1].pos - pVtx[nNum + m_nVtxX].pos;

				D3DXVec3Cross(&nor[0], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[0], &nor[0]);

				vec[0] = pVtx[nNum + m_nVtxX + 1].pos - pVtx[nNum + 1].pos;
				vec[1] = pVtx[nNum].pos - pVtx[nNum + 1].pos;

				D3DXVec3Cross(&nor[1], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[1], &nor[1]);

				SumNor = (nor[0] + nor[1]) / 2;

				D3DXVec3Normalize(&SumNor, &SumNor);

				// 法線座標の設定
				pVtx[nNum].nor = SumNor;
			}
			else if (nCntDep == 0 && nCntWid != m_nDiviX)
			{ // 奥縁(角以外)の場合

				vec[0] = pVtx[nNum + m_nVtxX].pos - pVtx[nNum].pos;
				vec[1] = pVtx[nNum - 1].pos - pVtx[nNum].pos;

				D3DXVec3Cross(&nor[0], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[0], &nor[0]);

				vec[0] = pVtx[nNum].pos - pVtx[nNum + m_nVtxX].pos;
				vec[1] = pVtx[nNum + m_nVtxX + 1].pos - pVtx[nNum + m_nVtxX].pos;

				D3DXVec3Cross(&nor[1], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[1], &nor[1]);

				vec[0] = pVtx[nNum + m_nVtxX + 1].pos - pVtx[nNum + 1].pos;
				vec[1] = pVtx[nNum].pos - pVtx[nNum + 1].pos;

				D3DXVec3Cross(&nor[2], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[2], &nor[2]);

				SumNor = (nor[0] + nor[1] + nor[2]) / 3;

				D3DXVec3Normalize(&SumNor, &SumNor);

				// 法線座標の設定
				pVtx[nNum].nor = SumNor;
			}
			else if (nCntDep == 0 && nCntWid == m_nDiviX)
			{ // 右奥角の場合

				vec[0] = pVtx[nNum + m_nVtxX].pos - pVtx[nNum].pos;
				vec[1] = pVtx[nNum - 1].pos - pVtx[nNum].pos;

				D3DXVec3Cross(&nor[0], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[0], &nor[0]);

				// 法線座標の設定
				pVtx[nNum].nor = nor[0];
			}
			else if (nCntWid == 0 && nCntDep != m_nDiviZ)
			{ // 左縁(角以外)の場合

				vec[0] = pVtx[nNum].pos - pVtx[nNum + m_nVtxX].pos;
				vec[1] = pVtx[nNum + m_nVtxX + 1].pos - pVtx[nNum + m_nVtxX].pos;

				D3DXVec3Cross(&nor[0], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[0], &nor[0]);

				vec[0] = pVtx[nNum - m_nVtxX].pos - pVtx[nNum].pos;
				vec[1] = pVtx[nNum + 1].pos - pVtx[nNum].pos;

				D3DXVec3Cross(&nor[1], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[1], &nor[1]);

				vec[0] = pVtx[nNum + m_nVtxX + 1].pos - pVtx[nNum + 1].pos;
				vec[1] = pVtx[nNum].pos - pVtx[nNum + 1].pos;

				D3DXVec3Cross(&nor[2], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[2], &nor[2]);

				SumNor = (nor[0] + nor[1] + nor[2]) / 3;

				D3DXVec3Normalize(&SumNor, &SumNor);

				// 法線座標の設定
				pVtx[nNum].nor = SumNor;
			}
			else if (nCntWid == m_nDiviX && nCntDep != m_nDiviZ)
			{ // 右縁(角以外)の場合

				vec[0] = pVtx[nNum + m_nVtxX].pos - pVtx[nNum].pos;
				vec[1] = pVtx[nNum - 1].pos - pVtx[nNum].pos;

				D3DXVec3Cross(&nor[0], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[0], &nor[0]);

				vec[0] = pVtx[nNum - m_nVtxX - 1].pos - pVtx[nNum - 1].pos;
				vec[1] = pVtx[nNum].pos - pVtx[nNum - 1].pos;

				D3DXVec3Cross(&nor[1], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[1], &nor[1]);

				vec[0] = pVtx[nNum].pos - pVtx[nNum - m_nVtxX].pos;
				vec[1] = pVtx[nNum - m_nVtxX - 1].pos - pVtx[nNum - m_nVtxX].pos;

				D3DXVec3Cross(&nor[2], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[2], &nor[2]);

				SumNor = (nor[0] + nor[1] + nor[2]) / 3;

				D3DXVec3Normalize(&SumNor, &SumNor);

				// 法線座標の設定
				pVtx[nNum].nor = SumNor;
			}
			else if (nCntDep == m_nDiviZ && nCntWid == 0)
			{ // 左手前奥の場合

				vec[0] = pVtx[nNum - m_nVtxX].pos - pVtx[nNum].pos;
				vec[1] = pVtx[nNum + 1].pos - pVtx[nNum].pos;

				D3DXVec3Cross(&nor[0], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[0], &nor[0]);

				// 法線座標の設定
				pVtx[nNum].nor = nor[0];
			}
			else if (nCntDep == m_nDiviZ && nCntWid != m_nDiviX)
			{ // 手前縁(角以外)の場合

				vec[0] = pVtx[nNum - m_nVtxX - 1].pos - pVtx[nNum - 1].pos;
				vec[1] = pVtx[nNum].pos - pVtx[nNum - 1].pos;

				D3DXVec3Cross(&nor[0], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[0], &nor[0]);

				vec[0] = pVtx[nNum].pos - pVtx[nNum - m_nVtxX].pos;
				vec[1] = pVtx[nNum - m_nVtxX - 1].pos - pVtx[nNum - m_nVtxX].pos;

				D3DXVec3Cross(&nor[1], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[1], &nor[1]);

				vec[0] = pVtx[nNum - m_nVtxX].pos - pVtx[nNum].pos;
				vec[1] = pVtx[nNum + 1].pos - pVtx[nNum].pos;

				D3DXVec3Cross(&nor[2], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[2], &nor[2]);

				SumNor = (nor[0] + nor[1] + nor[2]) / 3;

				D3DXVec3Normalize(&SumNor, &SumNor);

				// 法線座標の設定
				pVtx[nNum].nor = SumNor;
			}
			else if (nNum == (m_nNumVtx - 1))
			{ // 最後の場合

				vec[0] = pVtx[nNum - m_nVtxX - 1].pos - pVtx[nNum - 1].pos;
				vec[1] = pVtx[nNum].pos - pVtx[nNum - 1].pos;

				D3DXVec3Cross(&nor[0], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[0], &nor[0]);

				vec[0] = pVtx[nNum].pos - pVtx[nNum - m_nVtxX].pos;
				vec[1] = pVtx[nNum - m_nVtxX - 1].pos - pVtx[nNum - m_nVtxX].pos;

				D3DXVec3Cross(&nor[1], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[1], &nor[1]);

				SumNor = (nor[0] + nor[1]) / 2;

				D3DXVec3Normalize(&SumNor, &SumNor);

				// 法線座標の設定
				pVtx[nNum].nor = SumNor;
			}
			else
			{ // 上記以外(真ん中)の場合

				vec[0] = pVtx[nNum + m_nVtxX].pos - pVtx[nNum].pos;
				vec[1] = pVtx[nNum - 1].pos - pVtx[nNum].pos;

				D3DXVec3Cross(&nor[0], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[0], &nor[0]);

				vec[0] = pVtx[nNum - m_nVtxX - 1].pos - pVtx[nNum - 1].pos;
				vec[1] = pVtx[nNum].pos - pVtx[nNum - 1].pos;

				D3DXVec3Cross(&nor[1], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[1], &nor[1]);

				vec[0] = pVtx[nNum].pos - pVtx[nNum - m_nVtxX].pos;
				vec[1] = pVtx[nNum - m_nVtxX - 1].pos - pVtx[nNum - m_nVtxX].pos;

				D3DXVec3Cross(&nor[2], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[2], &nor[2]);

				vec[0] = pVtx[nNum - m_nVtxX].pos - pVtx[nNum].pos;
				vec[1] = pVtx[nNum + 1].pos - pVtx[nNum].pos;

				D3DXVec3Cross(&nor[3], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[3], &nor[3]);

				vec[0] = pVtx[nNum + m_nVtxX + 1].pos - pVtx[nNum + 1].pos;
				vec[1] = pVtx[nNum].pos - pVtx[nNum + 1].pos;

				D3DXVec3Cross(&nor[4], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[4], &nor[4]);

				vec[0] = pVtx[nNum].pos - pVtx[nNum + m_nVtxX].pos;
				vec[1] = pVtx[nNum + m_nVtxX + 1].pos - pVtx[nNum + m_nVtxX].pos;

				D3DXVec3Cross(&nor[5], &vec[0], &vec[1]);

				D3DXVec3Normalize(&nor[5], &nor[5]);

				SumNor = (nor[0] + nor[1] + nor[2] + nor[3] + nor[4] + nor[5]) / 6;

				D3DXVec3Normalize(&SumNor, &SumNor);

				// 法線座標の設定
				pVtx[nNum].nor = SumNor;
			}
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================
// インデックスの設定処理
//================================
void CElevation::SetIndex(void)
{
	// ローカル変数宣言
	WORD      *pIdx;							// インデックス情報へのポインタ
	int nIdx = 0;								// インデックス数

	//------------------------------------------------------------------------------------------------------------------
	//	インデックス情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// インデックスバッファをロックし、頂点番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntDep = 1; nCntDep < m_nVtxZ; nCntDep++)
	{
		if (nCntDep != 1)
		{ // 最初以外

			// インデックス数を設定する
			pIdx[0] = (WORD)(m_nVtxX * nCntDep);

			// インデックスデータを加算する
			pIdx++;
			nIdx++;
		}

		for (int nCntWid = 0; nCntWid < m_nVtxX; nCntWid++)
		{ // 1層ごとに設定する

			// インデックス数を設定する
			pIdx[0] = (WORD)((m_nVtxX * nCntDep) + nCntWid);
			pIdx[1] = (WORD)((m_nVtxX * nCntDep) - (m_nVtxX - nCntWid));

			// インデックスデータを2つ進める
			pIdx += 2;
			nIdx += 2;

			if (nCntWid == (m_nVtxX - 1) && nCntDep != (m_nVtxZ - 1))
			{ // 折り返しに入った場合

				// インデックス数を設定する
				pIdx[0] = (WORD)((m_nVtxX * nCntDep) - (m_nVtxX - nCntWid));

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
// 起伏の当たり判定
//================================
float CElevation::ElevationCollision(const D3DXVECTOR3& pos)
{
	// ローカル変数宣言
	D3DXVECTOR3 nor, vec1, vec2;	// 位置
	float fHeight = pos.y;			// 対象の高さ
	int nNum = 0;					// 現在の番号

	// 頂点の番号
	int nVtxLeftUp;		// 左上
	int nVtxLeftDown;	// 左下
	int nVtxRightUp;	// 右上
	int nVtxRightDown;	// 右下
	D3DXVECTOR3 vtxLeftUp;		// 左上の位置
	D3DXVECTOR3 vtxLeftDown;	// 左下の位置
	D3DXVECTOR3 vtxRightUp;		// 右上の位置
	D3DXVECTOR3 vtxRightDown;	// 右下の位置

	if (pos.x <= m_pos.x + m_size.x &&
		pos.x >= m_pos.x - m_size.x &&
		pos.z <= m_pos.z + m_size.z &&
		pos.z >= m_pos.z - m_size.z)
	{ // 地面の範囲にいる場合

		VERTEX_3D * pVtx;				//頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntDep = 0; nCntDep < m_nVtxZ - 1; nCntDep++)
		{
			for (int nCntWid = 0; nCntWid < m_nVtxX - 1; nCntWid++)
			{
				// 現在の頂点の番号を設定する
				nNum = (m_nVtxX * nCntDep) + nCntWid;

				// 頂点の番号を設定する
				nVtxLeftUp = nNum;					// 左上
				nVtxLeftDown = nNum + m_nVtxX;		// 左下
				nVtxRightUp = nNum + 1;				// 右上
				nVtxRightDown = nNum + m_nVtxX + 1;	// 右下

				// 頂点の位置を設定する
				vtxLeftUp = m_pos + pVtx[nVtxLeftUp].pos;
				vtxLeftDown = m_pos + pVtx[nVtxLeftDown].pos;
				vtxRightUp = m_pos + pVtx[nVtxRightUp].pos;
				vtxRightDown = m_pos + pVtx[nVtxRightDown].pos;

				if (vtxLeftUp.z >= pos.z &&
					vtxLeftUp.x <= pos.x &&
					vtxRightDown.z <= pos.z &&
					vtxRightDown.x >= pos.x)
				{ // 四角形の中にいた場合

					if (useful::LineOuterProductXZ(vtxLeftDown, vtxLeftUp, pos) >= 0 &&
						useful::LineOuterProductXZ(vtxLeftUp, vtxRightDown, pos) >= 0 &&
						useful::LineOuterProductXZ(vtxRightDown, vtxLeftDown, pos) >= 0)
					{ // 真ん中の境界線より左側に居た場合

						// 法線の計算(正規化)
						//NormalizeVector(nor, pVtx[nVtxRightDown].pos, pVtx[nVtxLeftUp].pos, pVtx[nVtxLeftDown].pos);

						vec1 = vtxLeftUp - vtxLeftDown;
						vec2 = vtxRightDown - vtxLeftDown;

						D3DXVec3Cross(&nor, &vec1, &vec2);

						D3DXVec3Normalize(&nor, &nor);

						if (nor.y != 0.0f)
						{ // 法線のYが0.0f以外の場合

							// 高さを設定する
							fHeight = (((pos.x - vtxLeftDown.x) * nor.x + (-vtxLeftDown.y) * nor.y + (pos.z - vtxLeftDown.z) * nor.z) * -1.0f) / nor.y;

							// 高さを返す
							return fHeight;
						}
					}
					else if (useful::LineOuterProductXZ(vtxLeftUp, vtxRightUp, pos) >= 0 &&
						useful::LineOuterProductXZ(vtxRightDown, vtxLeftUp, pos) >= 0 &&
						useful::LineOuterProductXZ(vtxRightUp, vtxRightDown, pos) >= 0)
					{ // 真ん中の境界線より右側に居た場合

						// 法線の計算(正規化)
						//NormalizeVector(nor, pVtx[nVtxLeftUp].pos, pVtx[nVtxRightDown].pos, pVtx[nVtxRightUp].pos);

						vec1 = vtxRightDown - vtxRightUp;
						vec2 = vtxLeftUp - vtxRightUp;

						D3DXVec3Cross(&nor, &vec1, &vec2);

						D3DXVec3Normalize(&nor, &nor);

						if (nor.y != 0.0f)
						{ // 法線のYが0.0f以外の場合

							// 高さを設定する
							fHeight = (((pos.x - vtxRightUp.x) * nor.x + (-vtxRightUp.y) * nor.y + (pos.z - vtxRightUp.z) * nor.z) * -1.0f) / nor.y;

							// 高さを返す
							return fHeight;
						}
					}
				}
			}
		}

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();

		// 高さを返す
		return fHeight;
	}
	else
	{ // 地面の範囲内にいない場合

		// 高さを返す
		return pos.y;
	}
}

//================================
// 起伏の当たり判定(範囲判定付き)
//================================
float CElevation::ElevationCollision(const D3DXVECTOR3& pos, bool& bRange)
{
		// ローカル変数宣言
	D3DXVECTOR3 nor, vec1, vec2;	// 位置
	float fHeight = pos.y;			// 対象の高さ
	int nNum = 0;					// 現在の番号

	// 頂点の番号
	int nVtxLeftUp;		// 左上
	int nVtxLeftDown;	// 左下
	int nVtxRightUp;	// 右上
	int nVtxRightDown;	// 右下
	D3DXVECTOR3 vtxLeftUp;		// 左上の位置
	D3DXVECTOR3 vtxLeftDown;	// 左下の位置
	D3DXVECTOR3 vtxRightUp;		// 右上の位置
	D3DXVECTOR3 vtxRightDown;	// 右下の位置

	if (pos.x <= m_pos.x + (m_size.x * 0.5f) &&
		pos.x >= m_pos.x - (m_size.x * 0.5f) &&
		pos.z <= m_pos.z + (m_size.z * 0.5f) &&
		pos.z >= m_pos.z - (m_size.z * 0.5f))
	{ // 地面の範囲にいる場合

		VERTEX_3D * pVtx;				//頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntDep = 0; nCntDep < m_nVtxZ - 1; nCntDep++)
		{
			for (int nCntWid = 0; nCntWid < m_nVtxX - 1; nCntWid++)
			{
				// 現在の頂点の番号を設定する
				nNum = (m_nVtxX * nCntDep) + nCntWid;

				// 頂点の番号を設定する
				nVtxLeftUp = nNum;					// 左上
				nVtxLeftDown = nNum + m_nVtxX;		// 左下
				nVtxRightUp = nNum + 1;				// 右上
				nVtxRightDown = nNum + m_nVtxX + 1;	// 右下

				// 頂点の位置を設定する
				vtxLeftUp = m_pos + pVtx[nVtxLeftUp].pos;
				vtxLeftDown = m_pos + pVtx[nVtxLeftDown].pos;
				vtxRightUp = m_pos + pVtx[nVtxRightUp].pos;
				vtxRightDown = m_pos + pVtx[nVtxRightDown].pos;

				if (vtxLeftUp.z >= pos.z &&
					vtxLeftUp.x <= pos.x &&
					vtxRightDown.z <= pos.z &&
					vtxRightDown.x >= pos.x)
				{ // 四角形の中にいた場合

					if (useful::LineOuterProductXZ(vtxLeftDown, vtxLeftUp, pos) >= 0 &&
						useful::LineOuterProductXZ(vtxLeftUp, vtxRightDown, pos) >= 0 &&
						useful::LineOuterProductXZ(vtxRightDown, vtxLeftDown, pos) >= 0)
					{ // 真ん中の境界線より左側に居た場合

						// 法線の計算(正規化)
						//NormalizeVector(nor, pVtx[nVtxRightDown].pos, pVtx[nVtxLeftUp].pos, pVtx[nVtxLeftDown].pos);

						vec1 = vtxLeftUp - vtxLeftDown;
						vec2 = vtxRightDown - vtxLeftDown;

						D3DXVec3Cross(&nor, &vec1, &vec2);

						D3DXVec3Normalize(&nor, &nor);

						if (nor.y != 0.0f)
						{ // 法線のYが0.0f以外の場合

							// 高さを設定する
							fHeight = (((pos.x - vtxLeftDown.x) * nor.x + (-vtxLeftDown.y) * nor.y + (pos.z - vtxLeftDown.z) * nor.z) * -1.0f) / nor.y;

							// 範囲内にいる
							bRange = true;

							// 高さを返す
							return fHeight;
						}
					}
					else if (useful::LineOuterProductXZ(vtxLeftUp, vtxRightUp, pos) >= 0 &&
						useful::LineOuterProductXZ(vtxRightDown, vtxLeftUp, pos) >= 0 &&
						useful::LineOuterProductXZ(vtxRightUp, vtxRightDown, pos) >= 0)
					{ // 真ん中の境界線より右側に居た場合

						// 法線の計算(正規化)
						//NormalizeVector(nor, pVtx[nVtxLeftUp].pos, pVtx[nVtxRightDown].pos, pVtx[nVtxRightUp].pos);

						vec1 = vtxRightDown - vtxRightUp;
						vec2 = vtxLeftUp - vtxRightUp;

						D3DXVec3Cross(&nor, &vec1, &vec2);

						D3DXVec3Normalize(&nor, &nor);

						if (nor.y != 0.0f)
						{ // 法線のYが0.0f以外の場合

							// 高さを設定する
							fHeight = (((pos.x - vtxRightUp.x) * nor.x + (-vtxRightUp.y) * nor.y + (pos.z - vtxRightUp.z) * nor.z) * -1.0f) / nor.y;

							// 範囲内にいる
							bRange = true;

							// 高さを返す
							return fHeight;
						}
					}
				}
			}
		}

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();

		// 範囲内にいる
		bRange = true;

		// 高さを返す
		return fHeight;
	}
	else
	{ // 地面の範囲内にいない場合

		// 高さを返す
		return pos.y;
	}
}

//================================
// 近くの頂点を探す処理
//================================
int CElevation::NearVertexSearch(const D3DXVECTOR3& pos)
{
	// ローカル変数宣言
	D3DXVECTOR3 distance;		// 距離
	int nNum = 0;				// 頂点の番号
	int nNearNum = 0;			// 一番近い番号
	float fSum = 0.0f;			// 合計値
	float fNearSum = 0.0f;		// 一番近い頂点の合計値

	VERTEX_3D * pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDep = 0; nCntDep < m_nVtxZ; nCntDep++)
	{
		for (int nCntWid = 0; nCntWid < m_nVtxX; nCntWid++)
		{
			// 現在の頂点の番号を設定する
			nNum = (m_nVtxX * nCntDep) + nCntWid;

			// 距離を測る
			distance.x = fabsf(pos.x - (m_pos.x + pVtx[nNum].pos.x));
			distance.z = fabsf(pos.z - (m_pos.z + pVtx[nNum].pos.z));

			// 合計値を取る
			fSum = distance.x + distance.z;

			if (nNum == 0)
			{ // 一番近い番号が無い場合

				// 候補の距離を設定する
				fNearSum = fSum;

				// 候補の番号を設定する
				nNearNum = nNum;
			}
			else if(fNearSum >= fSum)
			{ // 現在の距離の方が短い場合

				// 候補の距離を設定する
				fNearSum = fSum;

				// 候補の番号を設定する
				nNearNum = nNum;
			}
		}
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 番号を返す
	return nNearNum;
}

//================================
// 生成処理
//================================
CElevation* CElevation::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fSizeX, const float fSizeZ, const int nDiviX, const int nDiviZ, char* texturename)
{
	// ローカルオブジェクトを生成
	CElevation* pMesh = nullptr;	// オブジェクト3Dのインスタンスを生成

	if (pMesh == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pMesh = new CElevation;
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

		// 情報の設定処理
		pMesh->SetData(pos, rot, fSizeX, fSizeZ, nDiviX, nDiviZ);

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
void CElevation::TxtSet(void)
{
	// 変数を宣言
	CElevation* pElevation = nullptr;	// 起伏のポインタ
	int nEnd;			// テキスト読み込み終了の確認用
	D3DXVECTOR3 pos = NONE_D3DXVECTOR3;		// 位置
	D3DXVECTOR3 rot = NONE_D3DXVECTOR3;		// 向き
	float fSizeX = 0.0f;					// 横のサイズ
	float fSizeZ = 0.0f;					// 奥行のサイズ
	int nDiviX = 0;							// 横の分割数
	int nDiviZ = 0;							// 奥行の分割数
	int nVtxX = 0;							// 頂点数(X軸)
	int nVtxZ = 0;							// 頂点数(Z軸)

	// 変数配列を宣言
	char aString[MAX_STRING];		// テキストの文字列の代入用
	char aTextureName[MAX_STRING];	// テクスチャのパス名

	// ポインタを宣言
	FILE  *pFile;					// ファイルポインタ
	VERTEX_3D * pVtx;				// 頂点情報へのポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(ELEVATION_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "SET_ELEVATION") == 0)
			{ // 読み込んだ文字列が SET_ELEVATION の場合

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
						fscanf(pFile, "%f%f", &fSizeX, &fSizeZ);				// 大きさを読み込む
					}
					else if (strcmp(&aString[0], "WIDTH") == 0)
					{ // 読み込んだ文字列が WIDTH の場合
						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &nDiviX);					// 横幅の分割数を読み込む
					}
					else if (strcmp(&aString[0], "DEPTH") == 0)
					{ // 読み込んだ文字列が DEPTH の場合
						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &nDiviZ);					// 奥幅の分割数を読み込む
					}
					else if (strcmp(&aString[0], "TEXTURE") == 0)
					{ // 読み込んだ文字列が TEXTURE の場合
						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%s", &aTextureName[0]);			// パスを読み込む

						// 生成処理
						pElevation = Create(pos, rot, fSizeX, fSizeZ, nDiviX, nDiviZ, aTextureName);

						// 頂点数(X軸)を設定する
						nVtxX = pElevation->m_nVtxX;

						// 頂点数(Z軸)を設定する
						nVtxZ = pElevation->m_nVtxZ;
					}
					else if (strcmp(&aString[0], "ELEV") == 0)
					{ // 読み込んだ文字列が ELEV の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)

						//頂点バッファをロックし、頂点情報へのポインタを取得
						pElevation->m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

						for (int nCnt = 0; nCnt < nVtxX * nVtxZ; nCnt++)
						{
							// 座標を読み込む
							fscanf(pFile, "%f %f %f", &pVtx[nCnt].pos.x, &pVtx[nCnt].pos.y, &pVtx[nCnt].pos.z);
						}

						//頂点バッファをアンロックする
						pElevation->m_pVtxBuff->Unlock();
					}
				} while (strcmp(&aString[0], "END_SET_ELEVATION") != 0);	// 読み込んだ文字列が END_SET_ELEVATION ではない場合ループ
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