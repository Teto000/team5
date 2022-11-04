//===================================================
//
// タイトルロゴ
// Author : Arita Meigen
//
//===================================================
#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_	//二重インクルード防止

//-------------------------------
// インクルード
//-------------------------------
#include "object2d.h"

//================================
// メッセージクラス
//================================
class CTitlelogo : public CObject2D
{
public:

	CTitlelogo();		//コンストラクタ
	~CTitlelogo();		//デストラクタ

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
	static CTitlelogo* Create(D3DXVECTOR3 pos, CTexture::TEXTURE tex);

private:
	//------------------
	// メンバ変数
	//------------------
	D3DXVECTOR3 m_pos;	//位置
	D3DXCOLOR m_col;	//色
	float m_fWidth;		//幅
	float m_fHeight;	//高さ
	
};

#endif