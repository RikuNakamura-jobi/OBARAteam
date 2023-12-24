//======================================================================================================================
//
// メッシュヘッダー [mesh.h]
// Author：小原立暉
//
//======================================================================================================================
#ifndef _MESH_H_			// このマクロ定義がされていない場合
#define _MESH_H_			// 二重インクルード防止のマクロを定義する

#include "object.h"
#include "objectX.h"

// クラス定義(メッシュ)
class CMesh : public CObject
{
public:			// 誰でもアクセスできる

	// 構造体定義(方眼情報)
	struct SGrid
	{
		int x;			// 位置(X軸)
		int y;			// 位置(Y軸)
		int z;			// 位置(Z軸)
	};

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_FIELD = 0,		// フィールド
		TYPE_WALL,			// 壁
		TYPE_CYLINDER,		// 筒
		TYPE_DOME,			// ドーム
		TYPE_MAX			// この列挙型の総数
	};

	CMesh();			// コンストラクタ
	CMesh(CObject::TYPE type, CObject::PRIORITY priority = PRIORITY_BG);			// オーバーロードコンストラクタ
	virtual ~CMesh();	// デストラクタ
	void Box(void);		// コンストラクタの箱

	// リスト構造関係
	void SetPrev(CMesh* pPrev);	// 前のポインタの設定処理
	void SetNext(CMesh* pNext);	// 後のポインタの設定処理
	CMesh* GetPrev(void) const;	// 前のポインタの設定処理
	CMesh* GetNext(void) const;	// 次のポインタの設定処理

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void);		// 更新処理
	virtual void Draw(void);		// 描画処理

	void DrawProcess(void);			// 描画処理の中身

	void SetDataField(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const SGrid& Divi);			// フィールドの情報設定処理
	void SetDataWall(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const SGrid& Divi);			// 壁の情報設定処理
	void SetDataCylinder(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fCircumSize, const float fHeightSize, const int nCircum, const int nHeight);			// 筒の情報設定処理
	void SetDataDome(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fCircumSize, const float fHeightSize, const int nCircum, const int nHeight);				// ドームの情報設定処理

	void SetVertexField(void);		// フィールドの設定処理
	void SetVertexWall(void);		// 壁の設定処理
	void SetVertexCylinder(void);	// 筒の設定処理
	void SetVertexDome(void);		// ドームの設定処理

	void SetIndex(const int nNumVtx1, const int nNumVtx2);		// インデックスの設定処理

	void BindTexture(int nIdx);		// テクスチャの割り当て処理

	// セット・ゲット関係
	void SetPos(const D3DXVECTOR3& pos);		// 位置の設定処理
	D3DXVECTOR3 GetPos(void) const;				// 位置の取得処理

	void SetRot(const D3DXVECTOR3& rot);		// 向きの設定処理
	D3DXVECTOR3 GetRot(void) const;				// 向きの取得処理

	void SetSize(const D3DXVECTOR3& size);		// 拡大率の設定処理
	D3DXVECTOR3 GetSize(void) const;			// 拡大率の取得処理

	void SetLighting(const bool bLighting);		// ライティングの設定処理

	// 静的メンバ変数
	static CMesh* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size, const SGrid& Divi, const TYPE type, char* texturename, const bool bLighting);		// 生成処理
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
	TYPE	m_type;				// 種類
	SGrid	m_vtx;				// 頂点数
	SGrid	m_divi;				// 分割数
	int		m_nNumVtx;			// 総頂点数
	int		m_nNumIdx;			// 総インデックス数
	int		m_nTexIdx;			// テクスチャのインデックス
	bool	m_bLightOff;		// ライティング状況

	// リスト構造関係
	CMesh* m_pPrev;	// 前へのポインタ
	CMesh* m_pNext;	// 次へのポインタ
};


#endif