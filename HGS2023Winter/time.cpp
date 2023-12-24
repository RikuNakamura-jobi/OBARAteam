//============================================
//
// 時間処理[time.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "time.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

// マクロ定義
#define TIME_TEXTURE		"data\\TEXTURE\\TimeNumber.png"		// 時間のテクスチャ
#define TIMEDOT_TEXTURE		"data\\TEXTURE\\TimeDot.png"		// 時間の点のテクスチャ
#define TIMEDOT_COUNT		(3)									// 時間の点の番号

//========================
// コンストラクタ
//========================
CTime::CTime() : CObject(CObject::TYPE_TIME, CObject::PRIORITY_UI)
{
	// コンストラクタの箱
	Box();
}

//========================
// オーバーロードコンストラクタ
//========================
CTime::CTime(CObject::TYPE type, PRIORITY priority) : CObject(type, priority)
{
	// コンストラクタの箱
	Box();
}

//========================
// デストラクタ
//========================
CTime::~CTime()
{

}

//========================
// コンストラクタの箱
//========================
void CTime::Box(void)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_TIME_DIGIT; nCnt++)
	{
		// 数字の情報
		m_apNumber[nCnt] = nullptr;
	}

	m_pDot = nullptr;				// タイムの点
	m_TotalTime = 0;				// 合計時刻
}

//========================
// 初期化処理
//========================
HRESULT CTime::Init(void)
{
	// ローカル変数宣言
	int nTexIdx;		// テクスチャのインデックス

	// 全ての値を初期化する
	m_TotalTime = 0;				// 合計時刻

	// テクスチャの読み込み処理
	nTexIdx = CManager::Get()->GetTexture()->Regist(TIME_TEXTURE);

	// メモリを確保する
	for (int nCnt = 0; nCnt < MAX_TIME_DIGIT; nCnt++)
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

	// タイムの点を生成する
	m_pDot = CObject2D::Create(CObject2D::TYPE_NONE, CObject::TYPE_NONE, CObject::PRIORITY_UI);

	if (m_pDot != nullptr &&
		CManager::Get()->GetTexture() != nullptr)
	{ // タイムの点が NULL じゃないかつ、テクスチャが NULL じゃない場合

		// テクスチャの割り当て処理
		m_pDot->BindTexture(CManager::Get()->GetTexture()->Regist(TIMEDOT_TEXTURE));
	}
	else
	{ // 上記以外

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CTime::Uninit(void)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_TIME_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt] != nullptr)
		{ // 時間のポインタが NULL じゃない場合

			// 終了処理
			m_apNumber[nCnt]->Uninit();
			m_apNumber[nCnt] = nullptr;
		}
	}

	if (m_pDot != nullptr)
	{ // 点の情報が NULL じゃない場合

		// タイムの点の終了処理
		m_pDot->Uninit();
		m_pDot = nullptr;
	}

	// 数字の終了処理
	Release();
}

//========================
// 更新処理
//========================
void CTime::Update(void)
{
	// 1桁ごとの数値設定処理
	SetNumber();
}

//========================
// 描画処理
//========================
void CTime::Draw(void)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_TIME_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt] != nullptr)
		{ // 番号のポインタが NULL じゃない場合

			// 描画処理
			m_apNumber[nCnt]->Draw();
		}
	}

	if (m_pDot != nullptr)
	{ // タイムの点が NULL じゃない場合

		// 描画処理
		m_pDot->Draw();
	}
}

//========================
// 情報の設定処理
//========================
void CTime::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const D3DXVECTOR3& dotSize, const float fShift)
{
	// ローカル変数宣言
	D3DXVECTOR3 Numberpos;		// 番号の位置

	for (int nCnt = 0; nCnt < MAX_TIME_DIGIT; nCnt++)
	{
		if (nCnt == TIMEDOT_COUNT)
		{ // 点を打ちこむカウントの場合

			// 点の位置を設定する
			Numberpos.x = pos.x + (nCnt * fShift) - size.x - (dotSize.x * 0.5f);
			Numberpos.y = pos.y + size.y - dotSize.y;
			Numberpos.z = 0.0f;

			// 情報を設定する
			m_pDot->SetPos(Numberpos);		// 位置
			m_pDot->SetRot(rot);			// 向き
			m_pDot->SetSize(dotSize);		// サイズ
			m_pDot->SetAngle();				// 方向
			m_pDot->SetLength();			// 長さ

			// 頂点情報の設定処理
			m_pDot->SetVertex();
		}

		// 番号の位置を設定する
		Numberpos.x = pos.x + (nCnt * fShift);
		Numberpos.y = pos.y;
		Numberpos.z = 0.0f;

		if (m_apNumber[nCnt] != nullptr)
		{ // 番号のポインタが NULL じゃない場合

			// 設定処理
			m_apNumber[nCnt]->SetPos(Numberpos);		// 位置設定
			m_apNumber[nCnt]->SetRot(rot);				// 向き設定
			m_apNumber[nCnt]->SetSize(size);			// サイズ設定
			m_apNumber[nCnt]->SetLength();				// 長さ設定
			m_apNumber[nCnt]->SetAngle();				// 方向設定

			m_apNumber[nCnt]->SetType(CNumber::TYPE_TIME);		// 番号の種類
			m_apNumber[nCnt]->SetNumber(0);				// 番号を設定する

			// 頂点情報の設定処理
			m_apNumber[nCnt]->SetVertex();

			// テクスチャの設定処理(アニメーションバージョン)
			m_apNumber[nCnt]->SetVtxTextureAnim(NUMBER_TEXTURE_PATTERN, 0);
		}
	}
}

//========================
// 1桁ごとの数値設定処理
//========================
void CTime::SetNumber(void)
{
	// ローカル変数宣言
	int aNum[MAX_TIME_DIGIT];		// 数字

	// 10進数への変換処理
	useful::DecimalCalculation(MAX_TIME_DIGIT, m_TotalTime, &aNum[0]);

	for (int nCnt = 0; nCnt < MAX_TIME_DIGIT; nCnt++)
	{
		// 値を設定する
		m_apNumber[nCnt]->SetNumber(aNum[nCnt]);
	}
}

//========================
// 1桁ごとのテクスチャの設定処理
//========================
void CTime::SetTexture(void)
{
	for (int nCnt = 0; nCnt < MAX_TIME_DIGIT; nCnt++)
	{
		// テクスチャ座標の設定処理
		m_apNumber[nCnt]->SetVtxTextureAnim(NUMBER_TEXTURE_PATTERN, m_apNumber[nCnt]->GetNumber());
	}
}

//========================
// 経過時間の設定処理
//========================
void CTime::SetTotalTime(const DWORD time)
{
	// 経過時間を設定する
	m_TotalTime = time;
}

//========================
// 経過時間の取得処理
//========================
DWORD CTime::GetTotalTime(void) const
{
	// 経過時間を返す
	return m_TotalTime;
}

//========================
// 番号情報の取得処理
//========================
CNumber* CTime::GetNumber(const int nID)
{
	// 番号の情報を返す
	return m_apNumber[nID];
}

//========================
// 点の取得処理
//========================
CObject2D* CTime::GetDot(void)
{
	// 点の情報を取得する
	return m_pDot;
}

//========================
// 生成処理
//========================
CTime* CTime::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const D3DXVECTOR3& dotSize, const float fShift)
{
	// ローカルオブジェクトを生成
	CTime* pTime = nullptr;	// プレイヤーのインスタンスを生成

	if (pTime == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pTime = new CTime;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pTime != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pTime->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pTime->SetData(pos, rot, size, dotSize, fShift);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pTime;
}