//===================================================
//
// マップ
// Author : Arita Meigen
//
//===================================================
#ifndef _MAP_H_
#define _MAP_H_	

//-------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>
#include "application.h"
#include "model.h"
#include "object.h"

//================================
// フェードクラスの定義
//================================
class CMap :CObject
{
public:
	explicit CMap(int nPriority);	//コンストラクタ
	~CMap();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(D3DXVECTOR3 pos)override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CMap * Create(D3DXVECTOR3 pos);

	//セッター
	void SetPosition(D3DXVECTOR3 pos);

	//ゲッター
	D3DXVECTOR3 GetPosition();
	float GetWidth()override;
	float GetHeight()override;

private:
	//----------------
	// メンバ変数
	//----------------
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_rot;			//向き
	CModel* m_pModel;			//親モデルへのポインタ
	LPCTSTR m_modelName;		//モデルのパス

	/* ↓ モデル情報 ↓ */
	LPD3DXMESH m_pMesh;			//メッシュ情報へのポインタ
	LPD3DXBUFFER m_pBuffMat;	//マテリアル情報へのポインタ
	DWORD m_nNumMat;			//マテリアル情報の数
	D3DXVECTOR3 vtxMax;			//モデルの最大座標
	D3DXVECTOR3 vtxMin;			//モデルの座標の最小値

};

#endif

