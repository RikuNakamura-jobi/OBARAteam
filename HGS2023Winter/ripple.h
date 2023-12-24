//===================================
//
// 波紋ヘッダー[ripple.h]
// Author：小原立暉
//
//===================================
#ifndef _RIPPLE_H_			// このマクロ定義がされていなかったら
#define _RIPPLE_H_			// 2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// クラス定義(着地の波紋)
//-----------------------------------
class CRipple : public CModel
{
public:		// 誰でもアクセスできる

	CRipple();			// コンストラクタ
	~CRipple();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);			// 情報の設定処理

	// 静的メンバ関数
	static CRipple* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);		// 生成処理

private:	// 自分だけアクセスできる

	// メンバ変数
	float m_fAlpha;								// 透明度
};

#endif