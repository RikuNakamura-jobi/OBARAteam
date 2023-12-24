//=====================================================================================
//
//	軌跡ヘッダー [orbit.h]
//	Author：小原立暉
//
//=====================================================================================
#ifndef _ORBIT_H_			// このマクロ定義がされていない場合
#define _ORBIT_H_			// 二重インクルード防止のマクロを定義する

//*************************************************************************************
// インクルードファイル
//*************************************************************************************
#include "object.h"

//-------------------------------------------------------------------------------------
//	マクロ定義
//-------------------------------------------------------------------------------------
#define MAX_ORBIT_VTX		(160)			// 軌跡の最高頂点数

//-------------------------------------------------------------------------------------
// クラス定義(軌跡)
//-------------------------------------------------------------------------------------
class COrbit : CObject
{
public:			// 誰でもアクセスできる

	//-------------------------------------------------------------------------------------
	//	列挙型定義 (MATRIXPLACE)
	//-------------------------------------------------------------------------------------
	typedef enum
	{
		MATRIXPLACE_BOTTOM = 0,		// 下
		MATRIXPLACE_TOP,			// 上
		MATRIXPLACE_MAX				// この列挙型の総数
	}MATRIXPLACE;

	COrbit();						// コンストラクタ
	virtual ~COrbit();				// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void);		// 更新処理
	virtual void Draw(void);		// 描画処理

	void BindTexture(int nIdx);		// テクスチャの割り当て処理

	// セット・ゲット関係
	void SetParentData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);		// 親の位置の設定処理
	void PosSet(const D3DXVECTOR3& pos);									// 全頂点の位置の設定

	// 静的メンバ関数
	static COrbit* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3& rot, const int nTexIdx);		// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9 m_pTexture;					// テクスチャへのポインタ
	D3DXMATRIX m_mtxWorld;							// ワールドマトリックス
	D3DXVECTOR3 m_posParent;						// 親の位置
	D3DXVECTOR3 m_rotParent;						// 親の向き
	D3DXVECTOR3 m_aPosPoint[MAX_ORBIT_VTX];			// 計算後の各頂点座標
	D3DXCOLOR m_aColPoint[MAX_ORBIT_VTX];			// 計算後の各頂点カラー
};

#endif