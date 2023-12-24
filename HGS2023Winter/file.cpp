//============================================
//
// ファイルのメイン処理[file.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "scene.h"
#include "file.h"
#include "object.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define RANKING_BIN			"data\\BIN\\Ranking.bin"		// ランキングのテキスト

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
const char* CFile::c_apBooleanDisp[2] =					// bool型の表示
{
	"FALSE",							// TRUE
	"TRUE",								// FALSE
};

//===========================================
// コンストラクタ
//===========================================
CFile::CFile()
{
	// 全ての値をクリアする
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		m_RankingInfo.aRank[nCntRank] = 0;				// ランキングの値
	}

	// 成功状況をクリアする
	m_RankingInfo.bSuccess = false;		// ランキング
}

//===========================================
// デストラクタ
//===========================================
CFile::~CFile()
{

}

//===========================================
// セーブ処理
//===========================================
HRESULT CFile::Save(const TYPE type)
{
	switch (type)
	{
	case TYPE_RANKING:

		// ランキングのセーブ処理
		if (FAILED(SaveRanking()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// ロード処理
//===========================================
HRESULT CFile::Load(const TYPE type)
{
	switch (type)
	{
	case TYPE_RANKING:

		// ランキングのロード処理
		if (FAILED(LoadRanking()))
		{ // 失敗した場合

			// 失敗を返す
			return E_FAIL;
		}

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 結果を返す
	return S_OK;
}

//===========================================
// ランキングの設定処理
//===========================================
void CFile::SetRankingInfo(int* pRank)
{
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++, pRank++)
	{
		// ランキングの情報を設定する
		m_RankingInfo.aRank[nCnt] = *pRank;
	}
}

//===========================================
// ランキングの取得処理
//===========================================
CFile::SRankingInfo CFile::GetRankingInfo(void)
{
	// ランキングの情報を返す
	return m_RankingInfo;
}

//===========================================
// 初期化処理
//===========================================
HRESULT CFile::Init(void)
{
	// 全ての値を初期化する
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		m_RankingInfo.aRank[nCntRank] = 0;				// ランキングの値
	}

	// 成功状況を初期化する
	m_RankingInfo.bSuccess = false;		// ランキング

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CFile::Uninit(void)
{
	
}

//===========================================
// ランキングのセーブ処理
//===========================================
HRESULT CFile::SaveRanking(void)
{
	FILE *pFile;												// ファイルポインタを宣言
	m_RankingInfo.bSuccess = false;								// 成功状況

	// ファイルを開く
	pFile = fopen(RANKING_BIN, "wb");			// バイナリファイルに書き込むために開く

	// ファイルを比較する
	if (pFile != NULL)
	{ // ファイルが開けた場合

		// ファイルから数値を書き出す
		fwrite(&m_RankingInfo.aRank[0], sizeof(int), MAX_RANKING, pFile);

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_RankingInfo.bSuccess = true;

		// 成功を返す
		return S_OK;
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}
}

//===========================================
// ランキングのロード処理
//===========================================
HRESULT CFile::LoadRanking(void)
{
	FILE *pFile;						// ファイルポインタを宣言
	m_RankingInfo.bSuccess = false;		// 成功状況

	// ファイルを開く
	pFile = fopen(RANKING_BIN, "rb");			// バイナリファイルから読み込むために開く

	// ファイルを比較する
	if (pFile != NULL)
	{ // ファイルが開けた場合

		// ファイルから数値を読み込む
		fread(&m_RankingInfo.aRank[0], sizeof(int), MAX_RANKING, pFile);

		// ファイルを閉じる
		fclose(pFile);

		// 成功状況を true にする
		m_RankingInfo.bSuccess = true;

		// 成功を返す
		return S_OK;
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);

		for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
		{
			// 数値を設定する
			m_RankingInfo.aRank[0] = 0;
		}

		// 失敗を返す
		return E_FAIL;
	}
}