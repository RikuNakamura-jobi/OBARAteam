//===================================
//
// 当たり判定ヘッダー[collision.h]
// Author 小原立暉
//
//===================================
#ifndef _COLLISION_H_			//このマクロ定義がされていなかったら
#define _COLLISION_H_			//2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "main.h"

//-----------------------------------
// 名前空間(当たり判定)
//-----------------------------------
namespace collision
{
	// 丸影の当たり判定
	void ShadowCollision(const D3DXVECTOR3& pos, int nIdx);							// 丸影の当たり判定
	void ShadowElevCollision(D3DXVECTOR3& pos);										// 丸影と起伏地面の当たり判定

	// 起伏地面の当たり判定
	bool ElevOutRangeCollision(D3DXVECTOR3* pPos, const D3DXVECTOR3& posOld, const float fWidth);			// 起伏地面の範囲外の当たり判定
}

#endif