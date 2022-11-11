//===================================================
//
// ブロックの処理
//	Auhotr : C・Gen・Gow
//
//===================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_	

//-------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>
#include "application.h"
#include "objectX.h"

//--------------------------------
// 前方宣言
//--------------------------------
class CShadow;	//影

//================================
// ゴールクラスの定義
//================================
class CPlayer;
class CBlock
{
public:
	explicit CBlock(int nPriority);	//コンストラクタ
	~CBlock();	//デストラクタ

//------------------
// メンバ関数
//------------------
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();
	bool Collision(CPlayer *pPlayer);

	// クリエイト
	static CBlock * Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	// ブロック数のカウント処理
	static int *AddBlock(int *BlockHave) { return BlockHave++; }		
	
	//セッター
	void SetAbove() { m_bAbove = true; }
	void SetAboveFalse() { m_bAbove = false; }
	void SetPosition(D3DXVECTOR3 pos);

	//ゲッター
	static bool GetBlockCollision() { return m_bAbove; }
	static bool GetHaveBlock() { return m_bHaveBlock; }

private:
	//------------------
	// メンバ変数
	//------------------
	static bool m_bAbove;
	static bool m_bHaveBlock;
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_posold;		//1F前の位置
	D3DXVECTOR3 m_move;			//移動量
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_rotDest;		//目的の向き
	LPCTSTR		m_modelName;	//モデルのパス

	/* ↓ クラス情報 ↓ */
	LPD3DXMESH	 m_pMesh;		//メッシュ情報へのポインタ
	LPD3DXBUFFER m_pBuffMat;	//マテリアル情報へのポインタ
	DWORD		 m_nNumMat;		//マテリアル情報の数
	D3DXVECTOR3  m_vtxMax;		//モデルの最大座標
	D3DXVECTOR3  m_vtxMin;		//モデルの座標の最小値
};

#endif
