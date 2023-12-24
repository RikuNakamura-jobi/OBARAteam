//=======================================
//
//パーティクルのメイン処理[Particle.cpp]
//Author 小原立暉
//
//=======================================
#include "Particle.h"

//---------------------------------------
// コンストラクタ
//---------------------------------------
CParticle::CParticle()
{

}

//---------------------------------------
// デストラクタ
//---------------------------------------
CParticle::~CParticle()
{

}

//===============================
//パーティクルの初期化処理
//===============================
HRESULT CParticle::Init(void)
{
	// 成功を返す
	return S_OK;
}

//===============================
//パーティクルの終了処理
//===============================
void CParticle::Uninit(void)
{

}

//===============================
//パーティクルの更新処理
//===============================
void CParticle::Update(void)
{
	
}

//===============================
//パーティクルの描画処理
//===============================
void CParticle::Draw(void)
{
	
}

//===============================
// 生成処理
//===============================
void CParticle::Create(const D3DXVECTOR3& pos, const CParticle::TYPE type)
{
	// ローカルポインタ宣言
	CParticle* pParticle = nullptr;		// パーティクル2Dのポインタ

	if (pParticle == nullptr)
	{ // パーティクルが NULL の場合

		// メモリを確保する
		pParticle = new CParticle;
	}

	if (pParticle != nullptr)
	{ // 情報が NULL じゃない場合

		switch (type)
		{
		case TYPE_FIRE:			// 炎系

			// 爆発の初期化処理
			pParticle->InitFire(pos);

			break;

		case TYPE_DUST:			// 埃系

			// 埃の初期化処理
			pParticle->InitDust(pos);

			break;

		case TYPE_ITEMGET:		// アイテム取得

			// アイテムゲットの初期化処理
			pParticle->InitItemGet(pos);

			break;

		case TYPE_ENEMYDEATH:	// 敵の死亡

			// 敵の死亡の初期化処理
			pParticle->InitEnemyDeath(pos);

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
// 爆発パーティクル処理
//===============================
void CParticle::InitFire(const D3DXVECTOR3& pos)
{
	//[引数用変数]
	D3DXVECTOR3 ShiftPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	int nLife = 0;											// 体力
	float fAngle = 0.0f;									// 方向
	float fSpeed = 0.0f;									// 速度
	float fRadius = 0.0f;									// 半径

	for (int nCntPar = 0; nCntPar < 20; nCntPar++)
	{
		// 位置の設定
		ShiftPos.x = pos.x + (float)((rand() % 101 - 50) / 10.0f);
		ShiftPos.y = pos.y + (float)((rand() % 101 - 50) / 10.0f);
		ShiftPos.z = 0.0f;

		//角度を設定する
		fAngle = (float)(rand() % 629 - 314) / 100.0f;

		//移動量を設定する
		fSpeed = (float)(rand() % 300) / 100.0f + 4.0f;

		//移動量を設定する
		move.x = sinf(fAngle) * fSpeed;
		move.y = cosf(fAngle) * fSpeed;

		//半径を設定する
		fRadius = (float)(rand() % 1000) / 100.0f + 30.0f;

		// 寿命の設定
		nLife = rand() % 30 + 7;

		// エフェクトの設定処理
		CEffect::Create
		(
			ShiftPos,			// 位置
			move,				// 移動量
			nLife,				// 寿命
			fRadius,			// 半径
			CEffect::TYPE_FIRE,	// 種類
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			true
		);
	}
}

//===============================
// 着地煙の生成
//===============================
void CParticle::InitDust(const D3DXVECTOR3& pos)
{
	// ローカル変数宣言
	D3DXVECTOR3 EffectPos;		// エフェクトの位置
	D3DXVECTOR3 EffectMove;		// エフェクトの移動量
	int nLife;					// 寿命
	D3DXVECTOR3 randRot;		// ランダムの向き

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		// 向きを設定する
		randRot.x = (float)((rand() % 629 - 314) * 0.01f);
		EffectPos.x = pos.x + sinf(randRot.x) * 20.0f;

		EffectPos.y = pos.y;

		// 向きを設定する
		randRot.z = (float)((rand() % 629 - 314) * 0.01f);
		EffectPos.z = pos.z + cosf(randRot.z) * 20.0f;

		for (int nCntDust = 0; nCntDust < 4; nCntDust++)
		{
			EffectMove.x = sinf(randRot.x) * (rand() % 3 + 10);
			EffectMove.y = (float)(rand() % 3 + 1);
			EffectMove.z = cosf(randRot.z) * (rand() % 3 + 10);

			// 寿命を設定する
			nLife = rand() % 20 + 10;

			// エフェクトの生成処理
			CEffect::Create
			(
				EffectPos,
				EffectMove,
				nLife,
				50.0f,
				CEffect::TYPE_DUST,
				D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f),
				false
			);
		}
	}
}

//===============================
// アイテムゲットの生成
//===============================
void CParticle::InitItemGet(const D3DXVECTOR3& pos)
{
	// ローカル変数宣言
	D3DXVECTOR3 move;		// エフェクトの移動量
	int nLife;				// 寿命
	float fRot;				// 向き
	float fRadius;			// 半径

	for (int nCntDust = 0; nCntDust < 15; nCntDust++)
	{
		// 向きを設定する
		fRot = (float)((rand() % 629 - 314) * 0.01f);

		// 移動量を設定する
		move.x = sinf(fRot) * (rand() % 21 - 10);
		move.y = cosf(fRot) * (rand() % 21 - 10);
		move.z = cosf(fRot) * (rand() % 21 - 10);

		// 寿命を設定する
		nLife = rand() % 10 + 15;

		// 半径を設定する
		fRadius = (float)(rand() % 8 + 4);

		// エフェクトの生成処理
		CEffect::Create
		(
			pos,
			move,
			nLife,
			fRadius,
			CEffect::TYPE_NONE,
			D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f),
			true
		);
	}
}

//===============================
// 敵の死亡の生成
//===============================
void CParticle::InitEnemyDeath(const D3DXVECTOR3& pos)
{
	// ローカル変数宣言
	D3DXVECTOR3 move;		// エフェクトの移動量
	int nLife;				// 寿命
	float fRot;				// 向き
	float fRadius;			// 半径

	for (int nCntDust = 0; nCntDust < 10; nCntDust++)
	{
		// 向きを設定する
		fRot = (float)((rand() % 629 - 314) * 0.01f);

		// 移動量を設定する
		move.x = sinf(fRot) * (rand() % 21 - 10);
		move.y = cosf(fRot) * (rand() % 21 - 10);
		move.z = 0.0f;

		// 寿命を設定する
		nLife = rand() % 10 + 20;

		// 半径を設定する
		fRadius = (float)(rand() % 10 + 15);

		// エフェクトの生成処理
		CEffect::Create
		(
			pos,
			move,
			nLife,
			fRadius,
			CEffect::TYPE_RUPTURE,
			D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f),
			false
		);
	}
}