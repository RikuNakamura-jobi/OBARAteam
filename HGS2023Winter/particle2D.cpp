//=======================================
//
// パーティクル2Dのメイン処理[Particle2D.cpp]
// Author 小原立暉
//
//=======================================
#include "game.h"
#include "particle2D.h"

//---------------------------------------
// コンストラクタ
//---------------------------------------
CParticle2D::CParticle2D()
{

}

//---------------------------------------
// デストラクタ
//---------------------------------------
CParticle2D::~CParticle2D()
{

}

//===============================
//パーティクルの初期化処理
//===============================
HRESULT CParticle2D::Init(void)
{
	// 成功を返す
	return S_OK;
}

//===============================
//パーティクルの終了処理
//===============================
void CParticle2D::Uninit(void)
{

}

//===============================
//パーティクルの更新処理
//===============================
void CParticle2D::Update(void)
{
	
}

//===============================
//パーティクルの描画処理
//===============================
void CParticle2D::Draw(void)
{
	
}

//===============================
// 生成処理
//===============================
void CParticle2D::Create(const D3DXVECTOR3& pos, const CEffect2D::TYPE type)
{
	// ローカルポインタ宣言
	CParticle2D* pParticle = nullptr;		// パーティクル2Dのポインタ

	if (pParticle == nullptr)
	{ // パーティクルが NULL の場合

		// メモリを確保する
		pParticle = new CParticle2D;
	}

	if (pParticle != nullptr)
	{ // 情報があった場合

		switch (type)
		{
		case CEffect2D::TYPE_NONE:			// 無し

			break;

		case CEffect2D::TYPE_SHINING:		// 輝き系

			//// 輝き処理
			//InitShining(pos, type);

			break;

		case CEffect2D::TYPE_OVERTOP:		// 限界突破

			// ブーストの初期化
			pParticle->InitBoost(pos, type);

			break;

		case CEffect2D::TYPE_INFIRE:		// 内側の炎

			// ブーストの初期化
			pParticle->InitBoost(pos, type);

			break;

		case CEffect2D::TYPE_OUTFIRE:		// 外側の炎

			// ブーストの初期化
			pParticle->InitBoost(pos, type);

			break;

		default:			// 上記以外

			// 停止
			assert(false);

			break;
		}
	}
	else
	{ // 上記以外

		// 停止
		assert(false);
	}

	if (pParticle != nullptr)
	{ // パーティクルが NULL じゃない場合

		// メモリを開放する
		delete pParticle;
		pParticle = nullptr;
	}
}

//===============================
// ブーストの初期化
//===============================
void CParticle2D::InitBoost(const D3DXVECTOR3& pos, const CEffect2D::TYPE type)
{
	D3DXVECTOR3 move;			// 移動量
	float fRot;					// 向き
	int nLife;					// 寿命
	float fRadius;				// 半径

	fRot = (float)((rand() % 31 - 15) * 0.01f);

	// 移動量を設定する
	move.x = (float)(rand() % 11 - 5) * 0.1f;
	move.y = cosf(fRot) * (float)-(rand() % 40 + 20) * 0.1f;
	move.z = 0.0f;

	nLife = 40;

	fRadius = (float)(rand() % 20 + 60);

	// エフェクトの生成処理
	CEffect2D::Create(pos, move, nLife, fRadius, CEffect2D::TYPE_OUTFIRE);

	//for (int nCnt = 0; nCnt < 5; nCnt++)
	//{
	fRot = (float)((rand() % 31 - 15) * 0.01f);

	// 移動量を設定する
	move.x = (float)(rand() % 11 - 5) * 0.1f;
	move.y = cosf(fRot) * (float)-(rand() % 40 + 20) * 0.1f;
	move.z = 0.0f;

	nLife = 40;

	fRadius = (float)(rand() % 10 + 40);

	// エフェクトの生成処理
	CEffect2D::Create(pos, move, nLife, fRadius, CEffect2D::TYPE_INFIRE);
	//}
}