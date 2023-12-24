//===================================
//
// 伝達メッセージヘッダー[speech_message.h]
// Author 小原立暉
//
//===================================
#ifndef _SPEECH_MESSAGE_H_
#define _SPEECH_MESSAGE_H_

//***********************************
// インクルードファイル
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// クラス定義(伝達メッセージ)
//-----------------------------------
class CSpeechMessage : public CBillboard
{
public:

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_EXTEND = 0,	// 伸び状態
		STATE_SHRINK,		// 縮み状態
		STATE_MAX			// この先の処理を行わない
	};

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_HELP = 0,		// 救助要請
		TYPE_MAX			// この列挙型の総数
	};

	CSpeechMessage();		// コンストラクタ
	~CSpeechMessage();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const int nLife, const TYPE type);					// 情報の設定処理

	// 静的メンバ関数
	static CSpeechMessage* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const int nLife, const TYPE type);	// 生成処理

private:

	// メンバ変数
	D3DXVECTOR3 m_sizeDest;	// 目的のサイズ
	D3DXVECTOR3 m_sizeCopy;	// 最初のサイズ
	STATE m_state;			// 状態
	TYPE m_type;			// 種類
	int m_nLife;			// 寿命
	int m_nStateCount;		// 状態のカウント
};

#endif