//=======================================
//
//入力ヘッダー[input.h]
//Author 小原立暉
//
//=======================================
#ifndef _IMPUT_H_			//このマクロ定義がされていなかったら
#define _INPUT_H_			//2重インクルード防止のマクロを定義する

#include "main.h"
#include<Xinput.h>

#pragma comment(lib,"xinput.lib")

//---------------------------------------
// マクロ定義
//---------------------------------------
#define NUM_KEY_MAX			(256)			// キーの最大数
#define MAX_PLAYER			(4)				// プレイ出来る人数

// クラス定義(CInput)
class CInput
{
public:			// 誰でもアクセス出来る

	CInput();				// コンストラクタ
	~CInput();				// デストラクタ

	// メンバ関数
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);		// 初期化処理
	virtual void Uninit(void);									// 終了処理
	virtual void Update(void) = 0;								// 更新処理

protected:		// 自分と派生クラスのみアクセスできる

	// メンバ変数
	LPDIRECTINPUTDEVICE8 m_pDevice;				// 入力デバイスへのポインタ

	// 静的メンバ変数
	static LPDIRECTINPUT8 m_pInput;				// DirectInputオブジェクトへのポインタ
};

// キーボードクラス
class CInputKeyboard : public CInput
{
public:			// 誰でもアクセスできる

	CInputKeyboard();		// コンストラクタ
	~CInputKeyboard();		// デストラクタ

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	bool GetPress(int nKey);							// プレス処理
	bool GetTrigger(int nKey);							// トリガー処理
	bool GetRelease(int nKey);							// リリース処理
	bool GetRepeat(int nKey, int nCount);				// リピート処理

private:		// 自分のみアクセスできる

	// メンバ変数
	BYTE m_aKeyState[NUM_KEY_MAX];						// プレス情報
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];				// トリガー情報
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];				// リリース情報
	int m_nRepeatCount[NUM_KEY_MAX];					// リピートカウント
};

// ジョイパッドクラス
class CInputGamePad : public CInput
{
public:			// 誰でもアクセスできる

	//ゲームパッドのボタン
	enum JOYKEY
	{
		JOYKEY_UP = 0,			//上ボタン
		JOYKEY_DOWN,			//下ボタン
		JOYKEY_LEFT,			//左ボタン
		JOYKEY_RIGHT,			//右ボタン
		JOYKEY_START,			//STARTボタン
		JOYKEY_BACK,			//BACKボタン
		JOYKEY_LEFTSTICK,		//左のスティック押し込み
		JOYKEY_RIGHTSTICK,		//右のスティック押し込み
		JOYKEY_LB,				//LBボタン
		JOYKEY_RB,				//RBボタン
		JOYKEY_PPP1,			//不明
		JOYKEY_PPP2,			//不明
		JOYKEY_A,				//Aボタン
		JOYKEY_B,				//Bボタン
		JOYKEY_X,				//Xボタン
		JOYKEY_Y,				//Yボタン
		JOYKEY_STICKLX,			//Lスティック(X軸)(右に行くほど値は大きい)
		JOYKEY_STICKLY,			//Lスティック(Y軸)(上に行くほど値は大きい)
		JOYKEY_STICKRX,			//Lスティック(X軸)(右に行くほど値は大きい)
		JOYKEY_STICKRY,			//Lスティック(Y軸)(上に行くほど値は大きい)
		JOYKEY_MAX
	};

	CInputGamePad();		// コンストラクタ
	~CInputGamePad();		// デストラクタ

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	bool GetPress(JOYKEY nKey, int nPlayer);		// プレス処理
	bool GetTrigger(JOYKEY nKey, int nPlayer);		// トリガー情報処理
	bool GetRelease(JOYKEY nKey, int nPlayer);		// リリース情報処理
	void GetRightVibration(int nPlayer, const bool bSwitch);		// 右のバイブレーション処理
	void GetLeftVibration(int nPlayer, const bool bSwitch);			// 左のバイブレーション処理
	bool GetConnect(void);							// 接続判定の取得処理

	// 左スティック関係
	SHORT GetGameStickLXPress(int nPlayer);			// Lスティック(X軸)入力のプレス情報処理
	SHORT GetGameStickLYPress(int nPlayer);			// Lスティック(Y軸)入力のプレス情報処理

	// 右スティック関係
	SHORT GetGameStickRXPress(int nPlayer);			// Rスティック(X軸)入力のプレス情報処理
	SHORT GetGameStickRYPress(int nPlayer);			// Rスティック(Y軸)入力のプレス情報処理

private:		// 誰でもアクセスできる

	// メンバ変数
	XINPUT_STATE m_aPadState[MAX_PLAYER];			// プレス情報
	XINPUT_STATE m_aPadStateTrigger[MAX_PLAYER];	// トリガー情報
	XINPUT_STATE m_aPadStateRelease[MAX_PLAYER];	// リリース情報
	XINPUT_VIBRATION m_aPadVibration[MAX_PLAYER];	// バイブレーションの情報
	bool m_bConnect;								// 接続判定
};

// マウスクラス
class CInputMouse : public CInput
{
public:			// 誰でもアクセスできる

	CInputMouse();		// コンストラクタ
	~CInputMouse();		// デストラクタ

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理

private:		// 誰でもアクセスできる

	// メンバ変数
	DIMOUSESTATE m_MouseState;			// マウスの状態
};

#endif