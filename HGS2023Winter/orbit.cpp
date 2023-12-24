//=================================================================================
//
//	軌跡処理 [orbit.cpp]
//	Author：小原立暉
//
//=================================================================================
//*********************************************************************************
//	インクルードファイル
//*********************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "orbit.h"
#include "texture.h"

//---------------------------------------------------------------------------------
//	マクロ定義
//---------------------------------------------------------------------------------
#define ORBIT_HEIGHT_VTX		(2)											// 軌跡の縦幅
#define ORBIT_COL				(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))			// 軌跡の色
#define ORBIT_ALPHA_SUB			(0.02f)										// 透明度の減算量

//=================================
// コンストラクタ
//=================================
COrbit::COrbit() : CObject(CObject::TYPE_ORBIT, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pVtxBuff = nullptr;							// 頂点バッファへのポインタ
	m_pTexture = nullptr;							// テクスチャへのポインタ
	ZeroMemory(&m_mtxWorld,sizeof(m_mtxWorld));		// ワールドマトリックス
	m_posParent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 親の位置
	m_rotParent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 親の向き

	for (int nCntVtx = 0; nCntVtx < MAX_ORBIT_VTX; nCntVtx++)
	{
		m_aPosPoint[nCntVtx] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 計算後の各頂点座標
		m_aColPoint[nCntVtx] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// 計算後の各頂点カラー
	}
}

//=================================
// デストラクタ
//=================================
COrbit::~COrbit()
{

}

//=================================
//	軌跡の初期化処理
//=================================
HRESULT COrbit::Init(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ
	VERTEX_3D *pVtx;							// 頂点情報へのポインタ

	// 全ての値をクリアする
	m_pVtxBuff = nullptr;							// 頂点バッファへのポインタ
	m_pTexture = nullptr;						// テクスチャへのポインタ
	ZeroMemory(&m_mtxWorld, sizeof(m_mtxWorld));	// ワールドマトリックス
	m_posParent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 親の位置
	m_rotParent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 親の向き

	for (int nCntVtx = 0; nCntVtx < MAX_ORBIT_VTX; nCntVtx++)
	{
		m_aPosPoint[nCntVtx] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 計算後の各頂点座標
		m_aColPoint[nCntVtx] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// 計算後の各頂点カラー
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Diabolo.jpg", &m_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * MAX_ORBIT_VTX,	// 必要頂点数
		D3DUSAGE_WRITEONLY,					// 使用方法
		FVF_VERTEX_3D,						// 頂点フォーマット
		D3DPOOL_MANAGED,					// メモリの指定
		&m_pVtxBuff,					// 頂点バッファへのポインタ
		NULL
	);

	//------------------------------------------------------------------------------------------------------------------
	//	頂点情報の初期化
	//------------------------------------------------------------------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWid = 0; nCntWid < (int)(MAX_ORBIT_VTX * 0.5f); nCntWid++)
	{
		for (int nCntDep = 1; nCntDep >= 0; nCntDep--)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = ORBIT_COL;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2
			(
				(float)(nCntWid * 0.05f),
				(float)((nCntDep) % 2)
			);

			pVtx++;				// 頂点データを進める
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 成功を返す
	return S_OK;
}

//=================================
//	軌跡の終了処理
//=================================
void COrbit::Uninit(void)
{
	if (m_pTexture != nullptr)
	{ // 変数 (m_pTexture) がNULLではない場合

		m_pTexture = nullptr;
	}

	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{ // 変数 (g_pVtxBuffOrbit) がNULLではない場合

		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// 破棄処理
	Release();
}

//=================================
//	軌跡の更新処理
//=================================
void COrbit::Update(void)
{
	VERTEX_3D *pVtx;				// 頂点情報へのポインタ

	// 頂点座標と頂点カラーをずらす
	for (int nCnt = MAX_ORBIT_VTX - (ORBIT_HEIGHT_VTX + 1); nCnt >= 0; nCnt--)
	{ // 頂点数分ずらす

		// 頂点座標のデータを2個分ずらす
		m_aPosPoint[nCnt + ORBIT_HEIGHT_VTX] = m_aPosPoint[nCnt];

		// 透明度を下げる
		m_aColPoint[nCnt].a -= ORBIT_ALPHA_SUB;

		// 頂点カラーのデータを2個分ずらす
		m_aColPoint[nCnt + ORBIT_HEIGHT_VTX] = m_aColPoint[nCnt];
	}

	// 最初の頂点座標と頂点カラーを代入する
	for (int nCntNew = 0; nCntNew < MATRIXPLACE_MAX; nCntNew++)
	{ // マトリックスの数繰り返す

		switch (nCntNew)
		{
		case MATRIXPLACE_BOTTOM:

			// 頂点座標を代入する
			m_aPosPoint[nCntNew].x = m_posParent.x;
			m_aPosPoint[nCntNew].y = m_posParent.y;
			m_aPosPoint[nCntNew].z = m_posParent.z;

			break;

		case MATRIXPLACE_TOP:

			// 頂点座標を代入する
			m_aPosPoint[nCntNew].x = m_posParent.x + sinf(m_rotParent.x) * (100.0f);
			m_aPosPoint[nCntNew].y = m_posParent.y + cosf(m_rotParent.x) * (100.0f);
			m_aPosPoint[nCntNew].z = m_posParent.z;

			break;
		}

		// 色を代入する
		m_aColPoint[nCntNew] = ORBIT_COL;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nNum;

	for (int nCntWid = 0; nCntWid < (int)(MAX_ORBIT_VTX * 0.5f); nCntWid++)
	{
		for (int nCntDep = 1; nCntDep >= 0; nCntDep--)
		{
			nNum = (MATRIXPLACE_MAX * nCntWid) + (1 - nCntDep);

			// 頂点座標の設定
			pVtx[nNum].pos = m_aPosPoint[nNum];

			// 色の設定
			pVtx[nNum].col = m_aColPoint[nNum];
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=================================
//	軌跡の描画処理
//=================================
void COrbit::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot;			// 計算用マトリックス

	// ライティングをOFFにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Zテストの有効/無効設定

	// カリングをOFFにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		m_pVtxBuff,									//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_3D));							//頂点情報構造体のサイズ

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,											//描画する最初の頂点インデックス
		MAX_ORBIT_VTX - 2);							//描画するプリミティブ数

	// カリングをONにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// Zテストの有効/無効設定

	//ライティングをONにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//================================
// テクスチャの割り当て処理
//================================
void COrbit::BindTexture(int nIdx)
{
	// テクスチャを割り当てる
	m_pTexture = CManager::Get()->GetTexture()->GetAddress(nIdx);
}

//=================================
// 親の位置の設定処理
//=================================
void COrbit::SetParentData(const D3DXVECTOR3& pos,const D3DXVECTOR3& rot)
{
	// 親の位置と向きを設定する
	m_posParent = pos;
	m_rotParent = rot;
}

//=================================
// 全頂点の位置の設定
//=================================
void COrbit::PosSet(const D3DXVECTOR3& pos)
{
	// 位置を設定する
	m_posParent = pos;

	for (int nCntVtx = 0; nCntVtx < MAX_ORBIT_VTX; nCntVtx++)
	{
		// 計算後の各頂点座標を設定する
		m_aPosPoint[nCntVtx] = pos;
	}
}

//=================================
// 軌跡の設定処理
//=================================
COrbit* COrbit::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3& rot,const int nTexIdx)
{
	// ローカルオブジェクトを生成
	COrbit* pOrbit = nullptr;	// プレイヤーのインスタンスを生成

	if (pOrbit == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pOrbit = new COrbit;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pOrbit != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pOrbit->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "軌跡の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 全頂点の位置の設定処理
		pOrbit->PosSet(pos);

		// 向きを設定する
		pOrbit->m_rotParent = rot;

		// テクスチャの割り当て処理
		pOrbit->BindTexture(nTexIdx);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// プレイヤーのポインタを返す
	return pOrbit;
}