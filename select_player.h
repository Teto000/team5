//===================================================
//
// プレイヤーの人数のセレクト.h
// Author : Tomoki Tomidokoro
//
//===================================================
#ifndef _SELECT_PLAYER_H_
#define _SELECT_PLAYER_H_	

#define MAX_PLAYER (4)
//--------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>
#include "application.h"

//--------------------------------
// 前方宣言
//--------------------------------
class CUI_PSelect;	//UI

//================================
// Pセレクトクラスの定義
//================================
class CPSelect
{
public:
	CPSelect();	//コンストラクタ
	~CPSelect();	//デストラクタ

//------------------
// メンバ関数
//------------------
	HRESULT Init();
	void Uninit();
	void Update();
	void SelectPlayerNum();
	void UI_Create();
	void UI_Uninit();
	//------------------
	// ゲッター
	//------------------

private:
	//-------------------
	// メンバ変数
	//-------------------
	int m_PlayerSelectNum;
	CUI_PSelect *m_PUI[MAX_PLAYER];			// プレイヤー数のUI
	CUI_PSelect *m_title;					// 背景のUI
	CUI_PSelect *m_BG;						// 背景
	CUI_PSelect *m_Select;					// 人数選択の説明
};

#endif