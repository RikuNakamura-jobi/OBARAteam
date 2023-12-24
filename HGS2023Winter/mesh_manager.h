//============================================
//
// メッシュマネージャーヘッダー[mesh_manager.h]
// Author：小原立暉
//
//============================================
#ifndef _MESH_MANAGER_H_		// このマクロ定義がされていなかったら
#define _MESH_MANAGER_H_		// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CMesh;		// 起伏

//--------------------------------------------
// クラス(起伏マネージャークラス)
//--------------------------------------------
class CMeshManager
{
public:				// 誰でもアクセスできる

	CMeshManager();			// コンストラクタ
	~CMeshManager();			// デストラクタ

	// メンバ関数
	void Regist(CMesh* pThis);		// 登録処理
	void Uninit(void);						// 終了処理
	
	CMesh* GetTop(void);				// 先頭の取得処理
	void Pull(CMesh* pThis);			// リスト構造の引き抜き処理

	// 静的メンバ関数
	static CMeshManager* Create(void);		// 生成処理
	static CMeshManager* Get(void);		// 取得処理

private:			// 自分だけアクセスできる

	// メンバ変数
	CMesh* m_pTop;		// 先頭のオブジェクト
	int m_nNumAll;			// オブジェクトの総数

	// 静的メンバ変数
	static CMeshManager* m_pManager;		// マネージャーの変数
};

#endif