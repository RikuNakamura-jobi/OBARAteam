//===================================
//
// 破片ヘッダー[fraction.h]
// Author 小原立暉
//
//===================================
#ifndef _FRACTION_H_
#define _FRACTION_H_

//***********************************
// インクルードファイル
//***********************************
#include "model.h"

//-----------------------------------
// クラス定義(破片)
//-----------------------------------
class CFraction : public CModel
{
public:			// 誰でもアクセスできる

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_NONE = 0,		// 通常状態
		STATE_DELETE,		// 消去状態
		STATE_MAX			// この列挙型の総数
	};

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_SCREW = 0,		// ネジ
		TYPE_GEAR,			// 歯車
		TYPE_RING,			// 輪っか
		TYPE_WOOD,			// 木
		TYPE_MAX			// この列挙型の総数
	};

	CFraction();			// コンストラクタ
	~CFraction();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const TYPE type);				// 情報の設定処理

	// 静的メンバ関数
	static CFraction* Create(const D3DXVECTOR3& pos, const TYPE type);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void MoveSet(void);		// 移動量の設定処理
	void Move(void);		// 移動処理
	void RotMove(void);		// 向きの移動処理
	void Elevation(void);	// 起伏地面との当たり判定

	// メンバ変数
	D3DXVECTOR3 m_move;			// 移動量
	D3DXVECTOR3 m_rotMove;		// 向きの移動量
	STATE m_state;				// 状態
	int m_nStateCount;			// 状態カウント
	float m_fAlpha;				// 透明度
};

#endif