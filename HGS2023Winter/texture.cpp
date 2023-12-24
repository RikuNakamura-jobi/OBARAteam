//============================================
//
// テクスチャ処理[texture.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "texture.h"
#include "manager.h"
#include "renderer.h"

// マクロ定義
#define LOAD_TXT		"data/TXT/Texture.txt"		// テクスチャの情報テキスト

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
int CTexture::m_nNumAll = 0;						// テクスチャの総数

//--------------------------------------------
// コンストラクタ
//--------------------------------------------
CTexture::CTexture()
{
	// 全ての情報をクリアする
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		m_apTexture[nCnt] = nullptr;		// テクスチャの情報
		*m_aTextureName[nCnt] = {};			// テクスチャのパス
	}
}

//--------------------------------------------
// デストラクタ
//--------------------------------------------
CTexture::~CTexture()
{

}

//--------------------------------------------
// 読み込み処理
//--------------------------------------------
HRESULT CTexture::Init(void)
{
	// 全ての値を初期化する
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		m_apTexture[nCntTex] = nullptr;			// テクスチャのポインタ
		*m_aTextureName[nCntTex] = {};			// テクスチャのパス
	}

	// 成功を返す
	return S_OK;
}

//--------------------------------------------
// 破棄処理
//--------------------------------------------
void CTexture::Unload(void)
{
	for (int nCntUn = 0; nCntUn < MAX_TEXTURE; nCntUn++)
	{
		if (m_apTexture[nCntUn] != nullptr)
		{ // テクスチャの破棄

			m_apTexture[nCntUn]->Release();
			m_apTexture[nCntUn] = nullptr;

			// テクスチャの総数を減算する
			m_nNumAll--;
		}
	}
}

//--------------------------------------------
// テクスチャの登録処理
//--------------------------------------------
int CTexture::Regist(const char* pFilename)
{
	// ローカル変数宣言
	int nNumRegist = NONE_TEXIDX;		// 返り値用変数

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	if (pFilename != nullptr)
	{ // ファイル名が NULL じゃない場合

		for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
		{
			if (m_apTexture[nCntTex] == nullptr)
			{ // ポインタが NULL の場合

				// テクスチャの読み込み
				if (FAILED(D3DXCreateTextureFromFile
				(
					pDevice,
					pFilename,					// テクスチャの相対パス
					&m_apTexture[nCntTex]		// テクスチャのポインタ
				)))
				{ // テクスチャの生成に失敗した場合

					// 警告文
					MessageBox(NULL, "テクスチャの読み込みに失敗！", "警告！", MB_ICONWARNING);

					// 失敗を返す
					return NONE_TEXIDX;
				}

				// 返り値用変数に番号を代入する
				nNumRegist = nCntTex;

				// テクスチャの総数を加算する
				m_nNumAll++;

				// パスを登録する
				strcpy(&m_aTextureName[nCntTex][0], pFilename);

				// 抜け出す
				break;
			}
			else
			{ // ポインタが NULL じゃない場合

				if (strcmp(&m_aTextureName[nCntTex][0], pFilename) == 0)
				{ // 過去に読み込んでいた場合

					// 返り値用変数に番号を代入する
					nNumRegist = nCntTex;

					// 抜け出す
					break;
				}
			}
		}

		// インデックスを返す
		return nNumRegist;
	}
	else
	{ // 上記以外

		// -1 を返す
		return NONE_TEXIDX;
	}
}

//--------------------------------------------
// テクスチャの取得処理
//--------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetAddress(const int nIdx) const
{
	if (nIdx > NONE_TEXIDX)
	{ // テクスチャの番号が -1 以上の場合

		// テクスチャの情報を返す
		return m_apTexture[nIdx];
	}
	else
	{ // 上記以外

		// NULL を返す
		return nullptr;
	}
}

//--------------------------------------------
// ロード処理
//--------------------------------------------
HRESULT CTexture::Load(void)
{
	// ローカル変数宣言
	int nEnd;						// テキスト読み込み終了の確認用
	char aString[MAX_STRING];		// テキストの文字列の代入用
	char aTextureName[MAX_STRING];	// テクスチャの名前(相対パス)

	// ポインタを宣言
	FILE *pFile;						// ファイルポインタ

	// ファイルを読み込み形式で開く
	pFile = fopen(LOAD_TXT, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		do
		{ // 読み込んだ文字列が EOF ではない場合ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// テキストを読み込みきったら EOF を返す

			if (strcmp(&aString[0], "TEXTURE") == 0)
			{ // 読み込んだ文字列が TEXTURE の場合

				fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
				fscanf(pFile, "%s", &aTextureName[0]);		// テクスチャの相対パスを読み込む

				// テクスチャの登録処理
				Regist(&aTextureName[0]);
			}
		} while (nEnd != EOF);				// 読み込んだ文字列が EOF ではない場合ループ

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{ // ファイルが開けなかった場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}