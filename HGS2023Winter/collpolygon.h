//============================================
//
// 当たり判定ポリゴンヘッダー[collpolygon.h]
// Author：小原立暉
//
//============================================
#ifndef _COLLPOLYGON_H_		//このマクロ定義がされていなかったら
#define _COLLPOLYGON_H_		//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "object3D.h"

//--------------------------------------------
// クラス(弾クラス)
//--------------------------------------------
class CCollPolygon : public CObject3D
{
public:			// 誰でもアクセスできる

	CCollPolygon();				// コンストラクタ
	~CCollPolygon();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& VtxMax, const D3DXVECTOR3& VtxMin);		// 情報の設定処理

	// セット・ゲット関数
	D3DXVECTOR3 GetVtxMax(void) const;		// 最大値の取得処理

	D3DXVECTOR3 GetVtxMin(void) const;		// 最小値の取得処理

	int GetNumID(void) const;				// ポリゴンの番号の取得処理

	// 静的メンバ関数
	static int Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& VtxMax, const D3DXVECTOR3& VtxMin);	// 生成処理
	static void SetPosRot(const int nIdx, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);				// 位置と向きの設定処理
	static int Delete(const int nIdx);																	// 破棄処理
	static CCollPolygon* GetCollPolygon(const int nIdx);												// 当たり判定ポリゴンの取得処理

private:		// 自分だけアクセスできる

	// メンバ変数
	D3DXVECTOR3 m_vtxMax;		// 右上の座標
	D3DXVECTOR3 m_vtxMin;		// 左下の座標
	int m_nNumID;				// ポリゴンの番号

	// 静的メンバ変数
	static int m_nNumAll;		// ポリゴンの総数
};

#endif