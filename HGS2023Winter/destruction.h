//===================================
//
// 飛沫ヘッダー[destruction.h]
// Author 小原立暉
//
//===================================
#ifndef _DESTRUCTION_H_
#define _DESTRUCTION_H_

//***********************************
// インクルードファイル
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// クラス定義(CDestruction)
//-----------------------------------
class CDestruction : public CBillboard
{
public:

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_THORN = 0,		// トゲトゲ
		TYPE_AIRY,			// フワフワ
		TYPE_EXPLOSION,		// 爆発
		TYPE_MAX			// この列挙型の総数
	};

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_ADD = 0,		// 加算状態
		STATE_SUB,			// 減算状態
		STATE_MAX			// この列挙型の総数
	};

	CDestruction();			// コンストラクタ
	~CDestruction();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col, const TYPE type, const int nLife);				// 情報の設定処理

	// 静的メンバ関数
	static CDestruction* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col, const TYPE type, const int nLife);		// 生成処理

private:

	// メンバ変数
	D3DXCOLOR m_col;	// 色
	STATE m_state;		// 状態
	float m_fAddAlpha;	// 加算する透明度
	float m_fAlphaDest;	// 目的の透明度
};

#endif