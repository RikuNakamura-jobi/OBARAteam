//===================================
//
// パーティクルヘッダー[Particle.h]
// Author 小原立暉
//
//===================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "Effect.h"

//-----------------------------------
// クラス定義(CParticle)
//-----------------------------------
class CParticle
{
public:			// 誰でもアクセス出来る

	// パーティクルの種類
	enum TYPE
	{
		TYPE_FIRE = 0,		// 炎
		TYPE_DUST,			// 埃
		TYPE_ITEMGET,		// アイテムゲット
		TYPE_ENEMYDEATH,	// 敵の死亡
		TYPE_MAX
	};

	CParticle();							// コンストラクタ
	~CParticle();							// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	// 静的メンバ関数
	static void Create(const D3DXVECTOR3& pos, const CParticle::TYPE type);		// 生成処理

private:		// 自分だけアクセスできる

	// パーティクル発生処理
	void InitFire(const D3DXVECTOR3& pos);			// 炎系の生成
	void InitDust(const D3DXVECTOR3& pos);			// 着地煙の生成
	void InitItemGet(const D3DXVECTOR3& pos);		// アイテムゲットの生成
	void InitEnemyDeath(const D3DXVECTOR3& pos);	// 敵の死亡の生成
};

#endif