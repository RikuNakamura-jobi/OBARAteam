//============================================================
//
//	モーションヘッダー [motion.h]
//	Author：小原立暉
//
//============================================================
#ifndef _MOTION_H_	// このマクロ定義がされていない場合
#define _MOTION_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"
#include "character.h"

//------------------------------------------------------------
//	マクロ定義
//------------------------------------------------------------
#define MAX_KEY			(10)				// キーの総数
#define MAX_MOTION		(20)				// モーションの最大数

// クラス定義(CMotion)
class CMotion
{
public:			// 誰でもアクセスできる

	//------------------------------------------------------------
	// 列挙型定義(種類)
	//------------------------------------------------------------
	enum STYLE
	{
		TYPE_PLAYER = 0,			// プレイヤー
		TYPE_MAX					// この列挙型の総数
	};

	//------------------------------------------------------------
	//	構造体定義(キーの構造体)
	//------------------------------------------------------------
	struct Key
	{
		float fPosX;				// 位置X
		float fPosY;				// 位置Y
		float fPosZ;				// 位置Z
		float fRotX;				// 向きX
		float fRotY;				// 向きY
		float fRotZ;				// 向きZ
	};

	//------------------------------------------------------------
	//	構造体定義(キー情報の構造体)
	//------------------------------------------------------------
	struct Key_Info
	{
		int nFrame;					// 再生フレーム
		Key aKey[MAX_PARTS];		// 各モデルのキー要素
	};

	//------------------------------------------------------------
	//	構造体定義(モーション情報の構造体)
	//------------------------------------------------------------
	struct Info
	{
		bool bLoop;					// ループするかしないか
		int nNumKey;				// キーの総数
		int nNumFrame;				// フレームの総数
		Key_Info aKeyInfo[MAX_KEY];	// キーの情報
	};

	CMotion();				// コンストラクタ
	~CMotion();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);									// 初期化処理
	void Uninit(void);									// 終了処理
	void ResetData(void);								// モーション情報のリセット処理
	void Set(int nType);								// モーションの種類の設定処理
	void Update(void);									// モーションの更新処理
	int GetType(void);									// 種類の取得処理
	bool IsFinish(void);								// モーションの終了処理
	void SetInfo(Info info);							// モーションの情報の設定処理
	void SetModel(CHierarchy **ppHier, int nNumModel);	// モデルの設定処理
	void Load(STYLE type);								// モーションのロード処理
	void LoadPlayer(void);								// プレイヤーのモーションのロード処理

	// 静的メンバ関数
	static CMotion* Create();							// モーションの生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	D3DXVECTOR3 m_posInit[MAX_PARTS];	// 初期位置
	D3DXVECTOR3 m_posPast[MAX_PARTS];	// 過去の位置
	D3DXVECTOR3 m_rotPast[MAX_PARTS];	// 過去の向き
	Info m_aInfo[MAX_MOTION];			// 情報
	STYLE m_style;						// モーションを持つ者の種類
	int m_nNumAll;						// モーションの総数
	int m_nType;						// 種類
	int m_nNumKey;						// キーの総数
	int m_nKey;							// 現在のキー
	int m_nCounter;						// カウンター
	int m_nStopFrame;					// 停止するフレーム数
	bool m_bLoop;						// ループ状況
	bool m_bFinish;						// 終了したかどうか
	CHierarchy** m_ppModel;				// モデルへのダブルポインタ
	int m_nNumModel;					// モデルの総数
};

#endif