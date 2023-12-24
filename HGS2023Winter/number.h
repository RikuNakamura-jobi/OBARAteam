//============================================
//
// 数値ヘッダー[number.h]
// Author：小原立暉
//
//============================================
#ifndef _NUMBER_H_					// このマクロ定義がされていなかったら
#define _NUMBER_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "object2D.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define NUMBER_TEXTURE_PATTERN		(0.1f)		// 数字のテクスチャ座標

//--------------------------------------------
// クラス定義(数値)
//--------------------------------------------
class CNumber : public CObject2D
{
public:				// 誰でもアクセスできる

	//--------------------------------------------
	// 種類
	//--------------------------------------------
	enum TYPE
	{
		TYPE_DECIMAL = 0,	// 10進数
		TYPE_TIME,			// 時間
		TYPE_MAX			// この列挙型の総数
	};

	CNumber();							// コンストラクタ
	CNumber(CObject::TYPE type, CObject::PRIORITY priority);		// オーバーロードコンストラクタ
	~CNumber();							// デストラクタ
	void Box(void);						// コンストラクタの箱

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	static CNumber* Create(void);		// 生成処理

	// セット・ゲット関係
	void SetType(const TYPE type);		// 種類の設定処理
	TYPE GetType(void) const;			// 種類の取得処理

	void SetNumber(const int nNum);		// 番号の設定処理
	int GetNumber(void) const;			// 番号の取得処理

private:			// 自分だけアクセスできる

	// メンバ変数
	int m_nNumber;			// 数字
	TYPE m_type;			// 種類
};

#endif