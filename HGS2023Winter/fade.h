//===========================================
//
// フェードヘッダー[fade.h]
// Author 小原立暉
//
//===========================================
#ifndef _FADE_H_				// このマクロ定義がされていなかったら
#define _FADE_H_				// 2重インクルード防止のマクロを定義する

//-------------------------------------------
// インクルードファイル
//-------------------------------------------
#include "main.h"

// クラス定義(フェード)
class CFade
{
public:

	//-------------------------------------------
	// 列挙型定義(フェードの状態)
	//-------------------------------------------
	enum FADE
	{
		FADE_NONE = 0,					//何もしていない状態
		FADE_IN,						//フェードイン状態
		FADE_OUT,						//フェードアウト状態
		FADE_MAX
	};

	CFade();							// コンストラクタ
	~CFade();							// デストラクタ

	// メンバ関数
	HRESULT Init(CScene::MODE modeNext);	// 初期化処理
	void Uninit(void);						// 終了処理
	void Update(void);						// 更新処理
	void Draw(void);						// 描画処理
	void SetFade(CScene::MODE modeNext);	// フェードの設定処理
	FADE GetFade(void);						// フェードの取得処理

	// 静的メンバ関数
	static CFade* Create(CScene::MODE modeNext);	// 生成処理

private:

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// フェードの頂点バッファへのポインタ
	FADE m_fade;						// フェードの状態
	CScene::MODE m_modeNext;			// 次の画面(モード)
	D3DXCOLOR m_color;					// ポリゴン(フェード)の色
};

#endif