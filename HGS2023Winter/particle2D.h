//===================================
//
// パーティクル2Dヘッダー[Particle2D.h]
// Author 小原立暉
//
//===================================
#ifndef _PARTICLE2D_H_
#define _PARTICLE2D_H_

#include "effect2D.h"

//-----------------------------------
// クラス定義(CParticle2D)
//-----------------------------------
class CParticle2D
{
public:			// 誰でもアクセス出来る

	CParticle2D();							// コンストラクタ
	~CParticle2D();							// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	// 静的メンバ関数
	static void Create(const D3DXVECTOR3& pos, const CEffect2D::TYPE type);			// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void InitBoost(const D3DXVECTOR3& pos, const CEffect2D::TYPE type);				// ブーストの初期化
};

#endif