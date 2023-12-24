//============================================
//
// スコア処理[score.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "score.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

// マクロ定義
#define SCORE_TEXTURE		"data\\TEXTURE\\Number.png"		// スコアのテクスチャ
#define DISP_ADD_SCORE		(50)							// 描画用スコアの加算数

//========================
// コンストラクタ
//========================
CScore::CScore() : CObject(TYPE_SCORE, PRIORITY_UI)
{
	// コンストラクタの箱
	Box();
}

//========================
// オーバーロードコンストラクタ
//========================
CScore::CScore(CObject::TYPE type, CObject::PRIORITY priority) : CObject(type, priority)
{
	// コンストラクタの箱
	Box();
}

//========================
// デストラクタ
//========================
CScore::~CScore()
{

}

//========================
// コンストラクタの箱
//========================
void CScore::Box(void)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		// 数字の情報
		m_apNumber[nCnt] = nullptr;
	}

	m_nScore = 0;		// スコア
	m_nDispScore = 0;	// 表示用スコア
}

//========================
// 初期化処理
//========================
HRESULT CScore::Init(void)
{
	//ローカル変数宣言
	int nTexIdx = NONE_TEXIDX;		// テクスチャのインデックス

	// テクスチャの読み込み処理
	nTexIdx = CManager::Get()->GetTexture()->Regist(SCORE_TEXTURE);

	// 全ての値を初期化する
	m_nScore = 0;		// スコア
	m_nDispScore = 0;	// 表示用スコア

	// メモリを確保する
	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt] == nullptr)
		{ // ポインタが NULL の場合

			// メモリを確保する
			m_apNumber[nCnt] = new CNumber(CObject::TYPE_NONE, CObject::PRIORITY_UI);
		}
		else
		{ // ポインタが NULL じゃない場合

			// 停止
			assert(false);

			// 失敗を返す
			return E_FAIL;
		}

		if (m_apNumber[nCnt] != nullptr)
		{ // ポインタが NULL じゃない場合

			// 初期化処理
			if (FAILED(m_apNumber[nCnt]->Init()))
			{ // 初期化処理に失敗した場合

				// 停止
				assert(false);

				// 失敗を返す
				return E_FAIL;
			}

			// テクスチャの割り当て処理
			m_apNumber[nCnt]->BindTexture(nTexIdx);
		}
		else
		{ // ポインタが　NULL の場合

			// 停止
			assert(false);

			// 失敗を返す
			return E_FAIL;
		}
	}

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CScore::Uninit(void)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		// 終了処理
		m_apNumber[nCnt]->Uninit();
	}

	// 数字の終了処理
	Release();
}

//========================
// 更新処理
//========================
void CScore::Update(void)
{
	if (m_nDispScore >= m_nScore)
	{ // 描画用スコアが実際のスコアを上回った場合

		// 描画用スコアを設定する
		m_nDispScore = m_nScore;
	}
	else
	{ // 上記以外

		// 描画用スコアを加算する
		m_nDispScore += DISP_ADD_SCORE;
	}

	// 計算処理
	Calculate();

	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		// テクスチャ座標の設定処理
		m_apNumber[nCnt]->SetVtxTextureAnim(NUMBER_TEXTURE_PATTERN, m_apNumber[nCnt]->GetNumber());
	}
}

//========================
// 描画処理
//========================
void CScore::Draw(void)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		// 描画処理
		m_apNumber[nCnt]->Draw();
	}
}

//========================
// 情報の設定処理
//========================
void CScore::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const float fShift)
{
	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		// 設定処理
		m_apNumber[nCnt]->SetPos(D3DXVECTOR3(pos.x + (nCnt * fShift), pos.y, 0.0f));		// 位置設定
		m_apNumber[nCnt]->SetRot(rot);				// 向き設定
		m_apNumber[nCnt]->SetSize(size);			// サイズ設定
		m_apNumber[nCnt]->SetLength();				// 長さ設定
		m_apNumber[nCnt]->SetAngle();				// 方向設定

		m_apNumber[nCnt]->SetType(CNumber::TYPE_DECIMAL);	// 番号の種類
		m_apNumber[nCnt]->SetNumber(0);				// 番号を設定する

		// 頂点情報の設定処理
		m_apNumber[nCnt]->SetVertex();

		// テクスチャの設定処理(アニメーションバージョン)
		m_apNumber[nCnt]->SetVtxTextureAnim(NUMBER_TEXTURE_PATTERN, 0);
	}
}

//========================
// 計算処理
//========================
void CScore::Calculate(void)
{
	// ローカル変数宣言
	int aNum[MAX_SCORE_DIGIT];		// 数値

	// 10進数への計算処理
	useful::DecimalCalculation(MAX_SCORE_DIGIT, m_nDispScore, &aNum[0]);

	for (int nCnt = 0; nCnt < MAX_SCORE_DIGIT; nCnt++)
	{
		//値の計算
		m_apNumber[nCnt]->SetNumber(aNum[nCnt]);
	}
}

//========================
// スコアの設定処理
//========================
void CScore::SetScore(const int nScore)
{
	// スコアを代入する
	m_nScore = nScore;
}

//========================
// スコアの取得処理
//========================
int CScore::GetScore(void) const
{
	// スコアを返す
	return m_nScore;
}

//========================
// 表示スコアの設定処理
//========================
void CScore::SetScoreDisp(const int nScore)
{
	// 表示用スコアを設定する
	m_nDispScore = nScore;
}

//========================
// 表示スコアの取得処理
//========================
int CScore::GetScoreDisp(void) const
{
	// 表示用スコアを返す
	return m_nDispScore;
}

//========================
// 数値の取得処理
//========================
CNumber* CScore::GetNumber(int nID) const
{
	// 数値を返す
	return m_apNumber[nID];
}

//========================
// スコアの加算処理
//========================
void CScore::AddScore(const int nScore)
{
	// スコアを設定する
	m_nScore += nScore;
}