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
class CPolygon;		//ポリゴン
class CPolygon2d;	//ポリゴン2d
class CPlayer;		//プレイヤー
class CMeshField;	//メッシュフィールド

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
//------------------
// ゲッター
//------------------

private:
//-------------------
// 静的メンバ変数
//-------------------
	static CPolygon* pPolygon;
	static CPolygon2d* pPolygon2d;
	int m_PlayerSelectNum;
};

#endif