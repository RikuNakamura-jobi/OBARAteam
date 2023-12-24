//===========================================
//
// モデル影のメイン処理[shadowModel.cpp]
// Author 小原立暉
//
//===========================================
#include "main.h"
#include "shadowModel.h"
#include "light.h"
#include "manager.h"
#include "renderer.h"

//====================
// コンストラクタ
//====================
CShadowModel::CShadowModel() : CModel(TYPE_SHADOW, PRIORITY_SHADOW)
{
	// 全ての値をクリアする
	ZeroMemory(&m_mtxParent, sizeof(D3DXMATRIX));
}

//====================
// デストラクタ
//====================
CShadowModel::~CShadowModel()
{

}

//====================
// 初期化処理
//====================
HRESULT CShadowModel::Init(void)
{
	// 初期化処理
	if (FAILED(CModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	ZeroMemory(&m_mtxParent, sizeof(D3DXMATRIX));

	// 成功を返す
	return S_OK;
}

//====================
// 終了処理
//====================
void CShadowModel::Uninit(void)
{
	// オブジェクトの終了処理
	CModel::Uninit();
}

//====================
// 更新処理
//====================
void CShadowModel::Update(void)
{

}

//====================
// 描画処理
//====================
void CShadowModel::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// ローカル変数宣言
	D3DXMATRIX mtxShadow;		// 影のマトリックス
	D3DLIGHT9 light;			// ライトの情報
	D3DXVECTOR4 posLight;		// ライトの位置
	D3DXVECTOR3 pos, normal;	// 平面上の任意の点、法線ベクトル
	D3DXPLANE plane;			// 平面情報

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Zバッファ更新の有効 / 無効の設定

	// ライトの位置を設定
	pDevice->GetLight(0, &light);
	posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

	// 平面情報を生成
	pos = GetPos();
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	// 影のマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	// 影のマトリックスの作成
	D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
	D3DXMatrixMultiply(&mtxShadow, &m_mtxParent, &mtxShadow);

	// 影のマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	// 影専用描画処理
	CModel::DrawShadow();

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Zバッファ更新の有効 / 無効の設定
}

//====================
// 情報の設定処理
//====================
void CShadowModel::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const CXFile::TYPE& type, const D3DXMATRIX& mtx)
{
	// 情報の設定
	SetPos(pos);			// 位置
	SetPosOld(pos);			// 前回の位置
	SetRot(rot);			// 向き
	SetScale(scale);		// 拡大率
	SetFileData(type);		// 種類
	m_mtxParent = mtx;		// 親のマトリックス

	// Xファイルの情報を取得する
	CXFile::SXFile file = GetFileData();

	// 最大値を反映する
	file.vtxMax.x *= scale.x;
	file.vtxMax.y *= scale.y;
	file.vtxMax.z *= scale.z;

	// 最小値を反映する
	file.vtxMin.x *= scale.x;
	file.vtxMin.y *= scale.y;
	file.vtxMin.z *= scale.z;

	// データの設定処理
	SetFileData(file);
}

//====================
// 影の位置設定処理
//====================
void CShadowModel::SetPosRot(int nIdx, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ローカル変数宣言
	CObject* pObj = nullptr;			// オブジェクトへのポインタ
	CShadowModel* pShadow = nullptr;	// 影へのポインタ

	// オブジェクトの情報を取得する
	pObj = GetTop(PRIORITY_SHADOW);

	for (int nCnt = 0; nCnt < nIdx; nCnt++)
	{
		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}

	if (pObj != nullptr)
	{ // ポインタが NULL の場合

		if (pObj->GetType() == TYPE_SHADOW)
		{ // 種類が影の場合

			// 影の情報を取得する
			pShadow = dynamic_cast<CShadowModel*>(pObj);

			if (pShadow != nullptr)
			{ // ポインタが NULL じゃない場合

				// 位置の設定処理
				pShadow->SetPos(pos);

				// 向きの設定処理
				pShadow->SetRot(rot);
			}
		}
	}
}

//====================
// 生成処理
//====================
CShadowModel* CShadowModel::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const CXFile::TYPE& type, const D3DXMATRIX& mtx)
{
	// ローカルオブジェクトを生成
	CShadowModel* pShadow = nullptr;	// 影のインスタンスを生成

	if (pShadow == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pShadow = new CShadowModel;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pShadow != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pShadow->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pShadow->SetData(pos, rot, scale, type, mtx);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 影のポインタを返す
	return pShadow;
}