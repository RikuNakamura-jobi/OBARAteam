//============================================
//
// ビルボードのメイン処理[billboard.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "objectbillboard.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//----------------------------------------------------------------ここからCBillboardの処理----------------------------------------------------------------

//=========================================
// コンストラクタ
//=========================================
CBillboard::CBillboard() : CObject(CObject::TYPE_NONE, CObject::PRIORITY_BG)
{
	// コンストラクタの箱
	Box();
}

//=========================================
// オーバーロードコンストラクタ
//=========================================
CBillboard::CBillboard(CObject::TYPE type, PRIORITY priority) : CObject(type, priority)
{
	// コンストラクタの箱
	Box();
}

//=========================================
// デストラクタ
//=========================================
CBillboard::~CBillboard()
{

}

//=========================================
// コンストラクタの箱
//=========================================
void CBillboard::Box(void)
{
	// 全ての値をクリアする
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_posOld = m_pos;								// 前回の位置
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// サイズ
	m_pVtxBuff = nullptr;							// 頂点バッファのポインタ
	ZeroMemory(&m_mtxWorld, sizeof(D3DXMATRIX));	// ワールドマトリックス
	m_nTexIdx = NONE_TEXIDX;						// テクスチャのインデックス
}

//===========================================
// ビルボードの初期化処理
//===========================================
HRESULT CBillboard::Init(void)
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
	m_size = D3DXVECTOR3(1000.0f, 0.0f, 1000.0f);		// サイズ

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
// ビルボードの終了処理
//===========================================
void CBillboard::Uninit(void)
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
// ビルボードの更新処理
//===========================================
void CBillboard::Update(void)
{

}

//===========================================
// ビルボードの描画処理
//===========================================
void CBillboard::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	D3DXMATRIX mtxTrans;			//計算用マトリックス
	D3DXMATRIX mtxView;				//ビューマトリックス取得用

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Zテストの有効/無効設定

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);				//アルファテストの参照値設定

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ポリゴンをカメラに向けて正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	// 逆行列を求める
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

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

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		// アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						// アルファテストの参照値設定

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// Zテストの有効/無効設定
}

//===========================================
// 描画処理(ライティング無し)
//===========================================
void CBillboard::DrawLightOff(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	D3DXMATRIX mtxTrans;			//計算用マトリックス
	D3DXMATRIX mtxView;				//ビューマトリックス取得用

	// ライティングをOFFにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Zテストの有効/無効設定

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);				//アルファテストの参照値設定

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ポリゴンをカメラに向けて正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	// 逆行列を求める
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

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

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		// アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						// アルファテストの参照値設定

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// Zテストの有効/無効設定

	// ライティングをONにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//===========================================
// 描画処理(位置ずらし)
//===========================================
void CBillboard::DrawShift(const D3DXVECTOR3 shift)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	D3DXMATRIX mtxTrans;			//計算用マトリックス
	D3DXMATRIX mtxView;				//ビューマトリックス取得用

	// ライティングをOFFにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Zテストの有効/無効設定

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);				//アルファテストの参照値設定

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ポリゴンをカメラに向けて正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	// 逆行列を求める
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x + shift.x, m_pos.y + shift.y, m_pos.z + shift.z);
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

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		// アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						// アルファテストの参照値設定

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// Zテストの有効/無効設定

	// ライティングをONにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//===========================================
// 頂点情報の設定
//===========================================
void CBillboard::SetVertex(void)
{
	VERTEX_3D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, +m_size.y, +m_size.z);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, +m_size.y, +m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, -m_size.y, -m_size.z);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点情報の設定(下頂点)
//===========================================
void CBillboard::SetVtxPosUnder(void)
{
	VERTEX_3D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, (m_size.y * 2), (m_size.z * 2));
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, (m_size.y * 2), (m_size.z * 2));
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, 0.0f, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// アニメーションの設定
//===========================================
void CBillboard::SetVtxAnim(const float fTex, const int nPattern)
{
	VERTEX_3D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(nPattern * fTex, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(nPattern * fTex + fTex, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nPattern * fTex, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(nPattern * fTex + fTex, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// アニメーションの設定(反転)
//===========================================
void CBillboard::SetVtxAnimRev(const float fTex, const int nPattern)
{
	VERTEX_3D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(nPattern * fTex + fTex, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(nPattern * fTex, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nPattern * fTex + fTex, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(nPattern * fTex, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// 頂点カラーの設定
//===========================================
void CBillboard::SetVtxColor(D3DXCOLOR col)
{
	VERTEX_3D * pVtx;											//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================
// テクスチャの割り当て処理
//===========================================
void CBillboard::BindTexture(int nIdx)
{
	// テクスチャを割り当てる
	m_nTexIdx = nIdx;
}

//===========================================
// 位置設定処理
//===========================================
void CBillboard::SetPos(const D3DXVECTOR3& pos)
{
	// 位置を設定する
	m_pos = pos;
}

//===========================================
// 位置取得処理
//===========================================
D3DXVECTOR3 CBillboard::GetPos(void) const
{
	// 位置を返す
	return m_pos;
}

//===========================================
// 前回の位置設定処理
//===========================================
void CBillboard::SetPosOld(const D3DXVECTOR3& posOld)
{
	// 前回の位置を設定する
	m_posOld = posOld;
}

//===========================================
// 前回の位置取得処理
//===========================================
D3DXVECTOR3 CBillboard::GetPosOld(void) const
{
	// 前回の位置を返す
	return m_posOld;
}

//===========================================
// サイズ設定処理
//===========================================
void CBillboard::SetSize(const D3DXVECTOR3& size)
{
	// サイズを設定する
	m_size = size;
}

//===========================================
// サイズ取得処理
//===========================================
D3DXVECTOR3 CBillboard::GetSize(void) const
{
	// サイズを返す
	return m_size;
}

//===========================================
// テクスチャのインデックスの取得処理
//===========================================
int CBillboard::GetTexIdx(void) const
{
	// テクスチャのインデックスを返す
	return m_nTexIdx;
}

//===========================================
// 生成処理
//===========================================
CBillboard* CBillboard::Create(const TYPE type, const PRIORITY priority)
{
	// ローカルオブジェクトを生成
	CBillboard* pBillboard = nullptr;	// ビルボードのインスタンスを生成

	if (pBillboard == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pBillboard = new CBillboard(type, priority);
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pBillboard != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pBillboard->Init()))
		{ // 失敗した場合

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

	// ビルボードのポインタを返す
	return pBillboard;
}

//----------------------------------------------------------------ここからCBillboardAnimの処理----------------------------------------------------------------

//===========================================
// コンストラクタ
//===========================================
CBillboardAnim::CBillboardAnim() : CBillboard(CObject::TYPE_NONE, CObject::PRIORITY_BG)
{
	// コンストラクタの箱
	Box();
}

//===========================================
// オーバーロードコンストラクタ
//===========================================
CBillboardAnim::CBillboardAnim(CObject::TYPE type, PRIORITY priority) : CBillboard(type, priority)
{
	// コンストラクタの箱
	Box();
}

//===========================================
// デストラクタ
//===========================================
CBillboardAnim::~CBillboardAnim()
{

}

//===========================================
// コンストラクタの箱
//===========================================
void CBillboardAnim::Box(void)
{
	// 全ての値をクリアする
	m_nCounter = 0;					// アニメーションカウンター
	m_nReplayCount = 0;				// 再生カウント
	m_nPattern = 0;					// アニメーションパターン
	m_nReplayPattern = 0;			// 再生パターン
	m_fTexPattern = 0.0f;			// テクスチャのパターン
}

//===========================================
// 初期化処理
//===========================================
HRESULT CBillboardAnim::Init(void)
{
	// 初期化
	if (FAILED(CBillboard::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_nCounter = 0;				// アニメーションカウンター
	m_nPattern = 0;				// アニメーションパターン
	m_nReplayCount = 0;				// 再生カウント
	m_nReplayPattern = 0;			// 再生パターン
	m_fTexPattern = 0.0f;			// テクスチャの再生パターン

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CBillboardAnim::Uninit(void)
{
	// 終了処理
	CBillboard::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CBillboardAnim::Update(void)
{
	// カウントを加算する
	m_nReplayCount++;

	if (m_nReplayCount % m_nCounter == 0)
	{ // カウントが規定値に達した場合

		// 再生カウントを0にする
		m_nReplayCount = 0;

		// パターン数を移動する
		m_nReplayPattern = (m_nReplayPattern + 1) % m_nPattern;
	}

	// テクスチャの設定処理(アニメーションバージョン)
	SetVtxAnim(m_fTexPattern, m_nReplayPattern);
}

//===========================================
// 描画処理
//===========================================
void CBillboardAnim::Draw(void)
{
	// 描画処理
	CBillboard::Draw();
}

//===========================================
// 描画処理
//===========================================
void CBillboardAnim::DrawLightOff(void)
{
	// 描画処理
	CBillboard::DrawLightOff();
}

//===========================================
// パターンの取得処理
//===========================================
int CBillboardAnim::GetPattern(void) const
{
	// 現在のパターンを返す
	return m_nReplayPattern;
}

//===========================================
// テクスチャのパターンの取得処理
//===========================================
float CBillboardAnim::GetTexPattern(void) const
{
	// テクスチャのパターンを返す
	return m_fTexPattern;
}

//===========================================
// アニメーションの設定処理
//===========================================
void CBillboardAnim::SetAnim(const int nCount, const int nPattern)
{
	// 情報を設定する
	m_nCounter = nCount;			// アニメーションカウンター
	m_nPattern = nPattern;			// アニメーションパターン
	m_fTexPattern = 1.0f / nPattern;	// テクスチャの再生パターン
}

//===========================================
// 生成処理
//===========================================
CBillboardAnim* CBillboardAnim::Create(void)
{
	// ローカルオブジェクトを生成
	CBillboardAnim* pBillboard = nullptr;	// ビルボードのインスタンスを生成

	if (pBillboard == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pBillboard = new CBillboardAnim;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pBillboard != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pBillboard->Init()))
		{ // 初期化処理に失敗した場合

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

	// ビルボードのポインタを返す
	return pBillboard;
}