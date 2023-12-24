//============================================================
//
// キャラクターヘッダー [character.h]
// Author：小原立暉
//
//============================================================
#ifndef _CHARACTER_H_	// このマクロ定義がされていない場合
#define _CHARACTER_H_	// 二重インクルード防止のマクロを定義する

//************************************************************
// インクルードファイル
//************************************************************
#include "object.h"
#include "objectX.h"

#define MAX_PARTS		(20)				// パーツの最大数

//------------------------------------------------------------
// クラス定義(階層構造のモデル)
//------------------------------------------------------------
class CHierarchy
{
public:			// 誰でもアクセスできる

	CHierarchy();			// コンストラクタ
	~CHierarchy();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);			// 初期化処理
	void Uninit(void);			// 終了処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理
	void Draw(D3DXCOLOR col);	// 描画処理(色処理)
	void DrawShadow(void);		// モデル影の描画処理
	void Draw(float fAlpha);	// 描画処理(透明度処理)

	// セット・ゲット関数
	void SetPos(const D3DXVECTOR3& pos);	// 位置設定処理
	D3DXVECTOR3 GetPos(void) const;			// 位置取得処理

	void SetPosOld(const D3DXVECTOR3& posOld);	// 前回の位置設定処理
	D3DXVECTOR3 GetPosOld(void) const;			// 前回の位置取得処理

	void SetRot(const D3DXVECTOR3& rot);	// 向き設定処理
	D3DXVECTOR3 GetRot(void) const;			// 向き取得処理

	void SetScale(const D3DXVECTOR3& scale);	// 拡大率設定処理
	D3DXVECTOR3 GetScale(void) const;			// 拡大率取得処理

	void SetFileData(const CXFile::TYPE type);						// データの設定処理
	void SetFileData(const CXFile::SXFile filaData);				// データの設定処理(ファイルデータの設定版)

	D3DXMATRIX GetMatrix(void) const;			// マトリックスの取得処理

	CXFile::SXFile GetFileData(void);			// Xファイルのデータの取得処理

	void SetParent(CHierarchy* pModel);			// 親の設定処理
	CHierarchy* GetParent(void) const;			// 親の取得処理

	// 静的メンバ関数
	static CHierarchy* Create(void);			// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	D3DXVECTOR3		m_pos;			// 位置
	D3DXVECTOR3		m_posOld;		// 前回の位置
	D3DXVECTOR3		m_rot;			// 向き
	D3DXVECTOR3		m_scale;		// 拡大率
	D3DXMATRIX		m_mtxWorld;		// ワールドマトリックス
	CXFile::SXFile	m_XFileData;	// Xファイルのデータ
	CHierarchy*		m_apParent;		// 親モデルへのポインタ
};

//------------------------------------------------------------
// クラス定義(モデル)
//------------------------------------------------------------
class CCharacter : public CObject
{
public:			// 誰でもアクセスできる

	CCharacter();			// コンストラクタ
	CCharacter(CObject::TYPE type, PRIORITY priority = PRIORITY_ENTITY);		// オーバーロードコンストラクタ
	virtual ~CCharacter();	// デストラクタ
	void Box(void);			// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void);			// 初期化処理
	virtual void Uninit(void);			// 終了処理
	virtual void Update(void);			// 更新処理
	virtual void Draw(void);			// 描画処理

	void Draw(D3DXCOLOR col);	// 描画処理(色処理)
	void Draw(D3DXCOLOR* col);	// 描画処理(透明度調整)
	void DrawShadow(void);		// モデル影の描画処理
	void Draw(float fAlpha);	// 描画処理(透明度処理)

	// セット・ゲット関数
	void SetPos(const D3DXVECTOR3& pos);	// 位置設定処理
	D3DXVECTOR3 GetPos(void) const;			// 位置取得処理

	void SetPosOld(const D3DXVECTOR3& posOld);	// 前回の位置設定処理
	D3DXVECTOR3 GetPosOld(void) const;			// 前回の位置取得処理

	void SetRot(const D3DXVECTOR3& rot);	// 向き設定処理
	D3DXVECTOR3 GetRot(void) const;			// 向き取得処理

	void SetScale(const D3DXVECTOR3& scale);	// 拡大率設定処理
	D3DXVECTOR3 GetScale(void) const;			// 拡大率取得処理

	D3DXMATRIX GetMatrix(void) const;			// マトリックスの取得処理
	D3DXMATRIX* GetMatrixP(void);				// マトリックスの取得処理

	void SetData(void);							// データの設定処理

	CHierarchy* GetHierarchy(int nIdx);			// 階層モデルの取得処理
	CHierarchy** GetHierarchy(void);			// 階層モデルのダブルポインタの取得処理

	void SetNumModel(const int nNum);			// モデルの総数の設定処理
	int GetNumModel(void) const;				// モデルの総数の取得処理

	// 静的メンバ関数
	static CCharacter* Create(void);			// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	D3DXVECTOR3		m_pos;				// 位置
	D3DXVECTOR3		m_posOld;			// 前回の位置
	D3DXVECTOR3		m_rot;				// 向き
	D3DXVECTOR3		m_scale;			// 拡大率
	D3DXMATRIX		m_mtxWorld;			// ワールドマトリックス
	CHierarchy* m_apModel[MAX_PARTS];	// モデルの情報
	int m_nNumModel;					// モデルの総数
};

#endif