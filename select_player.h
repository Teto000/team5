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
class CObject2D;	//ポリゴン2d
class CPlayer;		//プレイヤー
class CMeshField;	//メッシュフィールド
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
	CUI_PSelect *m_BGUI;					// 背景のUI
};

#endif