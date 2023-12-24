//============================================
//
// 便利処理[useful.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// マクロ定義
//********************************************
#include "useful.h"
#include "manager.h"

//======================
// 向きの正規化
//======================
void useful::RotNormalize(float *pRot)
{
	if (*pRot > D3DX_PI)
	{ // 向きが3.14fを超えていた場合

		// 向きを補正する
		*pRot -= D3DX_PI * 2;
	}
	else if (*pRot < -D3DX_PI)
	{ // 向きが-3.14fを超えていた場合

		// 向きを補正する
		*pRot += D3DX_PI * 2;
	}
}

//======================
// 重力処理
//======================
void useful::Gravity(float *fGravity, D3DXVECTOR3& pPos, const float fAdd)
{
	// 重力を加算する
	*fGravity -= fAdd;

	// 重力をかける
	pPos.y += *fGravity;
}

//======================
// ホーミング処理
//======================
void useful::RotCorrect(const float fDest, float *pRot, const float fCorrect)
{
	// ローカル変数宣言
	float fRotDiff;			// 向きの差分

	// 向きの差分を求める
	fRotDiff = fDest - *pRot;

	// 向きの正規化
	RotNormalize(&fRotDiff);

	// 向きを補正する
	*pRot += fRotDiff * fCorrect;

	// 向きの正規化
	RotNormalize(pRot);
}

//======================
// 値の補正処理
//======================
void useful::Correct(const float fDest, float *pValue, const float fCorrect)
{
	// ローカル変数宣言
	float fDiff;			// 値の差分

	// 値の差分を求める
	fDiff = fDest - *pValue;

	// 値を補正する
	*pValue += fDiff * fCorrect;
}

//======================
// 1.0fと0.0fを反転する処理
//======================
void useful::ColReverse(float *pData)
{
	if (*pData >= 1.0f)
	{ // データが 0.0f 以下の場合

		// データを 1.0f にする
		*pData = 0.0f;
	}
	else if (*pData <= 0.0f)
	{ // データが 1.0f 以上の場合

		// データを 0.0f にする
		*pData = 1.0f;
	}
}

//======================
// 円の当たり判定(XY平面)
//======================
bool useful::CircleCollisionXY(const D3DXVECTOR3& pos0, const D3DXVECTOR3& pos1, const float fRadius0, const float fRadius1)
{
	// ローカル変数宣言
	float fLength;

	// 円の範囲を算出
	fLength = (pos1.x - pos0.x) * (pos1.x - pos0.x) + (pos1.y - pos0.y) * (pos1.y - pos0.y);

	if (fLength <= (fRadius0 * fRadius1))
	{ // オブジェクトが当たった場合

		// true を返す
		return true;
	}
	else
	{ // オブジェクトが当たっていない場合

		// false を返す
		return false;
	}
}

//======================
// 円の当たり判定(XZ平面)
//======================
bool useful::CircleCollisionXZ(const D3DXVECTOR3& pos0, const D3DXVECTOR3& pos1, const float fRadius0, const float fRadius1)
{
	// ローカル変数宣言
	float fLength;

	// 円の範囲を算出
	fLength = (pos1.x - pos0.x) * (pos1.x - pos0.x) + (pos1.z - pos0.z) * (pos1.z - pos0.z);

	if (fLength <= (fRadius0 * fRadius1))
	{ // オブジェクトが当たった場合

	  // true を返す
		return true;
	}
	else
	{ // オブジェクトが当たっていない場合

	  // false を返す
		return false;
	}
}

//======================
// 外積の左右判断処理
//======================
float useful::LineOuterProductXZ(D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 pos)
{
	// 変数を宣言
	D3DXVECTOR3 vecLine;	// 境界線ベクトル
	D3DXVECTOR3 vecToPos;	// 左端と位置のベクトル

	// 境界線ベクトルを求める
	vecLine = posRight - posLeft;

	// 左端と位置のベクトルを求める
	vecToPos = pos - posLeft;

	// 外積の計算結果を返す
	return (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
}

//======================
// 外積の左右判断処理(XY平面)
//======================
float useful::LineOuterProductXY(D3DXVECTOR3 posUp, D3DXVECTOR3 posDown, D3DXVECTOR3 pos)
{
	// 変数を宣言
	D3DXVECTOR3 vecLine;	// 境界線ベクトル
	D3DXVECTOR3 vecToPos;	// 左端と位置のベクトル

	// 境界線ベクトルを求める
	vecLine = posDown - posUp;

	// 左端と位置のベクトルを求める
	vecToPos = pos - posUp;

	// 外積の計算結果を返す
	return (vecLine.x * vecToPos.y) - (vecLine.y * vecToPos.x);
}

//======================
// 法線の計算(正規化)
//======================
void useful::NormalizeVector(D3DXVECTOR3& nor, const D3DXVECTOR3& posRight, const D3DXVECTOR3& posLeft,const D3DXVECTOR3& pos)
{
	// ローカル変数宣言
	D3DXVECTOR3 vec1;			// ベクトル1
	D3DXVECTOR3 vec2;			// ベクトル2

	// ベクトルを計算する
	vec1 = posLeft - pos;
	vec2 = posRight - pos;

	// 法線を算出する
	nor = D3DXVECTOR3((vec1.y * vec2.z) - (vec1.z * vec2.y), (vec1.z * vec2.x) - (vec1.x * vec2.z), (vec1.x * vec2.y) - (vec1.y * vec2.x));

	// 法線を正規化する
	D3DXVec3Normalize(&nor, &nor);
}

//======================
// 内積の判定処理
//======================
float useful::InnerProduct(const D3DXVECTOR3 VecMove, const D3DXVECTOR3 VecNor)
{
	// ローカル変数宣言
	float fDot = 0.0f;			// 内積

	// 内積の値を算出する
	fDot = (VecMove.x * VecNor.x) + (VecMove.y * VecNor.y) + (VecMove.z * VecNor.z);

	// 内積の値を返す
	return fDot;
}

//======================
// 外積の当たり判定処理(XZ軸版)
//======================
bool useful::CollisionOuterProductXZ(D3DXVECTOR3& Targetpos, D3DXVECTOR3& TargetposOld, const D3DXVECTOR3 posRight, const D3DXVECTOR3 posLeft)
{
	// ローカル変数宣言
	D3DXVECTOR3 vecMove, vecLine, vecTopos, posCross;	// 外積の変数
	float fRate;										// 割合の変数

	// 弾の軌跡を測る
	vecMove.x = (Targetpos.x - TargetposOld.x);
	vecMove.y = (Targetpos.y - TargetposOld.y);
	vecMove.z = (Targetpos.z - TargetposOld.z);

	// 壁の境界線を測る
	vecLine.x = (posRight.x - posLeft.x);
	vecLine.y = (posRight.y - posLeft.y);
	vecLine.z = (posRight.z - posLeft.z);

	// 壁の始点から弾の位置の距離を測る
	vecTopos.x = (Targetpos.x - posLeft.x);
	vecTopos.y = (Targetpos.y - posLeft.y);
	vecTopos.z = (Targetpos.z - posLeft.z);

	// 割合を求める
	fRate = ((vecTopos.z * vecMove.x) - (vecTopos.x * vecMove.z)) / ((vecLine.z * vecMove.x) - (vecLine.x * vecMove.z));

	// 交点を求める
	posCross.x = vecLine.x * fRate - posLeft.x;
	posCross.y = vecLine.y * fRate - posLeft.y;
	posCross.z = vecLine.z * fRate - posLeft.z;

	if ((vecLine.z * vecTopos.x) - (vecLine.x * vecTopos.z) < 0.0f)
	{ // 境界線を超えた場合

		if (fRate >= 0.0f && fRate <= 1.0f)
		{ // 割合が0.0f～1.0fの間だった(境界線を超えた)場合

			// 位置を設定する
			Targetpos = posCross;

			// true を返す
			return true;
		}
		else
		{ // 上記以外

			// false を返す
			return false;
		}
	}
	else
	{ // 上記以外

		// false を返す
		return false;
	}
}

//======================
// 外積の当たり判定処理(XY軸版)
//======================
bool useful::CollisionOuterProductXY(D3DXVECTOR3& Targetpos, D3DXVECTOR3& TargetposOld, const D3DXVECTOR3 posRight, const D3DXVECTOR3 posLeft)
{
	// ローカル変数宣言
	D3DXVECTOR3 vecMove, vecLine, vecTopos, posCross;	// 外積の変数
	float fRate;										// 割合の変数

	// 弾の軌跡を測る
	vecMove.x = (Targetpos.x - TargetposOld.x);
	vecMove.y = (Targetpos.y - TargetposOld.y);
	vecMove.z = (Targetpos.z - TargetposOld.z);

	// 壁の境界線を測る
	vecLine.x = (posRight.x - posLeft.x);
	vecLine.y = (posRight.y - posLeft.y);
	vecLine.z = (posRight.z - posLeft.z);

	// 壁の始点から弾の位置の距離を測る
	vecTopos.x = (Targetpos.x - posLeft.x);
	vecTopos.y = (Targetpos.y - posLeft.y);
	vecTopos.z = (Targetpos.z - posLeft.z);

	// 割合を求める
	fRate = ((vecTopos.x * vecMove.y) - (vecTopos.y * vecMove.x)) / ((vecLine.x * vecMove.y) - (vecLine.y * vecMove.x));

	// 交点を求める
	posCross.x = vecLine.x * fRate - posLeft.x;
	posCross.y = vecLine.y * fRate - posLeft.y;
	posCross.z = vecLine.z * fRate - posLeft.z;

	if ((vecLine.x * vecTopos.y) - (vecLine.y * vecTopos.x) < 0.0f)
	{ // 境界線を超えた場合

		if (fRate >= 0.0f && fRate <= 1.0f)
		{ // 割合が0.0f～1.0fの間だった(境界線を超えた)場合

			// 位置を設定する
			Targetpos = posCross;

			// true を返す
			return true;
		}
		else
		{ // 上記以外

			// false を返す
			return false;
		}
	}
	else
	{ // 上記以外

		// false を返す
		return false;
	}
}

//======================
// 矩形の当たり判定
//======================
bool useful::RectangleCollisionXY(const D3DXVECTOR3& pos0, const D3DXVECTOR3& pos1, const D3DXVECTOR3& VtxMax0, const D3DXVECTOR3& VtxMax1, const D3DXVECTOR3& VtxMin0, const D3DXVECTOR3& VtxMin1)
{
	if (pos0.x + VtxMax0.x >= pos1.x + VtxMin1.x&&
		pos0.x + VtxMin0.x <= pos1.x + VtxMax1.x&&
		pos0.y + VtxMax0.y >= pos1.y + VtxMin1.y&&
		pos0.y + VtxMin0.y <= pos1.y + VtxMax1.y)
	{ // 当たり判定に当たっていた場合

		// true を返す
		return true;
	}
	else
	{ // 当たり判定に当たっていない場合

		// false を返す
		return false;
	}
}

//======================
// XZ座標の矩形の当たり判定
//======================
bool useful::RectangleCollisionXZ(const D3DXVECTOR3& pos0, const D3DXVECTOR3& pos1, const D3DXVECTOR3& VtxMax0, const D3DXVECTOR3& VtxMax1, const D3DXVECTOR3& VtxMin0, const D3DXVECTOR3& VtxMin1)
{
	if (pos0.x + VtxMax0.x >= pos1.x + VtxMin1.x &&
		pos0.x + VtxMin0.x <= pos1.x + VtxMax1.x &&
		pos0.z + VtxMax0.z >= pos1.z + VtxMin1.z &&
		pos0.z + VtxMin0.z <= pos1.z + VtxMax1.z)
	{ // 当たり判定に当たっていた場合

		// true を返す
		return true;
	}
	else
	{ // 当たり判定に当たっていない場合

		// false を返す
		return false;
	}
}

//======================
// 円柱の当たり判定
//======================
void useful::CylinderCollision(D3DXVECTOR3* TargetPos, const D3DXVECTOR3& ObjectPos, const float fObjectRadius)
{
	// ローカル変数宣言
	float fLength = 0.0f;			// 距離
	float fAngle = 0.0f;			// 向き

	// 向きを取る
	fAngle = atan2f((TargetPos->x - ObjectPos.x), (TargetPos->z - ObjectPos.z));

	// 半径を測る
	fLength = sqrtf((TargetPos->x - ObjectPos.x) * (TargetPos->x - ObjectPos.x) + (TargetPos->z - ObjectPos.z) * (TargetPos->z - ObjectPos.z));

	if (fLength <= fObjectRadius)
	{ // 対象との距離がオブジェクトの半径以下の場合

		// 位置を設定する
		TargetPos->x = ObjectPos.x + sinf(fAngle) * fObjectRadius;
		TargetPos->z = ObjectPos.z + cosf(fAngle) * fObjectRadius;
	}
}

//======================
// 10進数への変換処理
//======================
void useful::DecimalCalculation(const int nDigit, const int nValue, int* aAnswer)
{
	for (int nCnt = 0; nCnt < nDigit; nCnt++)
	{
		// 値を計算
		aAnswer[nCnt] = (nValue % (int)(pow(10, (nDigit - (nCnt)))) / (int)(pow(10, (nDigit - (nCnt + 1)))));
	}
}

//======================
// 均等な数値の補正
//======================
bool useful::FrameCorrect(const float fDest, float *fTarget, const float fCorrect)
{
	if (*fTarget > fDest)
	{ // サイズが目標超過だった場合

		// サイズを減算する
		*fTarget -= fCorrect;

		if (*fTarget <= fDest)
		{ // 目標のサイズ以下になった場合

			// サイズを補正する
			*fTarget = fDest;

			// true を返す
			return true;
		}
	}

	if (*fTarget < fDest)
	{ // サイズが目標未満だった場合

		// サイズを加算する
		*fTarget += fCorrect;

		if (*fTarget >= fDest)
		{ // 目標のサイズ以上になった場合

			// サイズを補正する
			*fTarget = fDest;

			// true を返す
			return true;
		}
	}

	// false を返す
	return false;
}