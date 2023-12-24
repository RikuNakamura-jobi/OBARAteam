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
#include "character.h"
#include "model.h"
#include "texture.h"

//=====================================================================ここからCHierarchyの処理=====================================================================

//========================
// コンストラクタ
//========================
CHierarchy::CHierarchy()
{
	// 全ての値をクリアする
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 前回の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_scale = NONE_SCALE;								// 拡大率
	ZeroMemory(&m_mtxWorld, sizeof(D3DXMATRIX));		// ワールドマトリックス
	ZeroMemory(&m_XFileData, sizeof(CXFile::SXFile));	// Xファイルのデータ
	m_apParent = nullptr;								// 親モデルのポインタ
}

//========================
// デストラクタ
//========================
CHierarchy::~CHierarchy()
{

}

//========================
// 初期化処理
//========================
HRESULT CHierarchy::Init(void)
{
	// 全ての値を初期化する
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_posOld = m_pos;			// 前回の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_scale = NONE_SCALE;		// 拡大率

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CHierarchy::Uninit(void)
{
	// 親モデルへのポインタを NULL にする
	m_apParent = nullptr;
}

//========================
// 更新処理
//========================
void CHierarchy::Update(void)
{

}

//========================
// 描画処理
//========================
void CHierarchy::Draw(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ

	D3DXMATRIX mtxRotModel, mtxTransModel;		// 計算用マトリックス
	D3DXMATRIX mtxParent;						// 親のマトリックス

	// パーツのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// パーツの「親のマトリックス」を設定
	if (m_apParent != nullptr)
	{ // 親モデルがある場合

		// 親モデルのインデックスを指定する
		mtxParent = m_apParent->m_mtxWorld;
	}
	else
	{ // 親モデルがない場合

		// 親のマトリックスを取得する
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// 算出した「パーツのワールドマトリックス」と「親のマトリックス」を掛け合わせる
	D3DXMatrixMultiply
	(
		&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent
	);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_XFileData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_XFileData.dwNumMat; nCntMat++)
	{ // マテリアルの数分繰り返す

		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_XFileData.m_nTexIdx[nCntMat]));

		if (m_scale != NONE_SCALE)
		{ // 拡大率が変更されている場合

			// 頂点法線の自動正規化を有効にする
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// モデルの描画
		m_XFileData.pMesh->DrawSubset(nCntMat);

		// 頂点法線の自動正規化を無効にする
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
// 描画処理(色処理)
//========================
void CHierarchy::Draw(D3DXCOLOR col)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ
	D3DXMATERIAL     colMat;					// 描画用マテリアル

	D3DXMATRIX mtxRotModel, mtxTransModel;		// 計算用マトリックス
	D3DXMATRIX mtxParent;						// 親のマトリックス

	// パーツのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// パーツの「親のマトリックス」を設定
	if (m_apParent != nullptr)
	{ // 親モデルがある場合

		// 親モデルのインデックスを指定する
		mtxParent = m_apParent->m_mtxWorld;
	}
	else
	{ // 親モデルがない場合

		// 親のマトリックスを取得する
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// 算出した「パーツのワールドマトリックス」と「親のマトリックス」を掛け合わせる
	D3DXMatrixMultiply
	(
		&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent
	);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_XFileData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_XFileData.dwNumMat; nCntMat++)
	{ // マテリアルの数分繰り返す

		// 構造体の要素をクリア
		ZeroMemory(&colMat, sizeof(D3DXMATERIAL));

		// マテリアルデータのコピーに代入する
		colMat = pMat[nCntMat];

		// 透明度を代入する
		colMat.MatD3D.Diffuse = col;
		colMat.MatD3D.Ambient = col;
		colMat.MatD3D.Emissive = col;

		// マテリアルの設定
		pDevice->SetMaterial(&colMat.MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_XFileData.m_nTexIdx[nCntMat]));

		if (m_scale != NONE_SCALE)
		{ // 拡大率が変更されている場合

			// 頂点法線の自動正規化を有効にする
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// モデルの描画
		m_XFileData.pMesh->DrawSubset(nCntMat);

		// 頂点法線の自動正規化を無効にする
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
// モデル影の描画処理
//========================
void CHierarchy::DrawShadow(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ
	D3DXMATERIAL     colMat;					// 描画用マテリアル

	D3DXMATRIX mtxRotModel, mtxTransModel;		// 計算用マトリックス
	D3DXMATRIX mtxParent;						// 親のマトリックス

	// パーツのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// パーツの「親のマトリックス」を設定
	if (m_apParent != nullptr)
	{ // 親モデルがある場合

		// 親モデルのインデックスを指定する
		mtxParent = m_apParent->m_mtxWorld;
	}
	else
	{ // 親モデルがない場合

		// 親のマトリックスを取得する
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// 算出した「パーツのワールドマトリックス」と「親のマトリックス」を掛け合わせる
	D3DXMatrixMultiply
	(
		&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent
	);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_XFileData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_XFileData.dwNumMat; nCntMat++)
	{ // マテリアルの数分繰り返す

		// 構造体の要素をクリア
		ZeroMemory(&colMat, sizeof(D3DXMATERIAL));

		// 透明度を代入する
		colMat.MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		colMat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		colMat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// マテリアルの設定
		pDevice->SetMaterial(&colMat.MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		if (m_scale != NONE_SCALE)
		{ // 拡大率が変更されている場合

			// 頂点法線の自動正規化を有効にする
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// モデルの描画
		m_XFileData.pMesh->DrawSubset(nCntMat);

		// 頂点法線の自動正規化を無効にする
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
// 描画処理(透明度処理)
//========================
void CHierarchy::Draw(float fAlpha)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ
	D3DXMATERIAL     *pMat;						// マテリアルデータへのポインタ
	D3DXMATERIAL     colMat;					// 描画用マテリアル

	D3DXMATRIX mtxRotModel, mtxTransModel;		// 計算用マトリックス
	D3DXMATRIX mtxParent;						// 親のマトリックス

	// パーツのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// パーツの「親のマトリックス」を設定
	if (m_apParent != nullptr)
	{ // 親モデルがある場合

		// 親モデルのインデックスを指定する
		mtxParent = m_apParent->m_mtxWorld;
	}
	else
	{ // 親モデルがない場合

		// 親のマトリックスを取得する
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// 算出した「パーツのワールドマトリックス」と「親のマトリックス」を掛け合わせる
	D3DXMatrixMultiply
	(
		&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent
	);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_XFileData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_XFileData.dwNumMat; nCntMat++)
	{ // マテリアルの数分繰り返す

		// 構造体の要素をクリア
		ZeroMemory(&colMat, sizeof(D3DXMATERIAL));

		// 透明度を代入する
		colMat.MatD3D.Diffuse = pMat[nCntMat].MatD3D.Diffuse;
		colMat.MatD3D.Ambient = pMat[nCntMat].MatD3D.Ambient;

		// マテリアルの透明度を設定する
		colMat.MatD3D.Diffuse.a = fAlpha;
		colMat.MatD3D.Ambient.a = fAlpha;

		// マテリアルの設定
		pDevice->SetMaterial(&colMat.MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, CManager::Get()->GetTexture()->GetAddress(m_XFileData.m_nTexIdx[nCntMat]));

		if (m_scale != NONE_SCALE)
		{ // 拡大率が変更されている場合

		  // 頂点法線の自動正規化を有効にする
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// モデルの描画
		m_XFileData.pMesh->DrawSubset(nCntMat);

		// 頂点法線の自動正規化を無効にする
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
// Xファイルのデータの取得処理
//========================
CXFile::SXFile CHierarchy::GetFileData(void)
{
	// Xファイルのデータを返す
	return m_XFileData;
}

//========================
// 位置設定処理
//========================
void CHierarchy::SetPos(const D3DXVECTOR3& pos)
{
	// 位置を設定する
	m_pos = pos;
}

//========================
// 位置取得処理
//========================
D3DXVECTOR3 CHierarchy::GetPos(void) const
{
	// 位置を返す
	return m_pos;
}

//========================
// 前回の位置設定処理
//========================
void CHierarchy::SetPosOld(const D3DXVECTOR3& posOld)
{
	// 前回の位置を設定する
	m_posOld = posOld;
}

//========================
// 前回の位置取得処理
//========================
D3DXVECTOR3 CHierarchy::GetPosOld(void) const
{
	// 前回の位置を返す
	return m_posOld;
}

//========================
// 向き設定処理
//========================
void CHierarchy::SetRot(const D3DXVECTOR3& rot)
{
	// 向きを設定する
	m_rot = rot;
}

//========================
// 向き取得処理
//========================
D3DXVECTOR3 CHierarchy::GetRot(void) const
{
	// 向きを設定する
	return m_rot;
}

//========================
// サイズ設定処理
//========================
void CHierarchy::SetScale(const D3DXVECTOR3& scale)
{
	// 拡大率を設定する
	m_scale = scale;
}

//========================
// サイズ取得処理
//========================
D3DXVECTOR3 CHierarchy::GetScale(void) const
{
	// 拡大率を返す
	return m_scale;
}

//========================
// データの設定処理
//========================
void CHierarchy::SetFileData(const CXFile::TYPE type)
{
	if (type >= 0 && type < CXFile::TYPE_MAX)
	{ // 種類が規定内の場合

		// ファイルのデータを代入する
		m_XFileData = CXFile::GetXFile(type);
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}
}

//========================
// データの設定処理(ファイルデータの設定版)
//========================
void CHierarchy::SetFileData(const CXFile::SXFile filaData)
{
	// ファイルのデータを代入する
	m_XFileData = filaData;
}

//========================
// マトリックスの取得処理
//========================
D3DXMATRIX CHierarchy::GetMatrix(void) const
{
	// ワールドマトリックスを返す
	return m_mtxWorld;
}

//========================
// 親の設定処理
//========================
void CHierarchy::SetParent(CHierarchy* pModel)
{
	// 親のポインタを代入する
	m_apParent = pModel;
}

//========================
// 親の取得処理
//========================
CHierarchy* CHierarchy::GetParent(void) const
{
	if (m_apParent != nullptr)
	{ // 親モデルの情報があった場合

		// 親モデルの情報を返す
		return m_apParent;
	}
	else
	{ // 親モデルの情報がない場合

		// NULL を返す
		return nullptr;
	}
}

//========================
// 生成処理
//========================
CHierarchy* CHierarchy::Create(void)
{
	// ローカルオブジェクトを生成
	CHierarchy* pModel = nullptr;	// プレイヤーのインスタンスを生成

	if (pModel == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pModel = new CHierarchy;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pModel != nullptr)
	{ // オブジェクトが NULL じゃない場合

	  // 初期化処理
		if (FAILED(pModel->Init()))
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
	return pModel;
}

//=====================================================================ここからCCharacterの処理=====================================================================

//========================
// コンストラクタ
//========================
CCharacter::CCharacter() : CObject(TYPE_NONE, PRIORITY_BG)
{
	// コンストラクタの箱
	Box();
}

//========================
// オーバーロードコンストラクタ
//========================
CCharacter::CCharacter(CObject::TYPE type, PRIORITY priority) : CObject(type, priority)
{
	// コンストラクタの箱
	Box();
}

//========================
// デストラクタ
//========================
CCharacter::~CCharacter()
{

}

//========================
// コンストラクタの箱
//========================
void CCharacter::Box(void)
{
	// 全ての値をクリアする
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 前回の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_scale = NONE_SCALE;								// 拡大率
	ZeroMemory(&m_mtxWorld, sizeof(D3DXMATRIX));		// ワールドマトリックス
	
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		m_apModel[nCnt] = nullptr;						// モデルのポインタ
	}

	m_nNumModel = 0;									// モデルの総数
}

//========================
// 初期化処理
//========================
HRESULT CCharacter::Init(void)
{
	// 全ての値を初期化する
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_posOld = m_pos;									// 前回の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_scale = NONE_SCALE;								// 拡大率

	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		m_apModel[nCnt] = nullptr;						// モデルのポインタ
	}

	m_nNumModel = 0;									// モデルの総数

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CCharacter::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{ // モデルのポインタが NULL じゃない場合

			// 終了処理
			m_apModel[nCnt]->Uninit();

			// メモリを開放する
			delete m_apModel[nCnt];
			m_apModel[nCnt] = nullptr;
		}
	}

	// 破棄処理
	Release();
}

//========================
// 更新処理
//========================
void CCharacter::Update(void)
{

}

//========================
// 描画処理
//========================
void CCharacter::Draw(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxWorld = GetMatrix();			// マトリックスを取得する

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, GetScale().x, GetScale().y, GetScale().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y + (D3DX_PI), GetRot().x, GetRot().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		// 描画処理
		m_apModel[nCnt]->Draw();
	}
}

//========================
// 描画処理(色処理)
//========================
void CCharacter::Draw(D3DXCOLOR col)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxWorld = GetMatrix();			// マトリックスを取得する

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, GetScale().x, GetScale().y, GetScale().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y + (D3DX_PI), GetRot().x, GetRot().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		// 描画処理
		m_apModel[nCnt]->Draw(col);
	}
}

//========================
// モデル影の描画処理
//========================
void CCharacter::DrawShadow(void)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxWorld = GetMatrix();			// マトリックスを取得する

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, GetScale().x, GetScale().y, GetScale().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y + (D3DX_PI), GetRot().x, GetRot().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		// 描画処理
		m_apModel[nCnt]->DrawShadow();
	}
}

//========================
// 描画処理(複数色)
//========================
void CCharacter::Draw(D3DXCOLOR* col)
{
	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxWorld = GetMatrix();			// マトリックスを取得する

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, GetScale().x, GetScale().y, GetScale().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y + (D3DX_PI), GetRot().x, GetRot().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		// 描画処理
		m_apModel[nCnt]->Draw(*col);
	}
}

//========================
// 描画処理(透明度処理)
//========================
void CCharacter::Draw(float fAlpha)
{
		// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxWorld = GetMatrix();			// マトリックスを取得する

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();	// デバイスへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, GetScale().x, GetScale().y, GetScale().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y + (D3DX_PI), GetRot().x, GetRot().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		// 描画処理
		m_apModel[nCnt]->Draw(fAlpha);
	}
}

//========================
// 位置設定処理
//========================
void CCharacter::SetPos(const D3DXVECTOR3& pos)
{
	// 位置を設定する
	m_pos = pos;
}

//========================
// 位置取得処理
//========================
D3DXVECTOR3 CCharacter::GetPos(void) const
{
	// 位置を返す
	return m_pos;
}

//========================
// 前回の位置設定処理
//========================
void CCharacter::SetPosOld(const D3DXVECTOR3& posOld)
{
	// 前回の位置を設定する
	m_posOld = posOld;
}

//========================
// 前回の位置取得処理
//========================
D3DXVECTOR3 CCharacter::GetPosOld(void) const
{
	// 前回の位置を返す
	return m_posOld;
}

//========================
// 向き設定処理
//========================
void CCharacter::SetRot(const D3DXVECTOR3& rot)
{
	// 向きを設定する
	m_rot = rot;
}

//========================
// 向き取得処理
//========================
D3DXVECTOR3 CCharacter::GetRot(void) const
{
	// 向きを設定する
	return m_rot;
}

//========================
// サイズ設定処理
//========================
void CCharacter::SetScale(const D3DXVECTOR3& scale)
{
	// 拡大率を設定する
	m_scale = scale;
}

//========================
// サイズ取得処理
//========================
D3DXVECTOR3 CCharacter::GetScale(void) const
{
	// 拡大率を返す
	return m_scale;
}

//========================
// マトリックスの取得処理
//========================
D3DXMATRIX CCharacter::GetMatrix(void) const
{
	// マトリックスの情報を返す
	return m_mtxWorld;
}

//========================
// マトリックスの取得処理
//========================
D3DXMATRIX* CCharacter::GetMatrixP(void)
{
	// マトリックスの情報を返す
	return &m_mtxWorld;
}

//========================
// データの設定処理
//========================
void CCharacter::SetData(void)
{
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		// パーツへのポインタ
		m_apModel[nCnt] = CHierarchy::Create();
	}
}

//========================
// 階層モデルの取得処理
//========================
CHierarchy* CCharacter::GetHierarchy(int nIdx)
{
	if (m_apModel[nIdx] != nullptr)
	{ // モデルの情報があった場合

		// モデルの情報を返す
		return m_apModel[nIdx];
	}
	else
	{ // 上記以外

		// 停止
		assert(false);

		// nullptr を返す
		return nullptr;
	}
}

//========================
// 階層モデルのダブルポインタの取得処理
//========================
CHierarchy** CCharacter::GetHierarchy(void)
{
	// モデルのダブルポインタを返す
	return m_apModel;
}

//========================
// モデルの総数の設定処理
//========================
void CCharacter::SetNumModel(const int nNum)
{
	// モデルの総数を設定する
	m_nNumModel = nNum;
}

//========================
// モデルの総数の取得処理
//========================
int CCharacter::GetNumModel(void) const
{
	// モデルの総数を返す
	return m_nNumModel;
}

//========================
// 生成処理
//========================
CCharacter* CCharacter::Create(void)
{
	// ローカルオブジェクトを生成
	CCharacter* pModel = nullptr;	// プレイヤーのインスタンスを生成

	if (pModel == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pModel = new CCharacter(TYPE_CHARA, PRIORITY_BG);
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

	  // NULL を返す
		return nullptr;
	}

	if (pModel != nullptr)
	{ // オブジェクトが NULL じゃない場合

	  // 初期化処理
		if (FAILED(pModel->Init()))
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

	// モデルのポインタを返す
	return pModel;
}