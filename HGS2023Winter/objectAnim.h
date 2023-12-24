//============================================
//
// アニメーションヘッダー[anim.h]
// Author：小原立暉
//
//============================================
#ifndef _ANIMOBJECT_H_			//このマクロ定義がされていなかったら
#define _ANIMOBJECT_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "object2D.h"

//--------------------------------------------
// クラス(オブジェクト2Dアニメーションクラス)
//--------------------------------------------
class CAnim : public CObject2D
{
public:			// 誰でもアクセスできる

	CAnim();						// コンストラクタ
	CAnim(CObject::TYPE type, PRIORITY priority = PRIORITY_ENTITY);		// オーバーロードコンストラクタ
	virtual ~CAnim();				// デストラクタ
	void Box(void);					// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void);		// 更新処理
	virtual void Draw(void);		// 描画処理

	// セット・ゲット関係
	int GetPattern(void) const;								// パターンの取得処理
	float GetTexPattern(void) const;						// テクスチャのパターンの取得処理
	void SetAnim(const int nCount, const int nPattern);		// アニメーションの設定処理

	// 静的メンバ関数
	static CAnim* Create(const CObject::TYPE type, const PRIORITY priority);		// アニメーションの生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	int m_nCounter;					// アニメーションカウンター
	int m_nReplayCount;				// 再生カウント
	int m_nPattern;					// アニメーションパターン
	int m_nReplayPattern;			// 再生パターン
	float m_fTexPattern;			// テクスチャのパターン
};

#endif