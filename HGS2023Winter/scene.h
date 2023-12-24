//============================================
//
// シーンヘッダー[scene.h]
// Author：小原立暉
//
//============================================
#ifndef _SCENE_H_			//このマクロ定義がされていなかったら
#define _SCENE_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "object.h"

//--------------------------------------------
// シーンクラス
//--------------------------------------------
class CScene : public CObject
{
public:

	// 列挙型定義(モード)
	enum MODE
	{
		MODE_LOGO = 0,			// ロゴ画面
		MODE_TITLE,				// タイトル画面
		MODE_TUTORIAL,			// チュートリアル画面
		MODE_GAME,				// ゲーム画面
		MODE_RESULT,			// リザルト画面
		MODE_RANKING,			// ランキング画面
		MODE_MAX				// この列挙型の総数
	};

	CScene();					// コンストラクタ
	CScene(TYPE type = TYPE_NONE, PRIORITY priority = PRIORITY_BG);		// オーバーロードコンストラクタ
	virtual ~CScene();			// デストラクタ
	void Box(void);				// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void);			// 初期化処理
	virtual void Uninit(void);			// 終了処理
	virtual void Update(void);			// 更新処理
	virtual void Draw(void);			// 描画処理

	virtual void SetData(const MODE mode);		// 情報の設定処理

	// セット・ゲット関係
	void SetMode(const MODE mode);		// モードの設定処理
	MODE GetMode(void) const;			// モードの取得処理

	// 静的メンバ関数
	static CScene* Create(const MODE mode);		// 生成処理

private:

	// メンバ変数
	MODE m_mode;				// モード
};

#endif