//============================================
//
// 数値処理[number.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "renderer.h"
#include "number.h"

//============================
// コンストラクタ
//============================
CNumber::CNumber() : CObject2D(CObject::TYPE_NONE, CObject::PRIORITY_BG)
{
	// コンストラクタの箱
	Box();
}

//============================
// オーバーロードコンストラクタ
//============================
CNumber::CNumber(CObject::TYPE type, CObject::PRIORITY priority) : CObject2D(type, priority)
{
	// コンストラクタの箱
	Box();
}

//============================
// デストラクタ
//============================
CNumber::~CNumber()
{

}

//============================
// コンストラクタの箱
//============================
void CNumber::Box(void)
{
	// 全ての値をクリアする
	m_nNumber = 0;			// 数値
	m_type = TYPE_DECIMAL;	// 種類
}

//============================
// 初期化処理
//============================
HRESULT CNumber::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

	  // 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_nNumber = 0;			// 番号
	m_type = TYPE_DECIMAL;	// 種類

	// スクロールの設定処理
	SetPos(D3DXVECTOR3(400.0f, 300.0f, 0.0f));		// 位置設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));			// 向き設定
	SetSize(D3DXVECTOR3(20.0f, 40.0f, 0.0f));		// サイズ設定
	SetLength();				// 長さ設定
	SetAngle();					// 方向設定

	// 頂点情報の初期化
	SetVertex();

	// テクスチャ情報の設定処理
	SetVtxTextureAnim(NUMBER_TEXTURE_PATTERN, m_nNumber);

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CNumber::Uninit(void)
{
	// 終了
	CObject2D::Uninit();
}

//============================
// 更新処理
//============================
void CNumber::Update(void)
{
	// 頂点情報の設定処理
	SetVertex();

	// テクスチャ情報の設定処理
	SetVtxTextureAnim(NUMBER_TEXTURE_PATTERN, m_nNumber);
}

//============================
// 描画処理
//============================
void CNumber::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//============================
//生成処理
//============================
CNumber* CNumber::Create(void)
{
	// ローカルオブジェクトを生成
	CNumber* pNumber = nullptr;	// プレイヤーのインスタンスを生成

	if (pNumber == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pNumber = new CNumber;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pNumber != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pNumber->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pNumber;
}

//============================
// 種類の設定処理
//============================
void CNumber::SetType(const TYPE type)
{
	// 種類を代入する
	m_type = type;
}

//============================
// 種類の取得処理
//============================
CNumber::TYPE CNumber::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//============================
// 番号の設定処理
//============================
void CNumber::SetNumber(const int nNum)
{
	// 数値を代入する
	m_nNumber = nNum;
}

//============================
// 番号の取得処理
//============================
int CNumber::GetNumber(void) const
{
	// 数値を返す
	return m_nNumber;
}