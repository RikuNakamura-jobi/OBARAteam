//=======================================
//
// 波紋のメイン処理[ripple.cpp]
// Author：小原立暉
//
//=======================================
#include "ripple.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//---------------------------------------
// マクロ定義
//---------------------------------------
#define RIPPLE_INIT_SCALE			(D3DXVECTOR3(5.0f, 5.0f, 5.0f))			// 波紋の初期拡大率
#define RIPPLE_INIT_ALPHA			(1.0f)									// 波紋の初期透明度
#define RIPPLE_SUB_ALPHA			(0.05f)									// 波紋の透明度の減少量
#define RIPPLE_ADD_SCALE			(0.6f)									// 波紋の拡大率の加算量

//==========================================
// コンストラクタ
//==========================================
CRipple::CRipple() : CModel(CObject::TYPE_RIPPLE, CObject::PRIORITY_PLAYER)
{
	// 全ての値をクリアする
	m_fAlpha = 0.0f;			// 透明度
}

//==========================================
// デストラクタ
//==========================================
CRipple::~CRipple()
{

}

//==========================================
// 波紋の初期化処理
//==========================================
HRESULT CRipple::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_fAlpha = 0.0f;			// 透明度

	// 成功を返す
	return S_OK;
}

//========================================
// 波紋の終了処理
//========================================
void CRipple::Uninit(void)
{
	// 終了処理
	CModel::Uninit();
}

//========================================
// 波紋の更新処理
//========================================
void CRipple::Update(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 scale = GetScale();			// 拡大率を取得する

	// 透明度を減少させる
	m_fAlpha -= RIPPLE_SUB_ALPHA;

	// 拡大率を変化させる
	scale.x += RIPPLE_ADD_SCALE;
	scale.z += RIPPLE_ADD_SCALE;

	// 情報を更新する
	SetScale(scale);			// 拡大率を設定する

	if (m_fAlpha <= 0.0f)
	{ // 透明度が 0.0f 以下の場合

	  // 終了処理
		Uninit();

		// この先の処理を行わない
		return;
	}
}

//===========================================
// 波紋の描画処理
//===========================================
void CRipple::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);					//アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);				//アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);				//アルファテストの参照値設定

	// 描画処理(色反映型)
	CModel::Draw(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// アルファテストの有効/無効設定
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		// アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						// アルファテストの参照値設定
}

//===========================================
// 情報の設定処理
//===========================================
void CRipple::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// スクロールの設定処理
	SetPos(pos);								// 位置設定
	SetPosOld(pos);								// 前回の位置設定
	SetRot(rot);								// 向き設定
	SetScale(RIPPLE_INIT_SCALE);				// 拡大率
	SetFileData(CXFile::TYPE_RIPPLE);			// サイズ設定

	// 全ての値を初期化する
	m_fAlpha = RIPPLE_INIT_ALPHA;				// 透明度
}

//===========================================
// 生成処理
//===========================================
CRipple* CRipple::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ローカルオブジェクトを生成
	CRipple* pRipple = nullptr;	// フィールドのインスタンスを生成

	if (pRipple == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pRipple = new CRipple;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pRipple != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pRipple->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "波紋の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pRipple->SetData(pos, rot);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// 波紋のポインタを返す
	return pRipple;
}