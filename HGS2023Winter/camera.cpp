//===========================================
//
// カメラのメイン処理[camera.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "game.h"
#include "useful.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "object.h"
#include "model.h"
#include "scene.h"
#include "file.h"
#include "pause.h"

//-------------------------------------------
// マクロ定義
//-------------------------------------------
// カメラ全体
#define ASPECT_RATIO				(80.0f)				// 画面のアスペクト比
#define MIN_DISTANCE				(50.0f)				// 距離の最小値
#define MAX_DISTANCE				(8000.0f)			// 距離の最大値
#define DRAW_MIN_Z					(10.0f)				// Z軸の最小値
#define DRAW_MAX_Z					(50000.0f)			// Z軸の最大値

// 向き関係
#define ROT_Y_SPEED					(0.04f)				// Y軸の回転の速度
#define ROTATION_SPEED				(0.05f)				// 回り込み処理を行う基準のモデルの速度
#define ROTATION_ROT				(0.02f)				// カメラの角度の補正倍率

// 位置・距離関係
#define POS_SPEED					(30.0f)				// 移動速度
#define DIS_SPEED					(16.0f)				// 距離の移動量
#define CAMERA_DISTANCE				(550.0f)			// カメラの距離
#define POSR_POINT					(40.0f)				// 追従モードの注視点の位置
#define POSV_POINT					(40.0f)				// 追従モードの視点の位置
#define CORRECT_POSR				(0.22f)				// 注視点の補正倍率
#define CORRECT_POSV				(0.20f)				// 視点の補正倍率
#define RANKING_MOVE				(40.0f)				// ランキングカメラの移動量
#define RANKING_STOP				(25000.0f)			// ランキングカメラの止まる座標
#define MIN_POSR_Y					(90.0f)				// 注視点の最低座標(Y軸)
#define MIN_POSV_Y					(120.0f)			// 視点の最低座標(Y軸)

#define CHASE_SHIFT_X				(400.0f)			// 追跡カメラの前にずらす距離(X軸)
#define POSR_SHIFT_Y				(190.0f)			// 注視点のずらす幅(Y軸)
#define POSV_SHIFT_Y				(220.0f)			// 視点のずらす幅(Y軸)

//=======================
// コンストラクタ
//=======================
CCamera::CCamera()
{
	// 全ての情報をクリアする
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 視点
	m_posVDest = m_posV;							// 目的の視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 注視点
	m_posRDest = m_posR;							// 目的の注視点
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 上方向ベクトル
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	ZeroMemory(&m_viewport, sizeof(D3DVIEWPORT9));	// ビューポート
	m_type = TYPE_NONE;								// 種類
	m_rotDest = m_rot.y;							// 目的の向き
	m_Dis = 0.0f;									// 距離
	m_DisDest = 0.0f;								// 目的の距離
	m_nSwingCount = 0;								// 揺れカメラのカウント
	m_bControl = false;								// 操作状況
}

//=======================
// デストラクタ
//=======================
CCamera::~CCamera()
{

}

//=======================
// カメラの初期化処理
//=======================
HRESULT CCamera::Init(void)
{
	//カメラの情報のリセット処理
	Reset();

	// ビューポートの設定処理
	m_viewport.X = (DWORD)0.0f;				// 描画する画面の左上X座標
	m_viewport.Y = (DWORD)0.0f;				// 描画する画面の左上Y座標
	m_viewport.Width = SCREEN_WIDTH;	// 描画する画面の幅
	m_viewport.Height = SCREEN_HEIGHT;	// 描画する画面の高さ
	m_viewport.MinZ = 0.0f;				// クリップボリュームの最小値
	m_viewport.MaxZ = 1.0f;				// クリップボリュームの最大値

	// 成功を返す
	return S_OK;
}

//=======================
// カメラの終了処理
//=======================
void CCamera::Uninit(void)
{

}

//=======================
// カメラの更新処理
//=======================
void CCamera::Update(void)
{
	switch (CManager::Get()->GetMode())
	{
	case CScene::MODE_GAME:		// ゲームモード

		if (CGame::GetPause() != nullptr &&
			CGame::GetPause()->GetPause() == false)
		{ // ポーズ中以外の場合

			// 種類ごとの処理
			TypeProcess();
		}
		else
		{ // 上記以外

// デバッグモード
#ifdef _DEBUG

// ポーズ中のカメラ操作
#if 0
			if (m_bControl == true)
			{ // 捜査状況が true の場合

				// 操作処理
				Control();
			}
#endif

#endif
		}

		break;

	case CScene::MODE_RANKING:	// ランキング

		if (m_posV.x <= RANKING_STOP ||
			m_posV.x <= RANKING_STOP)
		{ // 位置が一定数以下だった場合

			// カメラを移動させる
			m_posV.x += RANKING_MOVE;
			m_posR.x += RANKING_MOVE;
		}

		break;

	default:					// 上記以外

		// 情報のリセット処理
		Reset();

		break;
	}

	// 向きの正規化処理
	useful::RotNormalize(&m_rot.y);

#if 0

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{ // スペースキーを押した場合

		// カメラの情報のリセット処理
		Reset();
	}

#endif

	//// 目的の注視点を設定する
	//m_posRDest.x = pos.x;
	//m_posRDest.z = pos.z;

	//// 目的の視点を設定する
	//m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
	//m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;

	//// 目的の向きを設定する
	//m_rotDest = 0.0f;

	if (m_posR.y <= MIN_POSR_Y)
	{ // 注視点が一定数になった場合

		// 注視点を設定する
		m_posR.y = MIN_POSR_Y;
	}

	if (m_posV.y <= MIN_POSV_Y)
	{ // 視点が一定数になった場合

		// 視点を設定する
		m_posV.y = MIN_POSV_Y;
	}
}

//=======================
// カメラの設定処理
//=======================
void CCamera::Set(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

// デバッグ用
#ifdef _DEBUG

		// プロジェクションマトリックスを作成(透視投影)
	D3DXMatrixPerspectiveFovLH
	(
		&m_mtxProjection,									// プロジェクションマトリックス
		D3DXToRadian(ASPECT_RATIO),							// 視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,			// 画面のアスペクト比
		DRAW_MIN_Z,											// Z値の最小値
		DRAW_MAX_Z											// Z値の最大値
	);

// リリース用
#else

	// プロジェクションマトリックスを作成(透視投影)
	D3DXMatrixPerspectiveFovLH
	(
		&m_mtxProjection,									// プロジェクションマトリックス
		D3DXToRadian(ASPECT_RATIO),							// 視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,			// 画面のアスペクト比
		DRAW_MIN_Z,											// Z値の最小値
		DRAW_MAX_Z											// Z値の最大値
	);

	//// プロジェクションマトリックスの作成(並行投影)
	//D3DXMatrixOrthoLH
	//(
	//	&m_mtxProjection,									// ビューマトリックス
	//	SCREEN_WIDTH,										// 画面の幅
	//	SCREEN_HEIGHT,										// 画面の高さ
	//	DRAW_MIN_Z,											// Z軸の最小値
	//	DRAW_MAX_Z											// Z軸の最大値
	//);

#endif

	// プロジェクトマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH
	(
		&m_mtxView,										// ビューマトリックス
		&m_posV,										// 視点
		&m_posR,										// 注視点
		&m_VecU											// 上方向ベクトル
	);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=======================
// 視点の位置の設定処理
//=======================
void CCamera::SetPosV(const D3DXVECTOR3 posV)
{
	// 視点の位置を設定する
	m_posV = posV;
}

//=======================
// 視点の位置の取得処理
//=======================
D3DXVECTOR3 CCamera::GetPosV(void) const
{
	// 視点の位置を返す
	return m_posV;
}

//=======================
// 注視点の位置の設定処理
//=======================
void CCamera::SetPosR(const D3DXVECTOR3 posR)
{
	// 注視点の位置を設定する
	m_posR = posR;
}

//=======================
// 注視点の位置の取得処理
//=======================
D3DXVECTOR3 CCamera::GetPosR(void) const
{
	// 注視点の位置を返す
	return m_posR;
}

//=======================
// 向きの設定処理
//=======================
void CCamera::SetRot(const D3DXVECTOR3& rot)
{
	// 向きの情報を設定する
	m_rot = rot;
}

//=======================
// 向きの取得処理
//=======================
D3DXVECTOR3 CCamera::GetRot(void) const
{
	// 向きの情報を返す
	return m_rot;
}

//=======================
// 距離の設定処理
//=======================
void CCamera::SetDistance(const float fDis)
{
	// 距離を設定する
	m_Dis = fDis;

	// 目的の視点を設定する
	m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
	m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
}

//=======================
// 距離の取得処理
//=======================
float CCamera::GetDistance(void) const
{
	// 距離を返す
	return m_Dis;
}

//=======================
// 種類の設定処理
//=======================
void CCamera::SetType(const TYPE type)
{
	// 種類を設定する
	m_type = type;
}

//=======================
// カメラの操作状況の切り替え処理
//=======================
void CCamera::ChangeControl(void)
{
	// 操作状況を切り替える
	m_bControl = m_bControl ? false : true;
}

//=======================
// カメラの情報のリセット処理
//=======================
void CCamera::Reset(void)
{
	// 全ての値をリセットする
	m_Dis = CAMERA_DISTANCE;					// 視点と注視点の距離
	m_posV = D3DXVECTOR3(0.0f, 0.0f, -m_Dis);	// 視点の位置
	m_posVDest = m_posV;						// 目的の視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 注視点の位置
	m_posRDest = m_posR;						// 目的の注視点
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 上方向ベクトル
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_type = TYPE_NONE;							// 種類
	m_rotDest = m_rot.y;						// 目的の向き
	m_DisDest = CAMERA_DISTANCE;				// 目的の距離
	m_nSwingCount = 0;							// 揺れカメラのカウント
	m_bControl = false;							// 操作状況
}

//=======================
// カメラの回り込み処理
//=======================
void CCamera::Rotation(void)
{
	////カメラの目的の向きを設定する
	//m_rotDest = (pModelrot->y);

	////カメラの向きの差分を設定する
	//m_rotDiff = m_rotDest - m_rot.y;

	//if (m_rotDiff > D3DX_PI)
	//{//角度が3.14fより大きかった場合
	// //角度から1周分減らす
	//	m_rotDiff -= (2 * D3DX_PI);
	//}
	//else if (m_rotDiff < -D3DX_PI)
	//{//角度が-3.14fより小さかった場合
	// //角度に1周分加える
	//	m_rotDiff += (2 * D3DX_PI);
	//}

	////カメラの向きを補正する
	//m_rot.y += m_rotDiff * ROTATION_ROT;

	//if (m_rot.y > D3DX_PI)
	//{//角度が3.14fより大きかった場合
	// //角度から1周分減らす
	//	m_rot.y = -D3DX_PI;
	//}
	//else if (m_rot.y < -D3DX_PI)
	//{//角度が-3.14fより小さかった場合
	// //角度に1周分加える
	//	m_rot.y = D3DX_PI;
	//}
}

//=======================
// カメラの移動処理
//=======================
void CCamera::Move(void)
{
	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_UP) == true ||
		CManager::Get()->GetInputGamePad()->GetGameStickRYPress(0) > 0)
	{ // Wキーを押した場合

		if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_RIGHT) == true ||
			CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) > 0)
		{ // Dキーを押した場合

			m_posR.x += sinf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			
			m_posR.z += cosf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.z += cosf(D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
		}
		else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_LEFT) == true ||
			CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) < 0)
		{ // Aキーを押した場合

			m_posR.x += sinf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;

			m_posR.z += cosf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
			m_posV.z += cosf(-D3DX_PI * 0.25f + m_rot.y) * POS_SPEED;
		}
		else
		{ // Wキーのみを押した場合

			m_posR.x += sinf(m_rot.y) * POS_SPEED;
			m_posV.x += sinf(m_rot.y) * POS_SPEED;

			m_posR.z += cosf(m_rot.y) * POS_SPEED;
			m_posV.z += cosf(m_rot.y) * POS_SPEED;
		}
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_DOWN) == true ||
		CManager::Get()->GetInputGamePad()->GetGameStickRYPress(0) < 0)
	{ // Sキーを押した場合

		if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_RIGHT) == true ||
			CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) > 0)
		{ // Dキーを押した場合

			m_posR.x += sinf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;

			m_posR.z += cosf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.z += cosf(D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
		}
		else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_LEFT) == true ||
			CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) < 0)
		{ // Aキーを押した場合

			m_posR.x += sinf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.x += sinf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;

			m_posR.z += cosf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
			m_posV.z += cosf(-D3DX_PI * 0.75f + m_rot.y) * POS_SPEED;
		}
		else
		{ // Sキーのみを押した場合

			m_posR.x -= sinf(m_rot.y) * POS_SPEED;
			m_posV.x -= sinf(m_rot.y) * POS_SPEED;

			m_posR.z -= cosf(m_rot.y) * POS_SPEED;
			m_posV.z -= cosf(m_rot.y) * POS_SPEED;
		}
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_RIGHT) == true ||
		CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) > 0)
	{ // Dキーを押した場合

		m_posR.x += sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
		m_posV.x += sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;

		m_posR.z += cosf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
		m_posV.z += cosf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_LEFT) == true ||
		CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) < 0)
	{ // Aキーを押した場合

		m_posR.x -= sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;
		m_posV.x -= sinf(D3DX_PI * 0.5f + m_rot.y) * POS_SPEED;

		m_posR.z += cosf(D3DX_PI * 0.5f - m_rot.y) * POS_SPEED;
		m_posV.z += cosf(D3DX_PI * 0.5f - m_rot.y) * POS_SPEED;
	}
}

//=======================
// カメラの視点操作処理
//=======================
void CCamera::MovePosV(void)
{
	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_Y) == true)
	{ // Yキーを押した場合
		
		// 視点を上に動かす
		m_posV.y += POS_SPEED;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_N) == true)
	{ // Nキーを押した場合

		// 視点を下に動かす
		m_posV.y -= POS_SPEED;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_Z) == true)
	{ // Zキーを押した場合

		// 視点を右に回す
		m_rot.y -= ROT_Y_SPEED;

		// カメラの視点を更新する
		m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_C) == true)
	{ // Cキーを押した場合

		// 視点を左に回す
		m_rot.y += ROT_Y_SPEED;

		// カメラの視点を更新する
		m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
	}
}

//=======================
// カメラの注視点操作処理
//=======================
void CCamera::MovePosR(void)
{
	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_T) == true)
	{ // Tキーを押した場合

		// 注視点を上に動かす
		m_posR.y += POS_SPEED;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_B) == true)
	{ // Bキーを押した場合

		// 注視点を下に動かす
		m_posR.y -= POS_SPEED;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_Q) == true)
	{ // Qキーを押した場合

		// 注視点を右に回す
		m_rot.y -= ROT_Y_SPEED;

		// 注視点を更新する
		m_posR.x = m_posV.x + sinf(m_rot.y) * m_Dis;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_Dis;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_E) == true)
	{ // Eキーを押した場合

		// 注視点を左に回す
		m_rot.y += ROT_Y_SPEED;

		// 注視点を更新する
		m_posR.x = m_posV.x + sinf(m_rot.y) * m_Dis;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_Dis;
	}
}

//=======================
// カメラの距離操作処理
//=======================
void CCamera::MoveRange(void)
{
	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{ // 左SHIFTキーを押している場合

		if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_U) == true)
		{ // Uキーを押した場合

			// 距離を減算する
			m_Dis -= DIS_SPEED;

			if (m_Dis <= MIN_DISTANCE)
			{ // 100.0f以下の場合

				// 100.0fにする
				m_Dis = MIN_DISTANCE;
			}

			// カメラの視点を更新する
			m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
			m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
		}
		else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_M) == true)
		{ // Mキーを押した場合

			// 距離を減算する
			m_Dis += DIS_SPEED;

			if (m_Dis >= MAX_DISTANCE)
			{ // 800.0f以上の場合

				// 800.0fにする
				m_Dis = MAX_DISTANCE;
			}

			// カメラの視点を更新する
			m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
			m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;
		}
	}
}

//=======================
// 操作処理
//=======================
void CCamera::Control(void)
{
	//カメラの移動処理
	Move();

	// カメラの視点操作処理
	MovePosV();

	// カメラの注視点操作処理
	MovePosR();

	// カメラの距離操作処理
	MoveRange();
}

//=======================
// カメラの位置の設定処理
//=======================
void CCamera::PosSet(void)
{
	// カメラの視点を更新する
	m_posV.x = m_posR.x + sinf(m_rot.y) * -m_Dis;
	m_posV.z = m_posR.z + cosf(m_rot.y) * -m_Dis;

	// カメラの注視点を更新する
	m_posR.x = m_posV.x + sinf(m_rot.y) * m_Dis;
	m_posR.z = m_posV.z + cosf(m_rot.y) * m_Dis;
}

//=======================
// カメラの種類ごとの処理
//=======================
void CCamera::TypeProcess(void)
{
	switch (m_type)
	{
	case CCamera::TYPE_NONE:		// 通常

		// 追跡処理
		Chase();

		break;

	case CCamera::TYPE_VIBRATE:		// 振動

		// 振動カメラ処理
		Vibrate();

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//=======================
// カメラの追跡処理
//=======================
void CCamera::Chase(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 rot;					// 向き
	//CPlayer* pPlayer = CPlayer::Get();	// プレイヤーのポインタ
	m_DisDest = CAMERA_DISTANCE;		// 目的の距離

	// 距離の補正処理
	useful::Correct(m_DisDest, &m_Dis, CORRECT_POSR);
	useful::Correct(m_rotDest, &m_rot.y, CORRECT_POSR);

	//if (pPlayer != nullptr)
	//{ // プレイヤーが NULL じゃない場合

	//	// プレイヤーの情報を取得する
	//	pos = pPlayer->GetPos();		// 位置
	//	rot = pPlayer->GetRot();		// 向き

	//	// 目的の注視点を設定する
	//	m_posRDest.x = pos.x + CHASE_SHIFT_X;
	//	m_posRDest.y = pos.y + POSR_SHIFT_Y;
	//	m_posRDest.z = pos.z;

	//	// 目的の視点を設定する
	//	m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
	//	m_posVDest.y = pos.y + POSV_SHIFT_Y;
	//	m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;

	//	// 注視点を補正
	//	m_posR.x += (m_posRDest.x - m_posR.x) * CORRECT_POSR;
	//	m_posR.y += (m_posRDest.y - m_posR.y) * CORRECT_POSR;
	//	m_posR.z += (m_posRDest.z - m_posR.z) * CORRECT_POSR;

	//	// 視点を補正
	//	m_posV.x += (m_posVDest.x - m_posV.x) * CORRECT_POSV;
	//	m_posV.y += (m_posVDest.y - m_posV.y) * CORRECT_POSR;
	//	m_posV.z += (m_posVDest.z - m_posV.z) * CORRECT_POSV;
	//}
}

//=======================
// カメラの振動処理
//=======================
void CCamera::Vibrate(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	//CPlayer* pPlayer = CPlayer::Get();	// プレイヤーのポインタ

	//if (pPlayer != nullptr)
	//{ // プレイヤーが NULL じゃない場合

	//	// プレイヤーの情報を取得する
	//	pos = pPlayer->GetPos();		// 位置
	//	rot = pPlayer->GetRot();		// 向き

	//	if (m_nSwingCount % 5 == 0)
	//	{ // 揺れカウントが一定数ごとに

	//		float f = (float)(rand() % 8 + 6);

	//		if (m_nSwingCount % 2 == 0)
	//		{ // カウントが偶数の場合

	//			// 目的の注視点を設定する
	//			m_posRDest.y = pos.y + POSR_SHIFT_Y - f;
	//		}
	//		else
	//		{ // カウントが奇数の場合

	//			// 目的の注視点を設定する
	//			m_posRDest.y = pos.y + POSR_SHIFT_Y + f;
	//		}

	//		// 目的の注視点を設定する
	//		m_posRDest.x = pos.x + CHASE_SHIFT_X;
	//		m_posRDest.z = pos.z;

	//		// 目的の視点を設定する
	//		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * -m_Dis;
	//		m_posVDest.y = pos.y + POSV_SHIFT_Y;
	//		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * -m_Dis;
	//	}

	//	// 注視点を補正
	//	m_posR.x += (m_posRDest.x - m_posR.x) * 0.3f;
	//	m_posR.y += (m_posRDest.y - m_posR.y) * 0.3f;
	//	m_posR.z += (m_posRDest.z - m_posR.z) * 0.3f;

	//	// 視点を補正
	//	m_posV.x += (m_posVDest.x - m_posV.x) * 0.3f;
	//	m_posV.y += (m_posVDest.y - m_posV.y) * 0.3f;
	//	m_posV.z += (m_posVDest.z - m_posV.z) * 0.3f;

	//	if (pPlayer->GetAbility()->GetAbility() != CAbility::ABILITY_GROUNDQUAKE)
	//	{ // カウント数が一定以上になった場合

	//		// 種類を設定する
	//		SetType(TYPE_NONE);

	//		// 揺れカウントを初期化する
	//		m_nSwingCount = 0;
	//	}
	//}

	// 揺れカウントを加算する
	m_nSwingCount++;
}