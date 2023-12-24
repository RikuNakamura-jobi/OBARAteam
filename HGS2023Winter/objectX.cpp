//============================================================
//
// Xファイル処理 [xfile.cpp]
// Author：小原立暉
//
//============================================================
//************************************************************
// インクルードファイル
//************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"
#include "texture.h"

//------------------------------------------------------------
// コンスト定義
//------------------------------------------------------------
const char* CXFile::c_apModelData[CXFile::TYPE_MAX] =		// モデルの名前
{
	// 演出モデル
	"data\\MODEL\\Platform\\FracScrew.x",		// ネジ(破片)
	"data\\MODEL\\Platform\\FracGear.x",		// 歯車(破片)
	"data\\MODEL\\Platform\\FracRing.x",		// 輪(破片)
	"data\\MODEL\\Platform\\FracWood.x",		// 木片(破片)
	"data\\MODEL\\Platform\\Ripple.x",			// 波紋
};

//------------------------------------------------------------
// モデルの情報
//------------------------------------------------------------
CXFile::SXFile CXFile::m_apModel[CXFile::TYPE_MAX] = {};

//============================================================
// コンストラクタ
//============================================================
CXFile::CXFile()
{

}

//============================================================
// デストラクタ
//============================================================
CXFile::~CXFile()
{

}

//============================================================
// モデルの初期化処理
//============================================================
HRESULT CXFile::Init(void)
{
	for (int nCnt = 0; nCnt < CXFile::TYPE_MAX; nCnt++)
	{
		// モデルの情報を初期化する
		m_apModel[nCnt].pMesh = nullptr;			// メッシュ (頂点情報) へのポインタ
		m_apModel[nCnt].pBuffMat = nullptr;			// マテリアルへのポインタ
		m_apModel[nCnt].dwNumMat = 0;				// マテリアルの数
		m_apModel[nCnt].vtxMin = INIT_VTX_MIN;		// 最小の頂点座標
		m_apModel[nCnt].vtxMax = INIT_VTX_MAX;		// 最大の頂点座標
		m_apModel[nCnt].collsize = INIT_SIZE;		// 当たり判定のサイズ
		m_apModel[nCnt].fRadius = 0.0f;				// 半径

		for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
		{
			m_apModel[nCnt].m_nTexIdx[nCntMat] = NONE_TEXIDX;		// テクスチャのインデックス
		}
	}

	// xファイルの読み込み
	if (FAILED(LoadXFile()))
	{ // xファイルの読み込みに失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 当たり判定の作成
	SetCollision();

	// テクスチャの読み込み
	if (FAILED(LoadTexture()))
	{ // テクスチャの読み込みに失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
// モデルの終了処理
//============================================================
void CXFile::Uninit(void)
{
	// テクスチャの破棄
	for (int nCntModel = 0; nCntModel < CXFile::TYPE_MAX; nCntModel++)
	{ // モデルの最大数分繰り返す

		if (m_apModel[nCntModel].pMesh != nullptr)
		{ // 変数 (m_apModel[nCntModel].pMesh) がNULLではない場合

			m_apModel[nCntModel].pMesh->Release();
			m_apModel[nCntModel].pMesh = nullptr;
		}

		if (m_apModel[nCntModel].pBuffMat != nullptr)
		{ // 変数 (m_apModel[nCntModel].pBuffMat) がNULLではない場合

			m_apModel[nCntModel].pBuffMat->Release();
			m_apModel[nCntModel].pBuffMat = nullptr;
		}
	}

// デバッグモード
#ifdef _DEBUG

	// テクスチャの破棄
	for (int nCntModel = 0; nCntModel < CXFile::TYPE_MAX; nCntModel++)
	{ // モデルの最大数分繰り返す

		if (m_apModel[nCntModel].pMesh != nullptr)
		{ // 変数 (m_apModel[nCntModel].pMesh) がNULLではない場合

			// 停止
			assert(false);
		}

		if (m_apModel[nCntModel].pBuffMat != nullptr)
		{ // 変数 (m_apModel[nCntModel].pBuffMat) がNULLではない場合

			// 停止
			assert(false);
		}
	}

#endif
}

//============================================================
//	xファイルの読み込み
//============================================================
HRESULT CXFile::LoadXFile(void)
{
	// 変数を宣言
	HRESULT hr;		// 異常終了の確認用

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	for (int nCntModel = 0; nCntModel < CXFile::TYPE_MAX; nCntModel++)
	{ // モデルの最大数分繰り返す

		// xファイルの読み込み
		hr = D3DXLoadMeshFromX
		( // 引数
			c_apModelData[nCntModel],		// モデルの相対パス
			D3DXMESH_SYSTEMMEM,				// メッシュ作成用オプション
			pDevice,						// デバイスへのポインタ
			NULL,							// 隣接性データ
			&m_apModel[nCntModel].pBuffMat,	// マテリアルへのポインタ
			NULL,							// エフェクトデータ
			&m_apModel[nCntModel].dwNumMat,	// マテリアルの数
			&m_apModel[nCntModel].pMesh		// メッシュ (頂点情報) へのポインタ
		);

		if (FAILED(hr))
		{ // xファイルの読み込みに失敗した場合

			// 停止
			assert(false);

			// 失敗を返す
			return E_FAIL;
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	当たり判定の作成
//============================================================
void CXFile::SetCollision(void)
{
	// 変数を宣言
	int         nNumVtx;		// モデルの頂点数
	DWORD       dwSizeFVF;		// モデルの頂点フォーマットのサイズ
	BYTE        *pVtxBuff;		// モデルの頂点バッファへのポインタ
	D3DXVECTOR3 vtx;			// モデルの頂点座標

	// 当たり判定の作成
	for (int nCntModel = 0; nCntModel < CXFile::TYPE_MAX; nCntModel++)
	{ // モデルの最大数分繰り返す

		// モデルの頂点数を取得
		nNumVtx = m_apModel[nCntModel].pMesh->GetNumVertices();

		// モデルの頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(m_apModel[nCntModel].pMesh->GetFVF());

		// モデルの頂点バッファをロック
		m_apModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{ // モデルの頂点数分繰り返す

			// モデルの頂点座標を代入
			vtx = *(D3DXVECTOR3*)pVtxBuff;

			// 頂点座標 (x) の設定
			if (vtx.x < m_apModel[nCntModel].vtxMin.x)
			{ // 今回の頂点座標 (x) が、現状の頂点座標 (x) よりも小さい場合

				// 今回の頂点情報 (x) を代入
				m_apModel[nCntModel].vtxMin.x = vtx.x;
			}
			else if (vtx.x > m_apModel[nCntModel].vtxMax.x)
			{ // 今回の頂点座標 (x) が、現状の頂点座標 (x) よりも大きい場合

				// 今回の頂点情報 (x) を代入
				m_apModel[nCntModel].vtxMax.x = vtx.x;
			}

			// 頂点座標 (y) の設定
			if (vtx.y < m_apModel[nCntModel].vtxMin.y)
			{ // 今回の頂点座標 (y) が、現状の頂点座標 (y) よりも小さい場合

				// 今回の頂点情報 (y) を代入
				m_apModel[nCntModel].vtxMin.y = vtx.y;
			}
			else if (vtx.y > m_apModel[nCntModel].vtxMax.y)
			{ // 今回の頂点座標 (y) が、現状の頂点座標 (y) よりも大きい場合

				// 今回の頂点情報 (y) を代入
				m_apModel[nCntModel].vtxMax.y = vtx.y;
			}

			// 頂点座標 (z) の設定
			if (vtx.z < m_apModel[nCntModel].vtxMin.z)
			{ // 今回の頂点座標 (z) が、現状の頂点座標 (z) よりも小さい場合

				// 今回の頂点情報 (z) を代入
				m_apModel[nCntModel].vtxMin.z = vtx.z;
			}
			else if (vtx.z > m_apModel[nCntModel].vtxMax.z)
			{ // 今回の頂点座標 (z) が、現状の頂点座標 (z) よりも大きい場合

				// 今回の頂点情報 (z) を代入
				m_apModel[nCntModel].vtxMax.z = vtx.z;
			}

			// 頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += dwSizeFVF;
		}

		// モデルの頂点バッファをアンロック
		m_apModel[nCntModel].pMesh->UnlockVertexBuffer();

		// モデルサイズを求める
		m_apModel[nCntModel].collsize = m_apModel[nCntModel].vtxMax - m_apModel[nCntModel].vtxMin;

		// モデルの円の当たり判定を作成
		m_apModel[nCntModel].fRadius = ((m_apModel[nCntModel].collsize.x * 0.5f) + (m_apModel[nCntModel].collsize.y * 0.5f));
	}
}

//============================================================
//	テクスチャの読み込み
//============================================================
HRESULT CXFile::LoadTexture(void)
{
	// デバイスを取得する
	D3DXMATERIAL     *pMat;						// マテリアルへのポインタ

	// テクスチャの読み込み
	for (int nCntModel = 0; nCntModel < CXFile::TYPE_MAX; nCntModel++)
	{ // モデルに使用するモデルの最大数分繰り返す

		// マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)m_apModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_apModel[nCntModel].dwNumMat; nCntMat++)
		{ // マテリアルの数分繰り返す

			if (pMat[nCntMat].pTextureFilename != nullptr)
			{ // テクスチャファイルが存在する場合

				// テクスチャの読み込み処理
				m_apModel[nCntModel].m_nTexIdx[nCntMat] = CManager::Get()->GetTexture()->Regist(pMat[nCntMat].pTextureFilename);
			}
		}
	}

	// 成功を返す
	return S_OK;
}

//============================================================
// Xファイルの取得処理
//============================================================
CXFile::SXFile CXFile::GetXFile(TYPE type)
{
	if (type >= 0 && type < TYPE_MAX)
	{ // 種類が規定内の場合

		// モデルの情報を返す
		return m_apModel[type];
	}
	else
	{ // 種類が規定外の場合

		// 停止
		assert(false);

		// 0番目を返す
		return m_apModel[0];
	}
}