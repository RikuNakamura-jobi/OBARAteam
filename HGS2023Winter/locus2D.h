//===================================
//
// 2D残像ヘッダー[locus2D.h]
// Author 小原立暉
//
//===================================
#ifndef _LOCUS2D_H_
#define _LOCUS2D_H_

//***********************************
// インクルードファイル
//***********************************
#include "object2D.h"

//-----------------------------------
// クラス定義(CLocus2D)
//-----------------------------------
class CLocus2D : public CObject2D
{
public:			// 誰でもアクセスできる

	CLocus2D();				// コンストラクタ
	~CLocus2D();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fAlpha, const int nLife, const int nTexIdx);				// 情報の設定処理

	// 静的メンバ関数
	static CLocus2D* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const float fAlpha, const int nLife, const int nTexIdx);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	int m_nLife;			// 寿命
	float m_fSub;			// 透明度の減衰量
	float m_fAlpha;			// 透明度
};

#endif