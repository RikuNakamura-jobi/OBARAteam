//============================================
//
// サンプルマネージャーヘッダー[sample_manager.h]
// Author：小原立暉
//
//============================================
#ifndef _SAMPLE_MANAGER_H_		// このマクロ定義がされていなかったら
#define _SAMPLE_MANAGER_H_		// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CListSample;		// サンプル

//--------------------------------------------
// クラス(サンプルマネージャークラス)
//--------------------------------------------
class CSampleManager
{
public:				// 誰でもアクセスできる

	CSampleManager();			// コンストラクタ
	~CSampleManager();			// デストラクタ

	// メンバ関数
	void Regist(CListSample* pThis);		// 登録処理
	void Uninit(void);					// 終了処理
	
	CListSample* GetTop(void);				// 先頭の取得処理
	void Pull(CListSample* pThis);			// リスト構造の引き抜き処理

	// 静的メンバ関数
	static CSampleManager* Create(void);	// 生成処理
	static CSampleManager* Get(void);		// 取得処理

private:			// 自分だけアクセスできる

	// メンバ変数
	CListSample* m_pTop;	// 先頭のオブジェクト
	int m_nNumAll;			// オブジェクトの総数

	// 静的メンバ変数
	static CSampleManager* m_pManager;		// マネージャーの変数
};

#endif