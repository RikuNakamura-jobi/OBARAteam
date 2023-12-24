//===================================
//
// 丸影ヘッダー[shadowCircle.h]
// Author 小原立暉
//
//===================================
#ifndef _SHADOWCIRCLE_H_				//このマクロ定義がされていなかったら
#define _SHADOWCIRCLE_H_				//2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "main.h"
#include "object3D.h"

//-----------------------------------
// クラス定義(影)
//-----------------------------------
class CShadowCircle : public CObject3D
{
public:			// 誰でもアクセスできる

	CShadowCircle();			// コンストラクタ
	~CShadowCircle();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, float fRadius);		// 情報の設定処理

	void AlphaSet(void);		// 透明度の設定処理

	// セット・ゲット関係
	int GetNumID(void) const;		// IDの取得処理

	// 静的メンバ関数
	static CShadowCircle* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, float fRadius);	// 生成処理
	static void SetPosRot(int nIdx, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);				// 影の位置設定処理
	static void SetPosRotXZ(int nIdx, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);				// 影の位置設定処理(Y軸無し)
	static CShadowCircle* GetShadow(int nIdx);														// 丸影の取得処理

private:		// 自分だけアクセスできる

	// メンバ変数
	D3DXCOLOR m_col;			// 色
	int m_nID;					// 影のインデックス

	// 静的メンバ変数
	static int m_nNumAll;		// 影の総数
};


#endif