//============================================
//
// スコアヘッダー[score.h]
// Author：小原立暉
//
//============================================
#ifndef _SCORE_H_					// このマクロ定義がされていなかったら
#define _SCORE_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "number.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define MAX_SCORE_DIGIT		(8)			// スコアの桁数

//--------------------------------------------
// クラス定義(スコア)
//--------------------------------------------
class CScore : public CObject
{
public:				// 誰でもアクセスできる

	CScore();						// コンストラクタ
	CScore(CObject::TYPE type, CObject::PRIORITY priority = PRIORITY_UI);		// オーバーロードコンストラクタ
	virtual ~CScore();				// デストラクタ
	void Box(void);					// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void);		// 更新処理
	virtual void Draw(void);		// 描画処理

	virtual void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const float fShift);		// 情報の設定処理

	void Calculate(void);			// 計算処理

	// セット・ゲット関係
	void SetScore(const int nScore);		// スコアの設定処理
	int GetScore(void) const;				// スコアの取得処理
	void SetScoreDisp(const int nScore);	// 表示スコアの設定処理
	int GetScoreDisp(void) const;			// 表示スコアの取得処理
	void AddScore(const int nScore);		// スコアの加算処理
	CNumber* GetNumber(int nID) const;		// 数値の取得処理

private:			// 自分だけアクセスできる

	// メンバ変数
	CNumber *m_apNumber[MAX_SCORE_DIGIT];		// 番号の情報
	int m_nScore;			// スコア
	int m_nDispScore;		// 表示用スコア
};

#endif