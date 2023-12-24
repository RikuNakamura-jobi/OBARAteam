//=======================================
//
//フェードのメイン処理[Fade.cpp]
//Author 小原立暉
//
//=======================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "Fade.h"
#include "debugproc.h"

//マクロ定義
#define FADE_ADD_ALPHA			(0.05f)									// フェード処理の追加するα値の数値
#define FADE_COLOR				(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))		// フェードの色

//==========================================
// コンストラクタ
//==========================================
CFade::CFade()
{
	// 全ての値をクリアする
	m_pVtxBuff = nullptr;				//フェードの頂点バッファへのポインタ
	m_fade = FADE_NONE;					// フェードの状態
	m_modeNext = CScene::MODE_LOGO;		// 次の画面(モード)
	m_color = FADE_COLOR;				// ポリゴン(フェード)の色
}

//==========================================
// デストラクタ
//==========================================
CFade::~CFade()
{

}

//==========================================
// フェードの初期化処理
//==========================================
HRESULT CFade::Init(CScene::MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// 全ての値を初期化する
	m_fade = FADE_IN;				// フェード状態
	m_modeNext = modeNext;			// 次の画面(モード)
	m_color = FADE_COLOR;			// ポリゴンの色

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = m_color;
	pVtx[1].col = m_color;
	pVtx[2].col = m_color;
	pVtx[3].col = m_color;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// モードの設定
	CManager::Get()->SetMode(m_modeNext);

	// 成功を返す
	return S_OK;
}

//========================================
// フェードの終了処理
//========================================
void CFade::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{			
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//========================================
// フェードの更新処理
//========================================
void CFade::Update(void)
{
	VERTEX_2D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{ // フェードイン状態

			// ポリゴンを透明にしていく
			m_color.a -= FADE_ADD_ALPHA;

			if (m_color.a <= 0.0f)
			{ // 透明度が0.0fを超えたら

				// 透明度を0.0fにする
				m_color.a = 0.0f;

				// 何もしていない状態にする
				m_fade = FADE_NONE;
			}
		}
		else if (m_fade == FADE_OUT)
		{ // フェードアウト状態

			// ポリゴンを不透明にしていく
			m_color.a += FADE_ADD_ALPHA;

			if (m_color.a >= 1.0f)
			{ // 透明度が1.0fを超えたら

				// 透明度を1.0fにする
				m_color.a = 1.0f;

				// フェードイン状態にする
				m_fade = FADE_IN;

				// モード設定(次の画面に移行)
				CManager::Get()->SetMode(m_modeNext);
			}
		}

		// 頂点カラーの設定
		pVtx[0].col = m_color;
		pVtx[1].col = m_color;
		pVtx[2].col = m_color;
		pVtx[3].col = m_color;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// デバッグ表示
	CManager::Get()->GetDebugProc()->Print("フェードの状態：%d\nフェードの色：%f\n", m_fade, m_color.a);
}

//=====================================
// フェードの描画処理
//=====================================
void CFade::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0,
		m_pVtxBuff,								// 頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D));						// 頂点情報構造体のサイズ

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, nullptr);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,											// 描画する最初の頂点インデックス
		2);											// 描画するプリミティブ数
}

//=====================================
// フェードの設定処理
//=====================================
void CFade::SetFade(CScene::MODE modeNext)
{
	if (CManager::Get()->GetFade()->GetFade() == CFade::FADE_NONE)
	{ // フェード状態が NONE だった場合

		// フェードアウト状態にする
		m_fade = FADE_OUT;

		// 次の画面(モード)を設定
		m_modeNext = modeNext;

		// 黒いポリゴン(透明)にしておく
		m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

//=====================================
// フェードの取得処理
//=====================================
CFade::FADE CFade::GetFade(void)
{
	// フェードの変数を返す
	return m_fade;
}

//=====================================
// 静的メンバ変数
//=====================================
CFade* CFade::Create(CScene::MODE modeNext)
{
	// ローカルオブジェクトを生成
	CFade* pFade = nullptr;	// プレイヤーのインスタンスを生成

	if (pFade == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pFade = new CFade;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pFade != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pFade->Init(modeNext)))
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

	// フェードのポインタを返す
	return pFade;
}