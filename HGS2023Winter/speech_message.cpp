//=======================================
//
// 伝達メッセージのメイン処理[speech_message.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "speech_message.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//=======================================
// 無名名前空間
//=======================================
namespace
{
	const char* TEXTURE[CSpeechMessage::TYPE_MAX] =		// テクスチャ
	{
		"data\\TEXTURE\\Platform\\Help.png",			// 救助要請のテクスチャ
	};
	const int MESSAGE_NONE_LIFE = -1;		// 寿命無し(無限)の値
	const int EXTEND_COUNT = 50;			// 伸び状態のカウント数
	const int SHRINK_COUNT = 15;			// 縮み状態のカウント数
	const D3DXVECTOR3 EXTEND_MAGNI = D3DXVECTOR3(0.8f, 1.2f, 0.0f);		// 伸び状態の倍率
	const D3DXVECTOR3 SHRINK_MAGNI = D3DXVECTOR3(1.2f, 0.8f, 0.0f);		// 縮み状態の倍率
}

//=========================
// コンストラクタ
//=========================
CSpeechMessage::CSpeechMessage() : CBillboard(CObject::TYPE_EFFECT, CObject::PRIORITY_EFFECT)
{
	// 全ての値をクリアする
	m_sizeDest = NONE_D3DXVECTOR3;	// 目的のサイズ
	m_sizeCopy = NONE_D3DXVECTOR3;	// 最初のサイズ
	m_state = STATE_EXTEND;			// 状態
	m_type = TYPE_HELP;				// 種類
	m_nLife = 0;					// 寿命
	m_nStateCount = 0;				// 状態のカウント
}

//=========================
// デストラクタ
//=========================
CSpeechMessage::~CSpeechMessage()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CSpeechMessage::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_sizeDest = NONE_D3DXVECTOR3;	// 目的のサイズ
	m_sizeCopy = NONE_D3DXVECTOR3;	// 最初のサイズ
	m_state = STATE_EXTEND;			// 状態
	m_type = TYPE_HELP;				// 種類
	m_nLife = 0;					// 寿命
	m_nStateCount = 0;				// 状態のカウント

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CSpeechMessage::Uninit(void)
{
	// 終了
	CBillboard::Uninit();
}

//=========================
// 更新処理
//=========================
void CSpeechMessage::Update(void)
{
	D3DXVECTOR3 size = GetSize();

	// 状態カウントを加算する
	m_nStateCount++;

	switch (m_state)
	{
	case CSpeechMessage::STATE_EXTEND:

		// サイズを設定する
		useful::FrameCorrect(m_sizeDest.x, &size.x, (m_sizeDest.x - m_sizeCopy.x) / EXTEND_COUNT);
		useful::FrameCorrect(m_sizeDest.y, &size.y, (m_sizeDest.y - m_sizeCopy.y) / EXTEND_COUNT);

		if (m_nStateCount % EXTEND_COUNT == 0)
		{ // 状態カウントが一定数になった場合

			// 状態カウントを初期化する
			m_nStateCount = 0;

			// 縮小状態にする
			m_state = STATE_SHRINK;

			// 目的のサイズを設定する
			m_sizeDest = D3DXVECTOR3(m_sizeCopy.x * SHRINK_MAGNI.x, m_sizeCopy.y * SHRINK_MAGNI.y, 0.0f);
		}

		break;

	case CSpeechMessage::STATE_SHRINK:

		// サイズを設定する
		useful::FrameCorrect(m_sizeDest.x, &size.x, (m_sizeDest.x - m_sizeCopy.x) / SHRINK_COUNT);
		useful::FrameCorrect(m_sizeDest.y, &size.y, (m_sizeDest.y - m_sizeCopy.y) / SHRINK_COUNT);

		if (m_nStateCount % SHRINK_COUNT == 0)
		{ // 状態カウントが一定数になった場合

			// 状態カウントを初期化する
			m_nStateCount = 0;

			// 拡大状態にする
			m_state = STATE_EXTEND;

			// 目的のサイズを設定する
			m_sizeDest = D3DXVECTOR3(m_sizeCopy.x * EXTEND_MAGNI.x, m_sizeCopy.y * EXTEND_MAGNI.y, 0.0f);
		}

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 寿命を減算する
	m_nLife--;

	if (m_nLife == 0)
	{ // 寿命が0になった場合

		// 終了処理
		Uninit();

		// この先の処理を行わない
		return;
	}

	// サイズの設定処理
	SetSize(size);

	// 頂点座標の設定処理
	SetVertex();
}

//=========================
// 描画処理
//=========================
void CSpeechMessage::Draw(void)
{
	// 描画処理
	CBillboard::DrawLightOff();
}

//=========================
// 情報の設定処理
//=========================
void CSpeechMessage::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const int nLife, const TYPE type)
{
	// スクロールの設定処理
	SetPos(pos);			// 位置設定
	SetSize(size);			// サイズ設定

	// 全ての値を初期化する
	m_sizeDest = D3DXVECTOR3(size.x * EXTEND_MAGNI.x, size.y * EXTEND_MAGNI.y, 0.0f);	// 目的のサイズ
	m_sizeCopy = size;			// 最初のサイズ
	m_state = STATE_EXTEND;		// 状態
	m_type = type;				// 種類
	m_nLife = nLife;			// 寿命
	m_nStateCount = 0;			// 状態のカウント

	// 頂点情報の初期化
	SetVertex();

	// テクスチャの割り当て処理
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[m_type]));
}

//=========================
// 生成処理
//=========================
CSpeechMessage* CSpeechMessage::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const int nLife, const TYPE type)
{
	// ローカルオブジェクトを生成
	CSpeechMessage* pMessage = nullptr;	// プレイヤーのインスタンスを生成

	if (pMessage == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pMessage = new CSpeechMessage;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pMessage != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pMessage->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pMessage->SetData(pos, size, nLife, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 伝達メッセージのポインタを返す
	return pMessage;
}