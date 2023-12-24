//===================================
//
// 危険マークヘッダー[danger_mark.h]
// Author 小原立暉
//
//===================================
#ifndef _DANGER_MARK_H_
#define _DANGER_MARK_H_

//***********************************
// インクルードファイル
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// クラス定義(CDangerMark)
//-----------------------------------
class CDangerMark : public CBillboard
{
public:

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_YELLOWJAGGY = 0,	// 黄色のギザギザ
		TYPE_MAX				// この列挙型の総数
	};

	CDangerMark();			// コンストラクタ
	~CDangerMark();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col, const TYPE type);					// 情報の設定処理

	// 静的メンバ関数
	static CDangerMark* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col, const TYPE type);		// 生成処理

private:

	// メンバ変数
	D3DXCOLOR m_col;		// 色
	float m_fAlphaDest;		// 目的の透明度
	bool m_bDisp;			// 描画状況
};

#endif