//===================================
//
// 2Dエフェクトヘッダー[effect2D.h]
// Author 小原立暉
//
//===================================
#ifndef _EFFECT2D_H_
#define _EFFECT2D_H_

//***********************************
// インクルードファイル
//***********************************
#include "object2D.h"

//-----------------------------------
// クラス定義(CEffect2D)
//-----------------------------------
class CEffect2D : public CObject2D
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_NONE = 0,	// 通常
		TYPE_SHINING,	// 輝き
		TYPE_OVERTOP,	// 限界突破
		TYPE_INFIRE,	// 内側の炎
		TYPE_OUTFIRE,	// 外側の炎
		TYPE_MAX		// この列挙型の総数
	};

	CEffect2D();			// コンストラクタ
	~CEffect2D();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int nLife, const float fRadius, const TYPE type);					// 情報の設定処理

	// 静的メンバ関数
	static CEffect2D* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const int nLife, const float fRadius, const TYPE type);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void TypeProcess(void);		// 種類による処理

	// メンバ変数
	D3DXVECTOR3 m_move;		// 移動量
	D3DXCOLOR m_col;		// 色
	TYPE m_type;			// 種類
	int m_nLife;			// 寿命
	float m_fSub;			// 透明になる間隔
	float m_fContra;		// 半径の縮む間隔
	bool m_bAdd;			// 加算合成状況
};

#endif