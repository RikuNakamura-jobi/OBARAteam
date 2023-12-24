//===================================
//
// 紙吹雪ヘッダー[confetti.h]
// Author 小原立暉
//
//===================================
#ifndef _CONFETTI_H_
#define _CONFETTI_H_

//***********************************
// インクルードファイル
//***********************************
#include "object3D.h"

//-----------------------------------
// クラス定義(CConfetti)
//-----------------------------------
class CConfetti : public CObject3D
{
public:

	CConfetti();			// コンストラクタ
	~CConfetti();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXVECTOR3& rotMove, const float fGravity, const D3DXCOLOR& col, const int nLife);				// 情報の設定処理

	// 静的メンバ関数
	static CConfetti* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXVECTOR3& rotMove, const float fGravity, const D3DXCOLOR& col, const int nLife);		// 生成処理

private:

	// メンバ変数
	D3DXVECTOR3 m_rotMove;		// 向きの移動量
	int m_nLife;				// 寿命
	float m_fGravity;			// 重力
};

#endif