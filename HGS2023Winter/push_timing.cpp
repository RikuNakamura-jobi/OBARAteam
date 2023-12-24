//============================================
//
// 押しタイミングの表示処理[push_timing.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "push_timing.h"
#include "input.h"
#include "texture.h"

//--------------------------------------------
// 無名名前空間
//--------------------------------------------
namespace
{
	const char* TEXTURE[CPushTiming::TYPE_MAX] =		// テクスチャ
	{
		"data\\TEXTURE\\Platform\\PushPadA.png",		// Aボタンのテクスチャ
		"data\\TEXTURE\\Platform\\PushKeySpace.png",	// SPACEキーのテクスチャ
	};
	const D3DXVECTOR3 TYPE_SIZE[CPushTiming::TYPE_MAX] =	// サイズ
	{
		D3DXVECTOR3(30.0f, 30.0f, 0.0f),		// Aボタンのサイズ
		D3DXVECTOR3(60.0f, 30.0f, 0.0f),		// SPACEキーのサイズ
	};
	const int ANIM_COUNT = 6;			// アニメーションカウント
	const int ANIM_PATTERN = 2;			// アニメーションパターン
}

//===========================================
// コンストラクタ
//===========================================
CPushTiming::CPushTiming() : CBillboardAnim(CObject::TYPE_PUSHTIMING,CObject::PRIORITY_UI)
{

}

//===========================================
// デストラクタ
//===========================================
CPushTiming::~CPushTiming()
{

}

//===========================================
// 初期化処理
//===========================================
HRESULT CPushTiming::Init(void)
{
	if (FAILED(CBillboardAnim::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CPushTiming::Uninit(void)
{
	// 終了
	CBillboardAnim::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CPushTiming::Update(void)
{
	// 更新処理
	CBillboardAnim::Update();
}

//===========================================
// 描画処理
//===========================================
void CPushTiming::Draw(void)
{
	// 描画処理
	CBillboardAnim::DrawLightOff();
}

//===========================================
// 設定処理
//===========================================
void CPushTiming::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// スクロールの設定処理
	SetPos(pos);				// 位置設定
	SetSize(TYPE_SIZE[type]);	// サイズ設定

	// アニメーションの設定処理
	SetAnim(ANIM_COUNT, ANIM_PATTERN);

	// 頂点情報の初期化
	SetVertex();

	// テクスチャ情報の設定処理
	SetVtxAnim(GetTexPattern(), GetPattern());

	// テクスチャの読み込み処理
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[type]));
}

//===========================================
// 生成処理
//===========================================
CPushTiming* CPushTiming::Create(const D3DXVECTOR3& pos, const TYPE type)
{
	// ローカルオブジェクトを生成
	CPushTiming* pPushTiming = nullptr;			// 押しタイミングのインスタンスを生成

	if (pPushTiming == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pPushTiming = new CPushTiming;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pPushTiming != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pPushTiming->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pPushTiming->SetData(pos, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// 押しタイミングのポインタを返す
	return pPushTiming;
}

//===========================================
// 種類の設定処理
//===========================================
void CPushTiming::SetType(const TYPE type)
{
	// テクスチャの読み込み処理
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[type]));

	// サイズの設定処理
	SetSize(TYPE_SIZE[type]);

	// 頂点座標の設定処理
	SetVertex();
}