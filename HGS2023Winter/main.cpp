//=========================================
//
// テキストの描画[main.cpp]
// Author 小原立暉
//
//=========================================
#include "main.h"
#include "manager.h"

// マクロ定義
#define CLASS_NAME					"WindowClass"				// ウインドウクラスの名前
#define WINDOW_NAME					"Mechanism"					// ウインドウの名前(キャプションに表示)
#define ID_BUTTON_FINISH			(101)						// 終了ボタンのID

// プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//-----------------------------------------
// グローバル変数
//-----------------------------------------
int g_nCountFPS = 0;

//=========================================
//メイン関数
//=========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hInstancePrev*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
#ifdef _DEBUG

	// 終了時にメモリリークを検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif 

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),										//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,												//ウインドウのスタイル
		WindowProc,												//ウインドウプロシージャ
		0,														//0にする(通常は使用しない)
		0,														//0にする(通常は使用しない)
		hInstance,												//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),							//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),								//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),								//クライアント領域の背景色
		NULL,													//メニューバー
		CLASS_NAME,												//ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)							//ファイルのアイコン
	};
	HWND hWnd;													//ウインドウハンドル(識別子)
	MSG msg;													//メッセージを格納する変数

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };				//画面サイズの構造体

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	DWORD dwCurrentTime;										//現在時刻
	DWORD dwExecLastTime;										//最後に処理した時刻
	DWORD dwFrameCount;											//フレームカウント
	DWORD dwFPSLastTime;										//最後にFPSを計測した時刻

	//ウインドウを生成
	hWnd = CreateWindowEx(0,									//拡張ウインドウスタイル
		CLASS_NAME,												//ウインドウクラスの名前
		WINDOW_NAME,											//ウインドウの名前
		WS_OVERLAPPEDWINDOW,									//ウインドウスタイル
		CW_USEDEFAULT,											//ウインドウの左上X座標
		CW_USEDEFAULT,											//ウインドウの左下Y座標
		(rect.right - rect.left),								//ウインドウの幅
		(rect.bottom - rect.top),								//ウインドウの高さ
		NULL,													//親ウインドウのハンドル
		NULL,													//メニューハンドルまたは子ウインドウID
		hInstance,												//インスタンスハンドル
		NULL);													//ウインドウ作成データ

	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;											//初期化する
	dwExecLastTime = timeGetTime();								//現在時刻を取得(保存)
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);									//ウインドウの表示状態を設定
	UpdateWindow(hWnd);											//クライアント領域を更新

	//ランダムの種を設定
	srand((unsigned int)time(0));

	// 生成処理
	CManager::Create();

	if (CManager::Get() != nullptr)
	{ // マネージャーが nullptr じゃない場合

		// マネージャーの初期化
		CManager::Get()->Init(hInstance, hWnd, TRUE);
	}
	else
	{ // 上記以外

		// 失敗を返す
		return -1;
	}

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理

			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける

				//抜け出す
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);							//仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);							//ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{//DirectXの処理

			dwCurrentTime = timeGetTime();						//現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{ // 0.5秒経過

				// FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				//FPSを測定した時刻を保存
				dwFPSLastTime = dwCurrentTime;

				// フレームカウントをクリア
				dwFrameCount = 0;
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{ // 60分の1秒経過

				// 処理開始の時刻[現在時刻]を保存
				dwExecLastTime = dwCurrentTime;

				if (CManager::Get() != nullptr)
				{ // 確保に成功していた場合

					// マネージャーの更新
					CManager::Get()->Update();

					// マネージャーの描画
					CManager::Get()->Draw();

					//フレームカウントを加算
					dwFrameCount++;
				}
			}
		}
	}

	// レンダラーの破棄
	if (CManager::Get() != nullptr)
	{ // マネージャーが nullptr じゃない場合

		// マネージャーの終了処理
		CManager::Get()->Uninit();
	}

	//分解能を戻す
	timeEndPeriod(1);

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=========================================
//ウインドウプロシージャ
//=========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;							//返り値を格納する変数を設定する

	switch (uMsg)
	{
	case WM_DESTROY:					//ウィンドウ破棄のメッセージ

		//WM_QUITメッセージを送る
		PostQuitMessage(0);

		//抜け出す
		break;

	case WM_KEYDOWN:											//キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:											//[ESKキー]が押された

			//終了メッセージを表示
			nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{//「はい」を選んだ場合

				//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			else
			{
				return 0;										//0を返さないと終了してしまう
			}

			//抜け出す
			break;
		}
		//抜け出す
		break;

	case WM_CLOSE:											//コマンド発行のメッセージ

		//終了メッセージを表示
		nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{//「はい」を選んだ場合

			//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;											//0を返さないと終了してしまう
		}

		//抜け出す
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);			//規定の処理を返す
}

//=========================================
// FPSの取得処理
//=========================================
int GetFPS(void)
{
	// FPSを返す
	return g_nCountFPS;
}