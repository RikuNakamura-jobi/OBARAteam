//============================================
//
// 時間ヘッダー[time.h]
// Author：小原立暉
//
//============================================
#ifndef _TIME_H_					// このマクロ定義がされていなかったら
#define _TIME_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "number.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CObject2D;				// 2Dポリゴン

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define MAX_TIME_DIGIT		(6)			// タイムの桁数
#define MAX_TIME			(999999)	// 時間の最大数

//--------------------------------------------
// クラス定義(タイム)
//--------------------------------------------
class CTime : public CObject
{
public:				// 誰でもアクセスできる

	CTime();				// コンストラクタ
	CTime(CObject::TYPE type, PRIORITY priority = PRIORITY_UI);		// オーバーロードコンストラクタ
	virtual ~CTime();		// デストラクタ
	void Box(void);			// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void);		// 更新処理
	virtual void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const D3DXVECTOR3& dotSize, const float fShift);				// 情報の設定処理

	void SetNumber(void);	// 1桁ごとの数値設定処理
	void SetTexture(void);	// 1桁ごとのテクスチャの設定処理

	// セット・ゲット関係
	void SetTotalTime(const DWORD time);	// 経過時間の設定処理
	DWORD GetTotalTime(void) const;			// 経過時間の取得処理

	CNumber* GetNumber(const int nID);		// 番号情報の取得処理

	CObject2D* GetDot(void);				// 点の取得処理

	// 静的メンバ変数
	static CTime* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const D3DXVECTOR3& dotSize, const float fShift);		// 生成処理

private:			// 自分だけアクセスできる

	// メンバ変数
	CNumber* m_apNumber[MAX_TIME_DIGIT];	// 番号の情報
	CObject2D* m_pDot;						// タイムの点
	DWORD m_TotalTime;						// 合計時間
};

#endif