//===========================================
//
// ライトのメイン処理[light.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"

#include "input.h"
#include "debugproc.h"

// マクロ定義
#define ONE_LIGHT_VEC		(D3DXVECTOR3(0.22f, -0.87f, 0.22f))		// 一つ目のライトの方向
#define TWO_LIGHT_VEC		(D3DXVECTOR3(-0.9f, -0.2f, 0.2f))		// 二つ目のライトの方向
#define THREE_LIGHT_VEC		(D3DXVECTOR3(0.55f, 0.3f, 0.88f))		// 三つ目のライトの方向

//=========================
// コンストラクタ
//=========================
CLight::CLight()
{
	for (int nCntSet = 0; nCntSet < NUM_LIGHT; nCntSet++)
	{
		// ライトの情報をクリアする
		ZeroMemory(&m_light[nCntSet], sizeof(D3DLIGHT9));
	}
}

//=========================
// デストラクタ
//=========================
CLight::~CLight()
{

}

//=========================
// ライトの初期化処理
//=========================
HRESULT CLight::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir;				// 設定用方向ベクトル

	for (int nCntSet = 0; nCntSet < NUM_LIGHT; nCntSet++)
	{
		// ライトの情報をクリアする
		ZeroMemory(&m_light[nCntSet], sizeof(D3DLIGHT9));

		// ライトの種類を設定
		m_light[nCntSet].Type = D3DLIGHT_DIRECTIONAL;

		switch (nCntSet)
		{
		case 0:

			// ライトの拡散光を設定
			m_light[nCntSet].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ライトの方向を設定
			vecDir = ONE_LIGHT_VEC;

			break;				// 抜け出す

		case 1:

			// ライトの拡散光を設定
			m_light[nCntSet].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

			// ライトの方向を設定
			vecDir = TWO_LIGHT_VEC;

			break;				// 抜け出す

		case 2:

			// ライトの拡散光を設定
			m_light[nCntSet].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

			// ライトの方向を設定
			vecDir = THREE_LIGHT_VEC;

			break;				// 抜け出す

		default:

			// 停止
			assert(false);

			break;
		}

		D3DXVec3Normalize(&vecDir, &vecDir);				// ベクトルを正規化する

		m_light[nCntSet].Direction = vecDir;

		// ライトを設定する
		pDevice->SetLight(nCntSet, &m_light[nCntSet]);

		// ライトを有効にする
		pDevice->LightEnable(nCntSet, TRUE);
	}

	// 成功を返す
	return S_OK;
}

//=========================
// ライトの終了処理
//=========================
void CLight::Uninit(void)
{

}

//=========================
// ライトの更新処理
//=========================
void CLight::Update(void)
{

}