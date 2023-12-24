//=========================================
//
//入力のメイン処理[input.cpp]
//Author 小原立暉
//
//=========================================
#include "manager.h"
#include "debugproc.h"
#include "input.h"

//-----------------------------------------
// 静的メンバ変数宣言
//-----------------------------------------
LPDIRECTINPUT8 CInput::m_pInput = nullptr;				// DirectInputオブジェクトへのポインタ

//======================================================ここからCInputの処理======================================================

//==================================
// コンストラクタ
//==================================
CInput::CInput()
{
	// 全ての値をクリアする
	m_pDevice = nullptr;								// デバイスへのポインタ
}

//==================================
// デストラクタ
//==================================
CInput::~CInput()
{

}

//==================================
// 初期化処理
//==================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == nullptr)
	{ // オブジェクトへのポインタが NULL の場合

		// DirectInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{ // オブジェクトの生成に失敗した場合

			// 警告文
			MessageBox(NULL, "DirectInputオブジェクトの生成に失敗！", "警告！", MB_ICONWARNING);

			// 停止
			assert(false);

			// 失敗を返す
			return E_FAIL;
		}
	}

	// 成功を返す
	return S_OK;
}

//==================================
// 終了処理
//==================================
void CInput::Uninit(void)
{
	//入力デバイスの破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();					//キーボードへのアクセス権を放棄

		m_pDevice->Release();

		m_pDevice = nullptr;
	}

	//DirectInputオブジェクトの放棄
	if (m_pInput != nullptr)
	{
		m_pInput->Release();

		m_pInput = nullptr;
	}
}

//======================================================ここからCInputKeyboardの処理======================================================

//==================================
// コンストラクタ
//==================================
CInputKeyboard::CInputKeyboard()
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
	{
		// キー情報
		ZeroMemory(&m_aKeyState[nCnt], sizeof(BYTE));
	}
}

//==================================
// デストラクタ
//==================================
CInputKeyboard::~CInputKeyboard()
{

}

//==================================
// 初期化処理
//==================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// CInputの初期化処理
	CInput::Init(hInstance, hWnd);

	//入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{ // 生成に失敗した場合

		// 警告文
		MessageBox(NULL, "キーボードの入力デバイスの生成に失敗！", "警告！", MB_ICONWARNING);

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{ // 設定に失敗した場合

		// 警告文
		MessageBox(NULL, "キーボードのデータフォーマット設定に失敗！", "警告！", MB_ICONWARNING);

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{ // 設定に失敗した場合

		// 警告文
		MessageBox(NULL, "キーボードの協調モード設定に失敗！", "警告！", MB_ICONWARNING);

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	// 成功を返す
	return S_OK;
}

//==================================
// 終了処理
//==================================
void CInputKeyboard::Uninit(void)
{
	// CInputの終了処理
	CInput::Uninit();
}

//==================================
// 更新処理
//==================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];						//キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{ // 取得に成功した場合

		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		//キーボードのトリガー情報を保存

			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];	//キーボードのリリース情報を保存

			m_aKeyState[nCntKey] = aKeyState[nCntKey];			//キーボードのプレス情報を保存
		}
	}
	else
	{ // 取得に失敗した場合

		//キーボードへのアクセス権を獲得
		m_pDevice->Acquire();
	}
}

//======================================
// プレス情報を獲得
//======================================
bool CInputKeyboard::GetPress(int nKey)
{
	// プレス状況を返す
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}

//======================================
// トリガー処理
//======================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	// トリガー状況を返す
	return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//======================================
// リリース処理
//======================================
bool CInputKeyboard::GetRelease(int nKey)
{
	// リリース状況を返す
	return(m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//======================================
// リピート処理
//======================================
bool CInputKeyboard::GetRepeat(int nKey, int nCount)
{
	if (CInputKeyboard::GetPress(nKey) == true)
	{ // キーを押している場合

		// カウントを加算する
		m_nRepeatCount[nKey]++;
	}
	else
	{ // キーを押していない場合

		// カウントを加算する
		m_nRepeatCount[nKey] = 0;
	}

	if (m_nRepeatCount[nKey] >= nCount)
	{ // リピートカウントがカウント数を超えた場合

		// カウントを0にする
		m_nRepeatCount[nKey] = 0;

		// true を返す
		return true;
	}
	else
	{ // 上記以外

		// false を返す
		return false;
	}
}

//======================================================ここからCInputGamePadの処理======================================================

//==================================
// コンストラクタ
//==================================
CInputGamePad::CInputGamePad()
{
	// 全ての値をクリアする
	ZeroMemory(&m_aPadState[0], sizeof(m_aPadState));					// プレス情報
	ZeroMemory(&m_aPadStateTrigger[0], sizeof(m_aPadStateTrigger));		// トリガー情報
	ZeroMemory(&m_aPadStateRelease[0], sizeof(m_aPadStateRelease));		// リリース情報
	ZeroMemory(&m_aPadVibration[0], sizeof(m_aPadVibration));			// バイブレーション情報
	m_bConnect = false;													// 接続判定
}

//==================================
// デストラクタ
//==================================
CInputGamePad::~CInputGamePad()
{

}

//==================================
// 初期化処理
//==================================
HRESULT CInputGamePad::Init(HINSTANCE hInstance, HWND hWnd)
{
	// CInputの初期化処理
	CInput::Init(hInstance, hWnd);

	// ゲームパッド設定を有効にする
	XInputEnable(true);

	// ゲームパッドのプレス情報を用意する
	memset(&m_aPadState[0], 0, sizeof(m_aPadState));

	// ゲームパッドのトリガー情報を用意する
	memset(&m_aPadStateTrigger[0], 0, sizeof(m_aPadStateTrigger));

	// ゲームパッドのリリース情報を用意する
	memset(&m_aPadStateRelease[0], 0, sizeof(m_aPadStateRelease));

	// バイブレーション情報を用意する
	memset(&m_aPadVibration[0], 0, sizeof(m_aPadVibration));

	// 接続判定を初期化する
	m_bConnect = false;

	// 失敗を返す
	return S_OK;
}

//==================================
// 終了処理
//==================================
void CInputGamePad::Uninit(void)
{
	// CInputの終了処理
	CInput::Uninit();

	// 接続判定を false にする
	m_bConnect = false;

	//ゲームパッド設定を無効にする
	XInputEnable(false);
}

//==================================
// 更新処理
//==================================
void CInputGamePad::Update(void)
{
	XINPUT_STATE aGamePadState[MAX_PLAYER];			// ゲームパッドの入力情報
	bool bConnect = false;							// 接続されているかどうか

	for (int nCntGP = 0; nCntGP < MAX_PLAYER; nCntGP++)
	{//それぞれのゲームパッド分設定する

		//入力デバイスからデータを取得
		if (XInputGetState(nCntGP, &aGamePadState[nCntGP]) == ERROR_SUCCESS)
		{
			//ゲームパッドのトリガー情報を保存
			m_aPadStateTrigger[nCntGP].Gamepad.wButtons = ~m_aPadState[nCntGP].Gamepad.wButtons & aGamePadState[nCntGP].Gamepad.wButtons;

			//ゲームパッドのリリース情報を保存
			m_aPadStateRelease[nCntGP].Gamepad.wButtons = (m_aPadState[nCntGP].Gamepad.wButtons ^ aGamePadState[nCntGP].Gamepad.wButtons) & ~aGamePadState[nCntGP].Gamepad.wButtons;

			//ゲームパッドのプレス情報を保存
			m_aPadState[nCntGP] = aGamePadState[nCntGP];

			// 接続されている
			bConnect = true;
		}
	}

	// 接続判定を代入する
	m_bConnect = bConnect;
}

//======================================
// プレス情報を獲得
//======================================
bool CInputGamePad::GetPress(JOYKEY nKey, int nPlayer)
{
	return(m_aPadState[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//======================================
// トリガー情報処理
//======================================
bool CInputGamePad::GetTrigger(JOYKEY nKey, int nPlayer)
{
	return(m_aPadStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//======================================
// リリース情報処理
//======================================
bool CInputGamePad::GetRelease(JOYKEY nKey, int nPlayer)
{
	return(m_aPadStateRelease[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//======================================
// 右のバイブレーション処理
//======================================
void CInputGamePad::GetRightVibration(int nPlayer, const bool bSwitch)
{
	if (bSwitch == true)
	{ // Switchが ON の場合

		// 右のバイブレーションを ON にする
		m_aPadVibration[nPlayer].wRightMotorSpeed = 65535;
	}
	else
	{ // Switchが OFF の場合

		// 右のバイブレーションを OFF にする
		m_aPadVibration[nPlayer].wRightMotorSpeed = 0;
	}

	// バイブレーションの情報を渡す
	XInputSetState(nPlayer, &m_aPadVibration[nPlayer]);
}

//======================================
// 左のバイブレーション処理
//======================================
void CInputGamePad::GetLeftVibration(int nPlayer, const bool bSwitch)
{
	if (bSwitch == true)
	{ // Switchが ON の場合

		// 左のバイブレーションを ON にする
		m_aPadVibration[nPlayer].wLeftMotorSpeed = 65535;
	}
	else
	{ // Switchが OFF の場合

		// 左のバイブレーションを OFF にする
		m_aPadVibration[nPlayer].wLeftMotorSpeed = 0;
	}

	// バイブレーションの情報を渡す
	XInputSetState(nPlayer, &m_aPadVibration[nPlayer]);
}

//======================================
// 接続されている場合
//======================================
bool CInputGamePad::GetConnect(void)
{
	// 接続判定を返す
	return m_bConnect;
}

//======================================
//Lスティック(X軸)入力のプレス情報処理
//======================================
SHORT CInputGamePad::GetGameStickLXPress(int nPlayer)
{
	return m_aPadState[nPlayer].Gamepad.sThumbLX;
}

//======================================
//Lスティック(Y軸)入力のプレス情報処理
//======================================
SHORT CInputGamePad::GetGameStickLYPress(int nPlayer)
{
	return m_aPadState[nPlayer].Gamepad.sThumbLY;
}

//======================================
//Rスティック(X軸)入力のプレス情報処理
//======================================
SHORT CInputGamePad::GetGameStickRXPress(int nPlayer)
{
	return m_aPadState[nPlayer].Gamepad.sThumbRX;
}

//======================================
//Rスティック(Y軸)入力のプレス情報処理
//======================================
SHORT CInputGamePad::GetGameStickRYPress(int nPlayer)
{
	return m_aPadState[nPlayer].Gamepad.sThumbRY;
}

//======================================================ここからCInputMouseの処理======================================================

//======================================
// コンストラクタ
//======================================
CInputMouse::CInputMouse()
{
	// 全ての値をクリアする
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
}

//======================================
// デストラクタ
//======================================
CInputMouse::~CInputMouse()
{

}

//======================================
// 初期化処理
//======================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// CInputの初期化処理
	CInput::Init(hInstance, hWnd);

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{ // 生成に失敗した場合

		// 警告文
		MessageBox(NULL, "マウスの入力デバイスの生成に失敗！", "警告！", MB_ICONWARNING);

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{ // 設定に失敗した場合

		// 警告文
		MessageBox(NULL, "マウスのデータフォーマット設定に失敗！", "警告！", MB_ICONWARNING);

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{ // 設定に失敗した場合

		// 警告文
		MessageBox(NULL, "マウスの協調モード設定に失敗！", "警告！", MB_ICONWARNING);

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// マウスへのアクセス権を獲得
	if (FAILED(m_pDevice->Acquire()))
	{ // 設定に失敗した場合

		// 警告文
		MessageBox(NULL, "マウスのアクセス権の取得に失敗！", "警告！", MB_ICONWARNING);

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// ポーリング開始
	if (FAILED(m_pDevice->Poll()))
	{ // 設定に失敗した場合

		// 警告文
		MessageBox(NULL, "マウスのポーリングに失敗！", "警告！", MB_ICONWARNING);

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));

	// 失敗を返す
	return S_OK;
}

//======================================
// 終了処理
//======================================
void CInputMouse::Uninit(void)
{
	// 終了処理
	CInput::Uninit();
}

//======================================
// 更新処理
//======================================
void CInputMouse::Update(void)
{
	// マウスの状態を取得する
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState)))
	{ // 取得に成功した場合

	}
}