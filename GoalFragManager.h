//===================================================
//
// ゴール関係のフラグ
// Author : Arita Meigen
//
//===================================================
#ifndef _GOALFRAGMANAGER_H_
#define _GOALFRAGMANAGER_H_	

//-------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>
#include "application.h"
#include"objectX.h"

//--------------------------------
// 前方宣言
//--------------------------------
class CShadow;	//影

//================================
// フェードクラスの定義
//================================
class CGoalFragManager
{
public:
	CGoalFragManager();	//コンストラクタ
	~CGoalFragManager();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	bool Collision();
	static CGoalFragManager * Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	//ゲッター
	bool GetGoalFrag();

private:
	//------------------
	// メンバ変数
	//------------------
	bool m_bGoal;
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_posold;		//1F前の位置
	D3DXVECTOR3 m_move;			//移動量
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_rotDest;		//目的の向き
	LPCTSTR m_modelName;		//モデルのパス


	/* ↓ クラス情報 ↓ */
	LPD3DXMESH m_pMesh;			//メッシュ情報へのポインタ
	LPD3DXBUFFER m_pBuffMat;	//マテリアル情報へのポインタ
	DWORD m_nNumMat;			//マテリアル情報の数
	D3DXVECTOR3 m_vtxMax;			//モデルの最大座標
	D3DXVECTOR3 m_vtxMin;			//モデルの座標の最小値

};

#endif
