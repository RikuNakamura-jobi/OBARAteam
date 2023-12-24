//============================================
//
// オブジェクト2Dヘッダー[object2D.h]
// Author：小原立暉
//
//============================================
#ifndef _OBJECT2D_H_//このマクロ定義がされていなかったら
#define _OBJECT2D_H_//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "object.h"

//--------------------------------------------
// クラス(オブジェクト2Dクラス)
//--------------------------------------------
class CObject2D : public CObject
{
public:			// 誰でもアクセスできる

	// 列挙型定義(TYPE)
	enum TYPE
	{
		TYPE_NONE = 0,		// 通常の2Dポリゴン
		TYPE_ANIM,			// アニメーション
		TYPE_SCROLL,		// スクロール(背景等に使用)
		TYPE_MAX			// この列挙型の総数
	};

	CObject2D();						// コンストラクタ
	CObject2D(CObject::TYPE type, PRIORITY priority = PRIORITY_ENTITY);		// オーバーロードコンストラクタ
	virtual ~CObject2D();				// デストラクタ
	void Box(void);						// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void);		// 更新処理
	virtual void Draw(void);		// 描画処理

	void BindTexture(int nIdx);				// テクスチャの割り当て処理

	// 頂点操作系
	void SetVertex(void);										// 頂点情報の設定
	void SetVertexRot(void);									// 頂点情報の設定(傾き有りバージョン)
	void SetVtxWidthGage(void);									// 頂点情報の設定(横ゲージ表示バージョン)
	void SetVtxUnderHeightGage(void);							// 頂点情報の設定(下中心ゲージ表示バージョン)
	void SetVtxUnderRot(void);									// 頂点情報の設定(下中心傾き有りバージョン)
	void SetVtxHardCoding(const D3DXVECTOR3& LeftUp, const D3DXVECTOR3& RightUp, const D3DXVECTOR3& LeftDown, const D3DXVECTOR3& RightDown);	// 頂点情報の設定(ベタ打ち)
	void SetVtxColor(const D3DXCOLOR& col);						// 頂点の色設定処理
	void SetVtxTexture(void);									// テクスチャの設定
	void SetVtxTextureAnim(float fTexPattern, int nPattern);	// テクスチャの設定(アニメーションバージョン)
	void SetVtxTextureScroll(D3DXVECTOR2& tex);					// テクスチャの設定(スクロールバージョン)
	void SetVtxTextureWidth(const float tex);					// テクスチャの設定(横のテクスチャ任意設定)
	void SetVtxTextureUnderHeight(const float tex);				// テクスチャの設定(下中心の縦のテクスチャ任意設定)
	void SetVertexAll(void);									// 全ての頂点情報の設定

	// セット・ゲット関係
	void SetPos(const D3DXVECTOR3& pos);	// 位置設定処理
	D3DXVECTOR3 GetPos(void) const;			// 位置取得処理

	void SetPosOld(const D3DXVECTOR3& posOld);	// 前回の位置設定処理
	D3DXVECTOR3 GetPosOld(void) const;			// 前回の位置取得処理

	void SetRot(const D3DXVECTOR3& rot);	// 向き設定処理
	D3DXVECTOR3 GetRot(void) const;			// 向き取得処理

	void SetSize(const D3DXVECTOR3& size);	// サイズ設定処理
	D3DXVECTOR3 GetSize(void) const;		// サイズ取得処理

	void SetLength(void);					// 長さ設定処理
	float GetLength(void) const;			// 長さ設定処理

	void SetAngle(void);					// 方向設定処理
	float GetAngle(void) const;				// 方向設定処理

	int GetTexIdx(void) const;				// テクスチャのインデックスの取得処理

	// 静的メンバ関数
	static CObject2D* Create(const TYPE type2D, const CObject::TYPE type, const PRIORITY priority);		// 生成処理

protected:		// 自分と派生クラスだけアクセスできる

private:

	// メンバ変数
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_posOld;						// 前回の位置
	D3DXVECTOR3 m_rot;							// 向き
	D3DXVECTOR3 m_size;							// サイズ
	float m_fAngle;								// 方向
	float m_fLength;							// 長さ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファのポインタ
	int m_nTexIdx;								// テクスチャのインデックス
};

#endif