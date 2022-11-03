//===================================================
//
// メッセージヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _POLYGON2D_H_
#define _POLYGON2D_H_	//二重インクルード防止

//-------------------------------
// インクルード
//-------------------------------
#include "object2d.h"

//================================
// メッセージクラス
//================================
class CMessage : public CObject2D
{
public:
	CMessage();		//コンストラクタ
	~CMessage();	//デストラクタ

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
	static CMessage* Create(D3DXVECTOR3 pos);
};

#endif