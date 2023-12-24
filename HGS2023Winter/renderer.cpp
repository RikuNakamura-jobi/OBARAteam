//============================================
//
// レンダラーのメイン処理[renderer.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "object.h"
#include "fade.h"

//=========================================
// コンストラクタ
//=========================================
CRenderer::CRenderer()
{
	// 全ての値をクリアする
	m_pD3D = nullptr;			// オブジェクトへのポインタ
	m_pD3DDevice = nullptr;	// デバイスへのポインタ
}

//=========================================
// デストラクタ
//=========================================
CRenderer::~CRenderer()
{

}

//=========================================
//初期化処理
//=========================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;										//ディスプレイモード

	D3DPRESENT_PARAMETERS d3dpp;								//プレゼンテーションパラメータ

	//Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
	{ // Direct3Dオブジェクトの生成に失敗した場合

		// 警告文
		MessageBox(NULL, "Direct3Dオブジェクトの生成に失敗！", "警告！", MB_ICONWARNING);

		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		// 警告文
		MessageBox(NULL, "ディスプレイモードの取得に失敗！", "警告！", MB_ICONWARNING);

		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							//パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						//ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						//ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						//バックバッファの形式
	d3dpp.BackBufferCount = 1;									//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;						//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;									//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//インターバル

	//Direct3Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,			//描画処理と頂点処理をハードウェアで行う
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,		//描画処理をハードウェアで行い、頂点処理をCPUで行う
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,	//描画処理と頂点処理をCPUで行う
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				// 警告文
				MessageBox(NULL, "Direct3Dデバイスの生成に失敗！", "警告！", MB_ICONWARNING);

				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		// カリングの設定
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定 (テクスチャの拡縮補間の設定)
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定 (テクスチャのアルファブレンドの設定)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// 成功を返す
	return S_OK;
}

//=============================================
//終了処理
//=============================================
void CRenderer::Uninit(void)
{
	//Direct3Dデバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();

		m_pD3DDevice = nullptr;
	}

	//Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();

		m_pD3D = nullptr;
	}
}

//======================================
//更新処理
//======================================
void CRenderer::Update(void)
{
	// FPS表示
	CManager::Get()->GetDebugProc()->Print("FPS:%d\n", GetFPS());

	// 全ての更新処理
	CObject::UpdateAll();
}

//======================================
//描画処理
//======================================
void CRenderer::Draw(void)
{
	//画面クリア(バックバッファとZバッファのクリア)
	m_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		// 全ての描画処理
		CObject::DrawAll();

		if (CManager::Get()->GetFade() != nullptr)
		{ // フェードが NULL じゃない場合

			// フェードの描画
			CManager::Get()->GetFade()->Draw();
		}

		if (CManager::Get()->GetDebugProc() != nullptr)
		{ // デバッグ表示が NULL じゃない場合

			// デバッグ表示の描画
			CManager::Get()->GetDebugProc()->Draw();
		}

		//描画終了
		m_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//===========================================
//デバイスの取得
//===========================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}