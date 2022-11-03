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
	enum MESSAGE
	{
		MESSAGE_COUNT_ONE = 0,
		MESSAGE_COUNT_TWO,
		MESSAGE_COUNT_THREE,
		MESSAGE_START,
		MESSAGE_FINISH,
		MESSAGE_MAX
	};

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

private:
	void SetTexture();		//テクスチャの設定
	void ChangeMessage();	//メッセージの変更

private:
	//------------------
	// メンバ変数
	//------------------
	D3DXCOLOR m_col;	//色
	int m_nCntTime;		//時間カウント
	float m_fWidth;		//幅
	float m_fHeight;	//高さ
	MESSAGE m_message;	//メッセージ
};

#endif