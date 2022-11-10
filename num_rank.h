//===================================================
//
// 順位ヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _NUM_RANK_H_
#define _NUM_RANK_H_	//二重インクルード防止

//-------------------------------
// インクルード
//-------------------------------
#include "object2d.h"
#include "game.h"

//================================
// ブロック数クラス
//================================
class CRank : public CObject2D
{
public:
	CRank();	//コンストラクタ
	~CRank();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//------------------
	// 静的メンバ変数
	//------------------
	static CRank* Create(D3DXVECTOR3 pos, int nNumPlayer);

private:
	void SetTexture(int nNumPlayer);	//テクスチャの設定

private:
	//------------------
	// メンバ変数
	//------------------
	D3DXVECTOR3	m_pos;					//位置
	int			m_aPosTexU;				//今の桁の数値
	int			m_nNumPlayer;			//対応するプレイヤー番号

	//------------------
	// 静的メンバ変数
	//------------------
	static int	m_nRank[MAX_PLAYER];	//順位
	static int	m_nDistance[MAX_PLAYER];//距離
};

#endif