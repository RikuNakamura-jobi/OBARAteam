//=======================================
//
// 危険マークのメイン処理[danger_mark.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "danger_mark.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//=======================================
// 無名名前関数
//=======================================
namespace
{
	const char* DANGER_TEXTURE[CDangerMark::TYPE_MAX] =		// 危険マークの
	{
		"data\\TEXTURE\\Platform\\danger001.png",
	};
	const float LIGHT_FLASH_MAX_ALPHA = 0.5f;		// 軽点滅の透明度の最大値
	const float LIGHT_FLASH_MIN_ALPHA = 0.2f;		// 軽点滅の透明度の最小値
	const float LIGHT_FLASH_ADD_ALPHA = 0.01f;		// 軽点滅の透明度の加算数
}

//=========================
// コンストラクタ
//=========================
CDangerMark::CDangerMark() : CBillboard(CObject::TYPE_DANGERMARK, CObject::PRIORITY_BLOCK)
{
	// 全ての値をクリアする
	m_col = NONE_D3DXCOLOR;		// 色
	m_fAlphaDest = 0.0f;		// 目的の透明度
	m_bDisp = true;				// 描画状況
}

//=========================
// デストラクタ
//=========================
CDangerMark::~CDangerMark()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CDangerMark::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_col = NONE_D3DXCOLOR;		// 色
	m_fAlphaDest = 0.0f;		// 目的の透明度
	m_bDisp = true;				// 描画状況

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CDangerMark::Uninit(void)
{
	// 終了
	CBillboard::Uninit();
}

//=========================
// 更新処理
//=========================
void CDangerMark::Update(void)
{
	if (useful::FrameCorrect(m_fAlphaDest, &m_col.a, LIGHT_FLASH_ADD_ALPHA) == true)
	{ // 目的の値に達した場合

		// 目的の透明度を設定する
		m_fAlphaDest = (m_fAlphaDest >= LIGHT_FLASH_MAX_ALPHA) ? LIGHT_FLASH_MIN_ALPHA : LIGHT_FLASH_MAX_ALPHA;
	}

	// 頂点情報の初期化
	SetVertex();

	// 色の設定処理
	SetVtxColor(m_col);
}

//=========================
// 描画処理
//=========================
void CDangerMark::Draw(void)
{
	if (m_bDisp == true)
	{ // 描画状況が true の場合

		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		//αブレンディングを加算処理に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		// 描画処理
		CBillboard::DrawLightOff();

		//αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//=========================
// 情報の設定処理
//=========================
void CDangerMark::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col, const TYPE type)
{
	// スクロールの設定処理
	SetPos(pos);	// 位置
	SetPosOld(pos);	// 前回の位置
	SetSize(size);	// サイズ

	// 全ての値を設定する
	m_col = col;							// 色
	m_fAlphaDest = LIGHT_FLASH_MIN_ALPHA;	// 目的の透明度
	m_bDisp = true;							// 描画状況

	// テクスチャの割り当て処理
	BindTexture(CManager::Get()->GetTexture()->Regist(DANGER_TEXTURE[type]));

	// 頂点情報の初期化
	SetVertex();

	// 色の設定処理
	SetVtxColor(m_col);
}

//=========================
// 生成処理
//=========================
CDangerMark* CDangerMark::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col, const TYPE type)
{
	// ローカルオブジェクトを生成
	CDangerMark* pDanger = nullptr;		// 危険マークのインスタンスを生成

	if (pDanger == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pDanger = new CDangerMark;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pDanger != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pDanger->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pDanger->SetData(pos, size, col, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 危険マークのポインタを返す
	return pDanger;
}