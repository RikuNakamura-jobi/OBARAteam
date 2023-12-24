//===================================
//
// オブジェクト3Dヘッダー[object3D.h]
// Author 小原立暉
//
//===================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "object.h"

//--------------------------------------------
// クラス(オブジェクト3Dクラス)
//--------------------------------------------
class CObject3D : public CObject
{
public:			// 誰でもアクセスできる

	CObject3D();						// コンストラクタ
	CObject3D(CObject::TYPE type, PRIORITY priority = PRIORITY_ENTITY);		// オーバーロードコンストラクタ
	virtual ~CObject3D();				// デストラクタ
	void Box(void);						// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void);			// 初期化処理
	virtual void Uninit(void);			// 終了処理
	virtual void Update(void);			// 更新処理
	virtual void Draw(void);			// 描画処理

	void DrawLightOff(void);	// 描画処理(ライティング無し)

	void BindTexture(int nIdx);				// テクスチャの割り当て処理

	// 頂点操作系
	void SetVertex(void);									// 頂点情報の設定
	void SetVertexHardCoding(const D3DXVECTOR3& VtxMax, const D3DXVECTOR3& VtxMin);			// 頂点情報の設定(決め打ち)
	void SetVertexElev(float fHeight);						// 起伏のある頂点情報の設定
	void SetVtxColor(const D3DXCOLOR& col);					// 頂点カラーの設定処理
	void SetVtxTextureWidth(const D3DXVECTOR2& size);		// 頂点テクスチャの設定処理(縦の一定間隔のテクスチャ配分)
	void SetVtxTextureHeight(const D3DXVECTOR2& size);		// 頂点テクスチャの設定処理(横の一定間隔のテクスチャ配分)

	// セット・ゲット関係
	void SetPos(const D3DXVECTOR3& pos);	// 位置設定処理
	D3DXVECTOR3 GetPos(void) const;			// 位置取得処理

	void SetPosOld(const D3DXVECTOR3& posOld);	// 前回の位置設定処理
	D3DXVECTOR3 GetPosOld(void) const;			// 前回の位置取得処理

	void SetRot(const D3DXVECTOR3& rot);	// 向き設定処理
	D3DXVECTOR3 GetRot(void) const;			// 向き取得処理

	void SetSize(const D3DXVECTOR3& size);	// サイズ設定処理
	D3DXVECTOR3 GetSize(void) const;		// サイズ取得処理

	D3DXMATRIX GetMatrix(void) const;		// マトリックスの取得処理

	int GetTexIdx(void) const;				// テクスチャのインデックスの取得処理

	// 静的メンバ関数
	static CObject3D* Create(const TYPE type, const PRIORITY priority);		// 生成処理

	float ElevationCollision(const D3DXVECTOR3 pos);	// 起伏のあるポリゴンの当たり判定

private:

	// メンバ変数
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_posOld;						// 前回の位置
	D3DXVECTOR3 m_rot;							// 向き
	D3DXVECTOR3 m_size;							// サイズ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファのポインタ
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	int m_nTexIdx;								// テクスチャのインデックス
};

#endif