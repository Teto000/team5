//===================================================
//
// ブロック数ヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _NUM_BLOCK_H_
#define _NUM_BLOCK_H_	//二重インクルード防止

//-------------------------------
// インクルード
//-------------------------------
#include "object2d.h"

//-------------------------------
// 前方宣言
//-------------------------------
class CNumber;	//数値

//================================
// ブロック数クラス
//================================
class CNumBlock : public CObject2D
{
public:
	CNumBlock();		//コンストラクタ
	~CNumBlock();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	int AddNumber(int nNumber);	//数値の加算

	//------------------
	// 静的メンバ変数
	//------------------
	static CNumBlock* Create(D3DXVECTOR3 pos);

private:
	void SetNumber();	//数値の設定

private:
	//------------------
	// 定数
	//------------------
	static const int nMaxDigits = 2;	//最大桁数

	//------------------
	// メンバ変数
	//------------------
	D3DXVECTOR3	m_pos;					//位置
	int			m_nNumBlock;			//ブロックの数
	int m_aPosTexU[nMaxDigits];			//今の桁の数値
	CNumber*	m_pNumber[nMaxDigits];	//数値
};

#endif