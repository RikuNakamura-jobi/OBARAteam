//===================================
//
// オブジェクト3Dファンヘッダー[object3Dfan.h]
// Author 小原立暉
//
//===================================
#ifndef _OBJECT3DFAN_H_
#define _OBJECT3DFAN_H_

#include "object.h"

//--------------------------------------------
// クラス(オブジェクト3Dクラス)
//--------------------------------------------
class CObject3DFan : public CObject
{
public:			// 誰でもアクセスできる

	CObject3DFan();			// コンストラクタ
	virtual ~CObject3DFan();		// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void);		// 更新処理
	virtual void Draw(void);		// 描画処理

	//void BindTexture(int nIdx);				// テクスチャの割り当て処理

	// 頂点操作系
	void SetVertex(void);						// 頂点情報の設定
	void SetVtxColor(const D3DXCOLOR& col);		// 頂点カラーの設定処理

	// 静的メンバ関数
	static CObject3DFan* Create(void);			// 生成処理

private:

	// メンバ変数
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_rot;							// 向き
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファのポインタ
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス

	int m_nNumAngle;							// 角度の数
	float m_fRadius;							// 半径
};

#endif