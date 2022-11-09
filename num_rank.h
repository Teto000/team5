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

//-------------------------------
// 前方宣言
//-------------------------------
class CNumber;	//数値

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
	static CRank* Create(D3DXVECTOR3 pos);

private:
	//------------------
	// メンバ変数
	//------------------
	D3DXVECTOR3	m_pos;					//位置
	int			m_nRank[MAX_PLAYER];	//順位
	int m_aPosTexU;						//今の桁の数値
	CNumber*	m_pNumber;				//数値
};

#endif