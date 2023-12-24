//===================================
//
// エフェクトヘッダー[Effect.h]
// Author 小原立暉
//
//===================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//***********************************
// インクルードファイル
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// クラス定義(CEffect)
//-----------------------------------
class CEffect : public CBillboard
{
public:

	// 種類
	enum TYPE
	{
		TYPE_NONE = 0,			// 通常
		TYPE_FIRE,				// 炎系
		TYPE_DUST,				// 煙
		TYPE_RUPTURE,			// 破裂
		TYPE_SMOKE,				// 煙
		TYPE_POPSMOKE,			// 煙(アニメ調)
		TYPE_MAX				// この列挙型の総数
	};

	CEffect();			// コンストラクタ
	~CEffect();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int nLife, const float fRadius, const TYPE type, const D3DXCOLOR& col, const bool bAdd);				// 情報の設定処理

	// 静的メンバ関数
	static CEffect* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int nLife, const float fRadius, const TYPE type, const D3DXCOLOR& col, const bool bAdd);		// 生成処理

private:

	// メンバ関数
	void Move(void);				// 移動処理
	void SubAlpha(void);			// 透明度減算処理
	void Sizing(void);				// サイジング処理

	// 種類ごとの処理
	void FireProcess(void);			// 炎系の処理
	void RuptureProcess(void);		// 破裂の処理

	// メンバ変数
	D3DXVECTOR3 m_move;		// 移動量
	D3DXCOLOR m_col;		// 色
	TYPE m_type;			// 種類
	int m_nLife;			// 寿命
	int m_nLifeOrigin;		// 元々の寿命
	float m_fSub;			// 透明になる間隔
	float m_fContra;		// 半径の縮む間隔
	bool m_bAdd;			// 加算合成状況
};

#endif