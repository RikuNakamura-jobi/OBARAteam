//====================================================
//
// スカイボックス処理 [skybox.cpp]
// Author：小原立暉
//
//====================================================
//****************************************************
//	インクルードファイル
//****************************************************
#include "manager.h"
#include "skybox.h"
#include "texture.h"

//----------------------------------------------------
// マクロ定義
//----------------------------------------------------
#define TEXTURE				"data\\TEXTURE\\Platform\\NoonSky.jpg"				// テクスチャのパス
#define SKYBOX_POS			(D3DXVECTOR3(20000.0f, -10000.0f, 0.0f))	// 位置
#define SKYBOX_ROT			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))				// 向き
#define SKYBOX_WIDTH		(40000.0f)									// 横幅
#define SKYBOX_HEIGHT		(40000.0f)									// 縦幅
#define SKYBOX_DIVI_CIRCUM	(32)										// 円周の分割数
#define SKYBOX_DIVI_HEIGHT	(32)										// 縦の分割数
#define SKYBOX_ROTATION		(0.001f)									// 回す速度

//================================
// コンストラクタ
//================================
CSkyBox::CSkyBox() : CMeshDome(CObject::TYPE_SKYBOX, CObject::PRIORITY_BG)
{
	
}

//================================
// デストラクタ
//================================
CSkyBox::~CSkyBox()
{

}

//================================
// 初期化処理
//================================
HRESULT CSkyBox::Init(void)
{
	if (FAILED(CMeshDome::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//================================
// 終了処理
//================================
void CSkyBox::Uninit(void)
{
	// 終了処理
	CMeshDome::Uninit();
}

//================================
// 更新処理
//================================
void CSkyBox::Update(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = GetRot();		// 向き

	// 向きを回す
	rot.y += SKYBOX_ROTATION;

	// 向きの設定処理
	SetRot(rot);
}

//================================
// 描画処理
//================================
void CSkyBox::Draw(void)
{
	// 描画処理
	CMeshDome::Draw();
}

//================================
// 生成処理
//================================
CSkyBox* CSkyBox::Create(void)
{
	// ローカルオブジェクトを生成
	CSkyBox* pSkyBox = nullptr;	// オブジェクト3Dのインスタンスを生成

	if (pSkyBox == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pSkyBox = new CSkyBox;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pSkyBox != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// ドームの設定処理
		pSkyBox->SetDataDome(SKYBOX_POS, SKYBOX_ROT, SKYBOX_WIDTH, SKYBOX_HEIGHT, SKYBOX_DIVI_CIRCUM, SKYBOX_DIVI_HEIGHT);

		// 初期化処理
		if (FAILED(pSkyBox->Init()))
		{ // 初期化処理に失敗した場合

			// NULL を返す
			return nullptr;
		}

		// テクスチャの割り当て処理
		pSkyBox->BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));

		// ライティング状況を設定する
		pSkyBox->SetLighting(true);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// スカイボックスのポインタを返す
	return pSkyBox;
}