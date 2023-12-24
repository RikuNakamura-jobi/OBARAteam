//============================================
//
// 気絶演出の処理[stun.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "stun.h"
#include "objectbillboard.h"
#include "texture.h"
#include "useful.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define STUN_SIZE				(D3DXVECTOR3(10.0f, 10.0f, 0.0f))		// 気絶演出のサイズ
#define STUN_TEXTURE			"data\\TEXTURE\\Platform\\Stun.png"		// 気絶演出のテクスチャ
#define STUN_SHIFT_ROT			((float)((D3DX_PI * 2) / 4))			// 気絶演出のずらす時の向き
#define STUN_SHIFT_WIDTH		(30.0f)									// 気絶演出のずらす幅
#define STUN_ADD_ROT			(0.1f)									// 気絶演出の向きの加算数

//========================
// コンストラクタ
//========================
CStun::CStun() : CObject(CObject::TYPE_STUN, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pos = NONE_D3DXVECTOR3;		// 位置
	m_fRot = 0.0f;					// 回転用の向き
	for (int nCntUI = 0; nCntUI < MAX_STUN; nCntUI++)
	{
		m_apStun[nCntUI] = nullptr;
	}
}

//========================
// デストラクタ
//========================
CStun::~CStun()
{

}

//========================
// 初期化処理
//========================
HRESULT CStun::Init(void)
{
	// メモリを確保する
	for (int nCnt = 0; nCnt < MAX_STUN; nCnt++)
	{
		if (m_apStun[nCnt] == nullptr)
		{ // ポインタが NULL の場合

			// メモリを確保する
			m_apStun[nCnt] = new CBillboard(CObject::TYPE_NONE, CObject::PRIORITY_ENTITY);
		}
		else
		{ // ポインタが NULL じゃない場合

			// 停止
			assert(false);

			// 失敗を返す
			return E_FAIL;
		}

		if (m_apStun[nCnt] != nullptr)
		{ // ポインタが NULL じゃない場合

			// 初期化処理
			if (FAILED(m_apStun[nCnt]->Init()))
			{ // 初期化処理に失敗した場合

				// 停止
				assert(false);

				// 失敗を返す
				return E_FAIL;
			}

			// テクスチャの割り当て処理
			m_apStun[nCnt]->BindTexture(CManager::Get()->GetTexture()->Regist(STUN_TEXTURE));
		}
		else
		{ // ポインタが　NULL の場合

			// 停止
			assert(false);

			// 失敗を返す
			return E_FAIL;
		}
	}

	// 全ての値を初期化する
	m_pos = NONE_D3DXVECTOR3;		// 位置
	m_fRot = 0.0f;					// 回転用の向き

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CStun::Uninit(void)
{
	// 全ての値を初期化する
	for (int nCnt = 0; nCnt < MAX_STUN; nCnt++)
	{
		if (m_apStun[nCnt] != nullptr)
		{ // 能力UIが NULL じゃない場合

			// 終了処理
			m_apStun[nCnt]->Uninit();
			m_apStun[nCnt] = nullptr;
		}
	}

	// 本体の終了処理
	Release();
}

//========================
// 更新処理
//========================
void CStun::Update(void)
{
	// 向きを加算する
	m_fRot += STUN_ADD_ROT;

	// 向きの正規化処理
	useful::RotNormalize(&m_fRot);

	// ローカル変数宣言
	D3DXVECTOR3 posCorrect;		// 補正後の位置
	float fRot;					// 向き

	for (int nCnt = 0; nCnt < MAX_STUN; nCnt++)
	{
		if (m_apStun[nCnt] != nullptr)
		{ // 番号のポインタが NULL じゃない場合

			// 向きを設定する
			fRot = m_fRot + STUN_SHIFT_ROT * (nCnt);

			// 向きの正規化処理
			useful::RotNormalize(&fRot);

			// 補正用の位置を設定する
			posCorrect.x = sinf(fRot) * STUN_SHIFT_WIDTH;
			posCorrect.y = 0.0f;
			posCorrect.z = cosf(fRot) * STUN_SHIFT_WIDTH;

			// 位置を設定する
			m_apStun[nCnt]->SetPos(m_pos + posCorrect);

			// 頂点情報の設定処理
			m_apStun[nCnt]->SetVertex();
		}
	}
}

//========================
// 描画処理
//========================
void CStun::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_STUN; nCnt++)
	{
		if (m_apStun[nCnt] != nullptr)
		{ // 情報が NULL じゃない場合

			// 描画処理
			m_apStun[nCnt]->DrawLightOff();
		}
	}
}

//========================
// 情報の設定処理
//========================
void CStun::SetData(const D3DXVECTOR3 pos)
{
	// ローカル変数宣言
	D3DXVECTOR3 posCorrect;		// 補正後の位置
	float fRot;					// 向き

	// 全ての値を設定する
	m_pos = pos;		// 位置を設定する
	m_fRot = 0.0f;		// 回転用の向き

	for (int nCnt = 0; nCnt < MAX_STUN; nCnt++)
	{
		// 向きを設定する
		fRot = STUN_SHIFT_ROT * (nCnt);

		// 向きの正規化処理
		useful::RotNormalize(&fRot);

		// 補正用の位置を設定する
		posCorrect.x = sinf(fRot) * STUN_SHIFT_WIDTH;
		posCorrect.y = 0.0f;
		posCorrect.z = cosf(fRot) * STUN_SHIFT_WIDTH;

		if (m_apStun[nCnt] != nullptr)
		{ // 番号のポインタが NULL じゃない場合

			// 情報を設定する
			m_apStun[nCnt]->SetPos(m_pos + posCorrect);	// 位置設定
			m_apStun[nCnt]->SetSize(STUN_SIZE);			// サイズ設定

			// 頂点情報の設定処理
			m_apStun[nCnt]->SetVertex();
		}
	}
}

//========================
// 位置の設定処理
//========================
void CStun::SetPos(const D3DXVECTOR3& pos)
{
	// 位置を設定する
	m_pos = pos;
}

//========================
// 生成処理
//========================
CStun* CStun::Create(const D3DXVECTOR3 pos)
{
	// ローカルオブジェクトを生成
	CStun* pStun = nullptr;	// プレイヤーのインスタンスを生成

	if (pStun == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pStun = new CStun;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pStun != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pStun->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pStun->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 気絶演出のポインタを返す
	return pStun;
}