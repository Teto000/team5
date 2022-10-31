//===================================================
//
// ゲームヘッダー
// Author : Sato Teruto
//
//====================================================
#ifndef _GAME_H_
#define _GAME_H_	

#define MAX_PLAYER (2)
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
// ゲームクラスの定義
//================================
class CGame
{
public:
	CGame();	//コンストラクタ
	~CGame();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init();
	void Uninit();
	void Update();
	void SetPlayerPosition(D3DXVECTOR3 pos);

	//------------------
	// ゲッター
	//------------------
	static CPlayer*	GetPlayer(int NumPlayer);	//プレイヤーの取得
	static CMeshField* GetMesh() { return pMeshField; }	//メッシュフィールドの取得

private:
	//-------------------
	// 静的メンバ変数
	//-------------------
	static CPolygon* pPolygon;
	static CPolygon2d* pPolygon2d;
	static CPlayer*	 pPlayer[MAX_PLAYER];
	static CMeshField* pMeshField;
};

#endif