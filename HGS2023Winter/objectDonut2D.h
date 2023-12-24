//============================================
//
// ドーナツ2Dヘッダー[donut2D.h]
// Author：小原立暉
//
//============================================
#ifndef _DONUT2D_H_		//このマクロ定義がされていなかったら
#define _DONUT2D_H_		//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "object.h"

//--------------------------------------------
// クラス(ドーナツ2Dクラス)
//--------------------------------------------
class CDonut2D : public CObject
{
public:			// 誰でもアクセスできる

	CDonut2D();						// コンストラクタ
	CDonut2D(CObject::TYPE type, PRIORITY priority = PRIORITY_UI);		// オーバーロードコンストラクタ
	virtual ~CDonut2D();			// デストラクタ
	void Box(void);					// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void);		// 更新処理
	virtual void Draw(void);		// 描画処理

	void BindTexture(int nIdx);				// テクスチャの割り当て処理

	// 頂点操作系
	void SetVertex(void);										// 頂点情報の設定
	void SetVtxColor(const D3DXCOLOR& col);						// 頂点の色設定処理
	void SetVtxColor(const int nVtx, const D3DXCOLOR& col1, const D3DXCOLOR& col2);			// 頂点の色(一定範囲)設定処理
	void SetVtxTexture(void);									// テクスチャ座標の設定処理
	void SetVertexAll(void);									// 全ての頂点情報の設定

	// セット・ゲット関係
	void SetPos(const D3DXVECTOR3& pos);	// 位置設定処理
	D3DXVECTOR3 GetPos(void) const;			// 位置取得処理

	void SetPosOld(const D3DXVECTOR3& posOld);	// 前回の位置設定処理
	D3DXVECTOR3 GetPosOld(void) const;			// 前回の位置取得処理

	void SetSize(const float size);			// サイズ設定処理
	float GetSize(void) const;				// サイズ取得処理

	void SetDistance(const float fDist);	// 距離の設定処理
	float GetDistance(void) const;			// 距離の取得処理

	void SetDivi(const int nDivi);			// 分割数の設定処理
	int GetDivi(void) const;				// 分割数の取得処理

	void SetNumVtx(void);					// 総頂点数の設定処理
	int GetNumVtx(void) const;				// 総頂点数の取得処理

	int GetTexIdx(void) const;				// テクスチャのインデックスの取得処理

	// 静的メンバ関数
	static CDonut2D* Create(const D3DXVECTOR3& pos, const float fSize, const float fDist, const int nDivi);		// 生成処理

private:

	// メンバ変数
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_posOld;						// 前回の位置
	float m_fSize;								// サイズ
	float m_fDistance;							// 距離
	int m_nDivi;								// 分割数
	int m_nNumVtx;								// 総頂点数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファのポインタ
	int m_nTexIdx;								// テクスチャのインデックス
};

#endif