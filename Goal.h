//===================================================
//
// ゴール関係のフラグ
// Author : Arita Meigen
//
//===================================================
#ifndef _GOAL_H_
#define _GOAL_H_	

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
// ゴールクラスの定義
//================================
class CGoal : public CObject
{
public:
	explicit CGoal(int nPriority);	//コンストラクタ
	~CGoal();						//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit();
	void Update();
	void Draw();
	bool Collision();
	static CGoal * Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	//セッター
	void SetPosition(D3DXVECTOR3 pos);

	//ゲッター
	static bool GetGoalFrag();
	D3DXVECTOR3 GetPosition()override;
	float GetWidth()override;
	float GetHeight()override;
	static int GetWinner();	

private:
	//------------------
	// メンバ変数
	//------------------
	static bool m_bGoal;
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_posold;		//1F前の位置
	D3DXVECTOR3 m_move;			//移動量
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_rotDest;		//目的の向き
	LPCTSTR		m_modelName;	//モデルのパス
	static int	m_nWinner;		//優勝したやつの番号

	/* ↓ クラス情報 ↓ */
	LPD3DXMESH m_pMesh;			//メッシュ情報へのポインタ
	LPD3DXBUFFER m_pBuffMat;	//マテリアル情報へのポインタ
	DWORD m_nNumMat;			//マテリアル情報の数
	D3DXVECTOR3 m_vtxMax;			//モデルの最大座標
	D3DXVECTOR3 m_vtxMin;			//モデルの座標の最小値

};

#endif
