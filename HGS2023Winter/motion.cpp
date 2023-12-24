//======================================================================================================================
//
//	モーション処理 [motion.cpp]
//	Author：小原立暉
//
//======================================================================================================================
//**********************************************************************************************************************
//	インクルードファイル
//**********************************************************************************************************************
#include "main.h"
#include "input.h"
#include "useful.h"
#include "motion.h"
#include "manager.h"

#include "debugproc.h"

//----------------------------------------------------------------------------------------------------------------------
// マクロ定義
//----------------------------------------------------------------------------------------------------------------------
#define MOTION_SET_FRAME		(7)			// モーション設定時のフレーム数

//============================================================
// コンストラクタ
//============================================================
CMotion::CMotion()
{
	// リセット処理
	ResetData();
}

//============================================================
// デストラクタ
//============================================================
CMotion::~CMotion()
{

}

//============================================================
// 初期化処理
//============================================================
HRESULT CMotion::Init(void)
{
	// リセット処理
	ResetData();

	// 成功を返す
	return S_OK;
}

//============================================================
// 終了処理
//============================================================
void CMotion::Uninit(void)
{

}

//============================================================
// モーション情報のリセット処理
//============================================================
void CMotion::ResetData(void)
{
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{ // モーションの種類分初期化する

		// モーションの情報を初期化
		m_aInfo[nCntMotion].nNumKey = 0;		// キーの総数
		m_aInfo[nCntMotion].bLoop = false;		// ループするかどうか
		m_aInfo[nCntMotion].nNumFrame = 0;		// 総フレーム数

		for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{ // キーの数初期化する

			// キーの情報を初期化する
			m_aInfo[nCntMotion].aKeyInfo[nCntKey].nFrame = 0;		// フレーム数

			for (int nCntPart = 0; nCntPart < MAX_PARTS; nCntPart++)
			{ // パーツ分初期化する

				// 座標の情報を初期化する
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosX = 0.0f;			// 位置(X座標)
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosY = 0.0f;			// 位置(Y座標)
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosZ = 0.0f;			// 位置(Z座標)
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotX = 0.0f;			// 向き(X座標)
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotY = 0.0f;			// 向き(Y座標)
				m_aInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotZ = 0.0f;			// 向き(Z座標)
			}
		}
	}

	for (int nCntPos = 0; nCntPos < MAX_PARTS; nCntPos++)
	{
		m_posInit[nCntPos] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 初期位置
		m_rotPast[nCntPos] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 過去の向き
		m_posPast[nCntPos] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 過去の位置
	}

	// モーションの情報を初期化する
	m_nNumAll = 0;			// モーションの総数
	m_nType = 0;			// 種類
	m_nKey = 0;				// 現在のキー
	m_nCounter = 0;			// カウンター
	m_nStopFrame = 0;		// 停止するフレーム数
	m_bLoop = false;		// ループ状況
	m_bFinish = false;		// 終了したかどうか
	m_ppModel = nullptr;	// モデルのポインタ
	m_nNumModel = 0;		// モデルの総数
	m_nNumKey = 0;			// キーの総数
}

//============================================================
// モーションの種類の設定処理
//============================================================
void CMotion::Set(int nType)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot;			// 向き

	// モーションの情報を初期化する
	m_nType = nType;						// 種類
	m_nNumKey = m_aInfo[nType].nNumKey;		// キーの総数
	m_nKey = 0;								// 現在のキー
	m_nCounter = 0;							// カウンター
	m_bLoop = m_aInfo[nType].bLoop;			// ループ状況
	m_bFinish = false;						// 終了したかどうか
	m_nStopFrame = MOTION_SET_FRAME;		// 停止するフレーム数

	// 全モデルの初期設定
	switch (m_style)
	{
	case TYPE_PLAYER:		// プレイヤー

		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			// 位置と向きを取得
			rot = m_ppModel[nCntModel]->GetRot();

			// パーツの位置・向きを設定
			m_posPast[nCntModel].x = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosX;
			m_posPast[nCntModel].y = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosY;
			m_posPast[nCntModel].z = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosZ;
			m_rotPast[nCntModel].x = rot.x;
			m_rotPast[nCntModel].y = rot.y;
			m_rotPast[nCntModel].z = rot.z;
		}

		break;
	}
}

//============================================================
// モーションの更新処理
//============================================================
void CMotion::Update(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 posDest;		// 目標の位置
	D3DXVECTOR3 posDiff;		// 位置の差分
	D3DXVECTOR3 rotDest;		// 目標の向き
	D3DXVECTOR3 rotDiff;		// 向きの差分
	D3DXVECTOR3 pos;			// 現在の位置
	D3DXVECTOR3 rot;			// 現在の向き

	if (m_bFinish == false)
	{ // 終了していない場合

		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{ // 全モデル(パーツ)の更新

			// 位置と向きを取得する
			pos = m_ppModel[nCntModel]->GetPos();
			rot = m_ppModel[nCntModel]->GetRot();

			// 位置と向きの目標値の設定
			posDest.x = m_aInfo[m_nType].aKeyInfo[(m_nKey + 1) % m_nNumKey].aKey[nCntModel].fPosX;
			posDest.y = m_aInfo[m_nType].aKeyInfo[(m_nKey + 1) % m_nNumKey].aKey[nCntModel].fPosY;
			posDest.z = m_aInfo[m_nType].aKeyInfo[(m_nKey + 1) % m_nNumKey].aKey[nCntModel].fPosZ;
			rotDest.x = m_aInfo[m_nType].aKeyInfo[(m_nKey + 1) % m_nNumKey].aKey[nCntModel].fRotX;
			rotDest.y = m_aInfo[m_nType].aKeyInfo[(m_nKey + 1) % m_nNumKey].aKey[nCntModel].fRotY;
			rotDest.z = m_aInfo[m_nType].aKeyInfo[(m_nKey + 1) % m_nNumKey].aKey[nCntModel].fRotZ;

			// キーの情報から位置・向きの差分の算出
			posDiff.x = posDest.x - m_posPast[nCntModel].x;		// 座標(X軸)
			posDiff.y = posDest.y - m_posPast[nCntModel].y;		// 座標(Y軸)
			posDiff.z = posDest.z - m_posPast[nCntModel].z;		// 座標(Z軸)
			rotDiff.x = rotDest.x - m_rotPast[nCntModel].x;		// 向き(X軸)
			rotDiff.y = rotDest.y - m_rotPast[nCntModel].y;		// 向き(Y軸)
			rotDiff.z = rotDest.z - m_rotPast[nCntModel].z;		// 向き(Z軸)

			// 向きの差分の正規化
			useful::RotNormalize(&rotDiff.x);
			useful::RotNormalize(&rotDiff.y);
			useful::RotNormalize(&rotDiff.z);

			// パーツの位置・向きを設定
			pos.x = m_posInit[nCntModel].x + m_posPast[nCntModel].x + posDiff.x * ((float)(m_nCounter) / (float)(m_nStopFrame));		// 座標(X軸)
			pos.y = m_posInit[nCntModel].y + m_posPast[nCntModel].y + posDiff.y * ((float)(m_nCounter) / (float)(m_nStopFrame));		// 座標(Y軸)
			pos.z = m_posInit[nCntModel].z + m_posPast[nCntModel].z + posDiff.z * ((float)(m_nCounter) / (float)(m_nStopFrame));		// 座標(Z軸)
			rot.x = m_rotPast[nCntModel].x + rotDiff.x * ((float)(m_nCounter) / (float)(m_nStopFrame));		// 向き(X軸)
			rot.y = m_rotPast[nCntModel].y + rotDiff.y * ((float)(m_nCounter) / (float)(m_nStopFrame));		// 向き(Y軸)
			rot.z = m_rotPast[nCntModel].z + rotDiff.z * ((float)(m_nCounter) / (float)(m_nStopFrame));		// 向き(Z軸)

			// 向きの差分の正規化
			useful::RotNormalize(&rot.x);
			useful::RotNormalize(&rot.y);
			useful::RotNormalize(&rot.z);

			// 位置と向きの設定処理
			m_ppModel[nCntModel]->SetPos(pos);
			m_ppModel[nCntModel]->SetRot(rot);
		}

		// モーションカウンターを加算する
		m_nCounter++;

		if (m_nCounter >= m_nStopFrame)
		{ // モーションカウンターが再生フレーム数に達した場合

			// モーションカウンターを初期化する
			m_nCounter = 0;

			// キーの番号を加算する
			m_nKey = (m_nKey + 1) % m_aInfo[m_nType].nNumKey;

			// 停止フレームを設定する
			m_nStopFrame = m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame;

			// 全モデルの初期設定
			for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
			{
				// 過去の位置を設定
				m_posPast[nCntModel].x = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosX;
				m_posPast[nCntModel].y = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosY;
				m_posPast[nCntModel].z = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosZ;

				// 過去の向きを設定
				m_rotPast[nCntModel].x = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotX;
				m_rotPast[nCntModel].y = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotY;
				m_rotPast[nCntModel].z = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotZ;

				// 向きを補正する
				m_ppModel[nCntModel]->SetRot(m_rotPast[nCntModel]);
			}

			if (m_nKey == (m_aInfo[m_nType].nNumKey - 1))
			{ // キー番号が総数になった場合

				if (m_bLoop == false)
				{ // ループしない場合

					switch (m_style)
					{
					case TYPE_PLAYER:		// プレイヤー

						// 終了状態にする
						m_bFinish = true;

						break;

					default:

						// 停止
						assert(false);

						break;
					}
				}
			}
		}
		
		// デバッグ
		CManager::Get()->GetDebugProc()->Print("モーション中\n");
	}
	else
	{ // 上記以外

		// デバッグ
		CManager::Get()->GetDebugProc()->Print("モーション終了\n");
	}
}

//============================================================
// 種類の取得処理
//============================================================
int CMotion::GetType(void)
{
	// 種類を返す
	return m_nType;
}

//============================================================
// モーションの終了処理
//============================================================
bool CMotion::IsFinish(void)
{
	// 終了したかどうかを返す
	return m_bFinish;
}

//============================================================
// モーションの情報の設定処理
//============================================================
void CMotion::SetInfo(Info info)
{
	// モーションの情報を入れる
	m_aInfo[m_nType] = info;
}

//============================================================
// モデルの設定処理
//============================================================
void CMotion::SetModel(CHierarchy **ppHier, int nNumModel)
{
	// モデルの情報を設定する
	m_ppModel = ppHier;

	// パーツの総数を設定する
	m_nNumModel = nNumModel;
}

//============================================================
// モーションのロード処理
//============================================================
void CMotion::Load(STYLE type)
{
	// 種類を設定する
	m_style = type;

	switch (type)
	{
	case TYPE_PLAYER:		// プレイヤー

		// プレイヤーのモーションのロード処理
		LoadPlayer();

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//============================================================
// プレイヤーのモーションのロード処理
//============================================================
void CMotion::LoadPlayer(void)
{
	// 変数を宣言
	D3DXVECTOR3 rot;				// 向きの設定処理
	int nEnd;						// テキスト読み込み終了の確認用
	int nCntPart = 0;				// セットしたパーツの数
	int nMotionCnt = 0;				// モーションの番号
	int nLoop = 0;					// ループの読み込み変数
	int nCntKey = 0;				// キーの番号
	int nParent = NONE_PARENT;		// 親の番号
	char aString[MAX_STRING];		// テキストの文字列の代入用

	// ファイルのポインタを宣言
	FILE  *pFile;							// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen("data/TXT/PlayerMotion.txt", "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "NUM_MODEL") == 0)
			{ // 読み込んだ文字列が NUM_MODEL だった場合

				// モデルの数を読み込む
				fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
				fscanf(pFile, "%d", &m_nNumModel);				// 総モデル数を読み込む
			}

			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // 読み込んだ文字列が CHARACTERSET の場合

				do
				{ // 読み込んだ文字列が PARTSSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // 読み込んだ文字列が PARTSSET の場合

						do
						{ // 読み込んだ文字列が END_PARTSSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // 読み込んだ文字列が INDEX の場合
								fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
								fscanf(pFile, "%d", &nCntPart);			// パーツの番号を読み込む
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // 読み込んだ文字列が PARENT の場合
								fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
								fscanf(pFile, "%d", &nParent);			// 親の番号を読み込む

								if (nParent != NONE_PARENT)
								{ // 親が居た場合

									// 親の設定処理
									m_ppModel[nCntPart]->SetParent(m_ppModel[nParent]);
								}
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // 読み込んだ文字列が POS の場合
								fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
								fscanf(pFile, "%f%f%f",							// モデルの位置を読み込む
									&m_posInit[nCntPart].x,
									&m_posInit[nCntPart].y,
									&m_posInit[nCntPart].z
								);

								// 位置の設定処理
								m_ppModel[nCntPart]->SetPos(m_posInit[nCntPart]);
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // 読み込んだ文字列が ROT の場合
								fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
								fscanf(pFile, "%f%f%f",							// モデルの向きを読み込む
									&rot.x,
									&rot.y,
									&rot.z
								);

								// 向きの設定処理
								m_ppModel[nCntPart]->SetRot(rot);
							}
						} while (strcmp(&aString[0], "END_PARTSSET") != 0);		// 読み込んだ文字列が PARTSSET ではない場合ループ

						nCntPart = 0;				// パーツの番号を初期化する
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);			// 読み込んだ文字列が END_CHARACTERSET ではない場合ループ
			}

			if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // 読み込んだ文字列が MOTIONSET だった場合

				do
				{ // 読み込んだ文字列が END_MOTIONSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "LOOP") == 0)
					{ // 読み込んだ文字列が LOOP だった場合
						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%d", &nLoop);					// ループするかどうかを読み込む

						if (nLoop == 0)
						{ // ループしない判定を読み込んだ場合

							// ループしない判定にする
							m_aInfo[nMotionCnt].bLoop = false;
						}
						else
						{ // ループする判定を読み込んだ場合

							// ループする判定にする
							m_aInfo[nMotionCnt].bLoop = true;
						}
					}

					if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // 読み込んだ文字列が NUM_KEY だった場合

						fscanf(pFile, "%s", &aString[0]);							// = を読み込む (不要)
						fscanf(pFile, "%d", &m_aInfo[nMotionCnt].nNumKey);			// キーの総数を読み込む
					}

					if (strcmp(&aString[0], "KEYSET") == 0)
					{ // 読み込んだ文字列が KEYSET の場合

						do
						{ // 読み込んだ文字列が END_KEYSET ではない場合ループ

							// ファイルから文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "FRAME") == 0)
							{ // 読み込んだ文字列が FRAME だった場合

								fscanf(pFile, "%s", &aString[0]);											// = を読み込む (不要)
								fscanf(pFile, "%d", &m_aInfo[nMotionCnt].aKeyInfo[nCntKey].nFrame);			// フレームの総数を読み込む
							}

							if (strcmp(&aString[0], "KEY") == 0)
							{ // 読み込んだ文字列が KEY の場合

								do
								{ // 読み込んだ文字列が END_KEY ではない場合ループ

									// ファイルから文字列を読み込む
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // 読み込んだ文字列が POS だった場合
										fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
										fscanf(pFile, "%f%f%f",							// モデルの位置を読み込む
											&m_aInfo[nMotionCnt].aKeyInfo[nCntKey].aKey[nCntPart].fPosX,
											&m_aInfo[nMotionCnt].aKeyInfo[nCntKey].aKey[nCntPart].fPosY,
											&m_aInfo[nMotionCnt].aKeyInfo[nCntKey].aKey[nCntPart].fPosZ
										);
									}

									if (strcmp(&aString[0], "ROT") == 0)
									{ // 読み込んだ文字列が ROT の場合
										fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
										fscanf(pFile, "%f%f%f",							// モデルの向きを読み込む
											&m_aInfo[nMotionCnt].aKeyInfo[nCntKey].aKey[nCntPart].fRotX,
											&m_aInfo[nMotionCnt].aKeyInfo[nCntKey].aKey[nCntPart].fRotY,
											&m_aInfo[nMotionCnt].aKeyInfo[nCntKey].aKey[nCntPart].fRotZ
										);
									}
								} while (strcmp(&aString[0], "END_KEY") != 0);		// 読み込んだ文字列が END_KEY ではない場合ループ

								nCntPart++;				// パーツの番号を加算する
							}

						} while (strcmp(&aString[0], "END_KEYSET") != 0);		// 読み込んだ文字列が END_KEYSET ではない場合ループ

						nCntPart = 0;					// パーツの番号を初期化する
						nCntKey++;						// キーの番号を加算する
					}
				} while (strcmp(&aString[0], "END_MOTIONSET") != 0);			// 読み込んだ文字列が END_CHARACTERSET ではない場合ループ

				nCntKey = 0;			// キーカウントを初期化する
				nMotionCnt++;			// モーションのカウントを加算する
			}
		} while (nEnd != EOF);			// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);
	}
}

//============================================================
// モーションの生成処理
//============================================================
CMotion* CMotion::Create()
{
	// ローカルオブジェクトを生成
	CMotion* pMotion = nullptr;	// プレイヤーのインスタンスを生成

	if (pMotion == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pMotion = new CMotion;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pMotion != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pMotion->Init()))
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

	// オブジェクト2Dのポインタを返す
	return pMotion;
}