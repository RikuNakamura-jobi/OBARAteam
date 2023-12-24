//======================================================================================================================
//
// 起伏地面ヘッダー [mesh_elevation.h]
// Author：小原立暉
//
//======================================================================================================================
#ifndef _MESH_ELEVATION_H_			// このマクロ定義がされていない場合
#define _MESH_ELEVATION_H_			// 二重インクルード防止のマクロを定義する

#include "object.h"

//-----------------------------------------------------
// クラス定義(起伏地面)
//-----------------------------------------------------
class CElevation : public CObject
{
public:			// 誰でもアクセスできる

	CElevation();		// コンストラクタ
	~CElevation();		// デストラクタ

	// リスト構造関係
	void SetPrev(CElevation* pPrev);	// 前のポインタの設定処理
	void SetNext(CElevation* pNext);	// 後のポインタの設定処理
	CElevation* GetPrev(void) const;	// 前のポインタの設定処理
	CElevation* GetNext(void) const;	// 次のポインタの設定処理

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fSizeX, const float fSizeZ, const int nDiviX, const int nDiviZ);			// 情報の設定処理

	void SetVertex(void);		// フィールドの設定処理
	void SetNormalize(void);	// 法線の設定処理
	void SetIndex(void);		// インデックスの設定処理
	float ElevationCollision(const D3DXVECTOR3& pos);					// 起伏の当たり判定
	float ElevationCollision(const D3DXVECTOR3& pos, bool& bRange);		// 起伏の当たり判定(範囲判定付き)
	int NearVertexSearch(const D3DXVECTOR3& pos);			// 近くの頂点を探す処理

	void BindTexture(int nIdx);		// テクスチャの割り当て処理

	// セット・ゲット関係
	D3DXVECTOR3 GetPos(void) const;			// 位置の取得処理
	D3DXVECTOR3 GetSize(void) const;		// サイズの取得処理
	int GetVtxX(void) const;				// X軸の頂点数の取得処理
	int GetVtxZ(void) const;				// Z軸の頂点数の取得処理
	int GetVtxNum(void) const;				// 全頂点数の取得処理
	void SetVtxHeight(const int nNum, const float fHeight);	// 頂点の設定処理
	void AddVertex(const int nNum, const float fAdd);		// 頂点の加算処理

	// 静的メンバ変数
	static CElevation* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fSizeX, const float fSizeZ, const int nDiviX, const int nDiviZ, char* texturename);		// 生成処理
	static void TxtSet(void);					// テキスト読み込み処理

private:		// 自分だけアクセスできる

	// メンバ変数
	D3DXVECTOR3	m_pos;			// 位置
	D3DXVECTOR3	m_rot;			// 向き
	D3DXVECTOR3	m_size;			// サイズ
	D3DXVECTOR3	m_sizeDivi;		// 1つ当たりの面のサイズ
	D3DXMATRIX	m_mtxWorld;		// ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;		// インデックスバッファへのポインタ
	int		m_nVtxX;			// 横の頂点数
	int		m_nVtxZ;			// 奥行の頂点数
	int		m_nDiviX;			// 横の分割数
	int		m_nDiviZ;			// 奥行の分割数
	int		m_nNumVtx;			// 総頂点数
	int		m_nNumIdx;			// 総インデックス数
	int		m_nTexIdx;			// テクスチャのインデックス

	// リスト構造関係
	CElevation* m_pPrev;	// 前へのポインタ
	CElevation* m_pNext;	// 次へのポインタ
};


#endif