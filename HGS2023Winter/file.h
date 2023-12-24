//============================================
//
// ファイルヘッダー[file.h]
// Author：小原立暉
//
//============================================
#ifndef _FILE_H_			//このマクロ定義がされていなかったら
#define _FILE_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"
#include "ranking.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define MAX_FILE_DATA		(512)		// ファイルのデータの最大数

//--------------------------------------------
// クラス(ファイル読み込みクラス)
//--------------------------------------------
class CFile
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_RANKING = 0,	// ランキング
		TYPE_MAX			// この列挙型の総数
	};

	// ランキングの情報
	struct SRankingInfo
	{
		int aRank[MAX_RANKING];				// ランキングのデータ
		bool bSuccess;						// 成功状況
	};

	CFile();					// コンストラクタ
	~CFile();					// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理

	HRESULT Save(const TYPE type);	// セーブ処理
	HRESULT Load(const TYPE type);	// ロード処理

	void SetRankingInfo(int* pRank);		// ランキングの設定処理
	SRankingInfo GetRankingInfo(void);		// ランキングの取得処理

private:		// 自分のみアクセスできる

	// メンバ関数(セーブ関係)
	HRESULT SaveRanking(void);		// ランキングのセーブ処理

	// メンバ関数(ロード関係)
	HRESULT LoadRanking(void);		// ランキングのロード処理

	// メンバ変数
	SRankingInfo m_RankingInfo;		// ランキングの情報

	// 静的メンバ変数
	static const char* c_apBooleanDisp[2];			// bool型の表示
};

#endif