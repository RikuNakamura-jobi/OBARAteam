//===================================
//
// テクスチャヘッダー[texture.h]
// Author 小原立暉
//
//===================================
#ifndef _TEXTURE_H_				//このマクロ定義がされていなかったら
#define _TEXTURE_H_				//2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "main.h"

//-----------------------------------
// マクロ定義
//-----------------------------------
#define MAX_TEXTURE		(128)			// テクスチャの最大数

//-----------------------------------
// クラス定義(テクスチャ)
//-----------------------------------
class CTexture
{
public:			// 誰でもアクセスできる

	CTexture();			// コンストラクタ
	~CTexture();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Unload(void);		// 破棄処理

	int Regist(const char* pFilename);						// テクスチャの登録処理
	LPDIRECT3DTEXTURE9 GetAddress(const int nIdx) const;	// テクスチャの取得処理
	HRESULT Load(void);		// ロード処理

private:		// 自分だけアクセスできる

	// メンバ変数
	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE];			// テクスチャの情報
	char m_aTextureName[MAX_TEXTURE][MAX_STRING];			// テクスチャのパス名

	// 静的メンバ変数
	static int m_nNumAll;									// テクスチャの総数
};


#endif