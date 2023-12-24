//===================================
//
// ロゴマークヘッダー[logo_mark.h]
// Author 小原立暉
//
//===================================
#ifndef _LOGO_MARK_H_
#define _LOGO_MARK_H_

//***********************************
// インクルードファイル
//***********************************
#include "object.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CObject2D;			// オブジェクト2D

//-----------------------------------
// クラス定義(CEffect2D)
//-----------------------------------
class CLogoMark : public CObject
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_SCREEN = 0,	// 画面
		TYPE_MARK,			// マーク
		TYPE_MAX			// この列挙型の総数
	};

	CLogoMark();			// コンストラクタ
	~CLogoMark();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	// 静的メンバ関数
	static CLogoMark* Create(void);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	CObject2D* m_aLogoMark[TYPE_MAX];	// ロゴマークの情報
};

#endif