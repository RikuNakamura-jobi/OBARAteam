//============================================
//
// スクロールオブジェクトヘッダー[scroll.h]
// Author：小原立暉
//
//============================================
#ifndef _SCROLLOBJECT_H_//このマクロ定義がされていなかったら
#define _SCROLLOBJECT_H_//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "object2D.h"

//=========================================
// マクロ定義
//=========================================
#define BG_MAX					(1.0f)		// 背景の最大値
#define BG_INIT					(0.0f)		// 背景の最小値

//--------------------------------------------
// クラス(オブジェクト2Dスクロールクラス)
//--------------------------------------------
class CScroll : public CObject2D
{
public:			// 誰でもアクセスできる

	CScroll();						// コンストラクタ
	CScroll(CObject::TYPE type, PRIORITY priority = PRIORITY_ENTITY);	// オーバーロードコンストラクタ
	virtual ~CScroll();				// デストラクタ
	void Box(void);					// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void);		// 更新処理
	virtual void Draw(void);		// 描画処理

	// ゲット・セット関係
	void SetTex(const D3DXVECTOR2& tex);			// テクスチャ座標の設定処理
	D3DXVECTOR2 GetTex(void) const;					// テクスチャ座標の取得処理
	void SetSpeed(const D3DXVECTOR2& speed);		// 速度の設定処理

private:		// 自分だけアクセスできる

	// メンバ変数
	D3DXVECTOR2 m_tex;		// テクスチャ座標
	D3DXVECTOR2 m_speed;	// 速度
};

#endif