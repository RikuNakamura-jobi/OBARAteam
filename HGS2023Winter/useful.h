//============================================
//
// 便利ヘッダー[useful.h]
// Author：小原立暉
//
//============================================
#ifndef _USEFUL_H_				//このマクロ定義がされていなかったら
#define _USEFUL_H_				//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"

//--------------------------------------------
// 名前空間
//--------------------------------------------
namespace useful
{
	void RotNormalize(float *pRot);													// 向きの正規化
	void Gravity(float *fGravity, D3DXVECTOR3& pPos, const float fAdd);				// 重力処理
	void RotCorrect(const float fDest, float *pRot, const float fCorrect);			// 向きの補正処理(ホーミング処理)
	void Correct(const float fDest, float *pValue, const float fCorrect);			// 値の補正処理
	void ColReverse(float *pData);													// 1.0fと0.0fを反転する処理
	bool CircleCollisionXY(const D3DXVECTOR3& pos0, const D3DXVECTOR3& pos1, const float fRadius0, const float fRadius1);		// 円の当たり判定(XY平面)
	bool CircleCollisionXZ(const D3DXVECTOR3& pos0, const D3DXVECTOR3& pos1, const float fRadius0, const float fRadius1);		// 円の当たり判定(XZ平面)
	float LineOuterProductXZ(D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 pos);										// 外積の左右判断処理(XZ平面)
	float LineOuterProductXY(D3DXVECTOR3 posUp, D3DXVECTOR3 posDown, D3DXVECTOR3 pos);											// 外積の左右判断処理(XY平面)
	void NormalizeVector(D3DXVECTOR3& nor, const D3DXVECTOR3& posRight, const D3DXVECTOR3& posLeft, const D3DXVECTOR3& pos);	// 法線の計算(正規化)
	float InnerProduct(const D3DXVECTOR3 VecMove, const D3DXVECTOR3 VecNor);		// 内積の判定処理
	bool CollisionOuterProductXZ(D3DXVECTOR3& Targetpos, D3DXVECTOR3& TargetposOld, const D3DXVECTOR3 posRight, const D3DXVECTOR3 posLeft);		// 外積の当たり判定処理(XZ軸版)
	bool CollisionOuterProductXY(D3DXVECTOR3& Targetpos, D3DXVECTOR3& TargetposOld, const D3DXVECTOR3 posRight, const D3DXVECTOR3 posLeft);		// 外積の当たり判定処理(XY軸版)
	bool RectangleCollisionXY(const D3DXVECTOR3& pos0, const D3DXVECTOR3& pos1, const D3DXVECTOR3& VtxMax0, const D3DXVECTOR3& VtxMax1, const D3DXVECTOR3& VtxMin0, const D3DXVECTOR3& VtxMin1);		// XY座標の矩形の当たり判定
	bool RectangleCollisionXZ(const D3DXVECTOR3& pos0, const D3DXVECTOR3& pos1, const D3DXVECTOR3& VtxMax0, const D3DXVECTOR3& VtxMax1, const D3DXVECTOR3& VtxMin0, const D3DXVECTOR3& VtxMin1);		// XZ座標の矩形の当たり判定
	void CylinderCollision(D3DXVECTOR3* TargetPos, const D3DXVECTOR3& ObjectPos, const float fObjectRadius);					// 円柱の当たり判定
	void DecimalCalculation(const int nDigit, const int nValue, int* aAnswer);		// 10進数への変換処理
	bool FrameCorrect(const float fDest, float *fTarget, const float fCorrect);		// 均等な数値の補正
}

#endif