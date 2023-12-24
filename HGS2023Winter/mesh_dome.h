//=====================================================
//
// メッシュドームヘッダー [mesh_dome.h]
// Author：小原立暉
//
//=====================================================
#ifndef _MESH_DOME_H_			// このマクロ定義がされていない場合
#define _MESH_DOME_H_			// 二重インクルード防止のマクロを定義する

//*****************************************************
// インクルードファイル
//*****************************************************
#include "Objectmesh.h"

//-----------------------------------------------------
// クラス定義(メッシュ)
//-----------------------------------------------------
class CMeshDome : public CMesh
{
public:		// 誰でもアクセスできる

	CMeshDome();				// コンストラクタ
	CMeshDome(CObject::TYPE type, CObject::PRIORITY priority = PRIORITY_BG);			// オーバーロードコンストラクタ
	virtual ~CMeshDome();		// デストラクタ
	void Box(void);				// コンストラクタの箱

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void);		// 更新処理
	virtual void Draw(void);		// 描画処理

private:		// 自分だけアクセスできる

};


#endif