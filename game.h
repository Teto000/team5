//===================================================
//
// ゲームヘッダー
// Author : Sato Teruto
//
//====================================================
#ifndef _GAME_H_
#define _GAME_H_	

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
class CCamera;		//カメラ

//--------------------
// マクロ定義
//--------------------
#define nDefaultMaxCamera	(4)
#define MAX_PLAYER (4)

//================================
// ゲームクラスの定義
//================================
class CGame
{
public:
	//カメラの最大数
	enum NUMCAMERA
	{
		NUMCAMERA_ONE = 0,
		NUMCAMERA_TWO,
		NUMCAMERA_THREE,
		NUMCAMERA_FOUR,
		NUMCAMERA_MAX
	};

	CGame();	//コンストラクタ
	~CGame();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init();
	void Uninit();
	void Update();
	void SetPlayerPosition(D3DXVECTOR3 pos);
	void ResetCameraSize();	//カメラの大きさのリセット

	//------------------
	// ゲッター
	//------------------
	static CPlayer*	GetPlayer(int NumPlayer);			//プレイヤーの取得
	static CMeshField* GetMesh() { return pMeshField; }	//メッシュフィールドの取得
	static CCamera	 *GetCamera(int nCnt);				//カメラの取得

	//プレイヤー人数の取得
	static int GetAmount() { return nDefaultMaxCamera; }

private:
	void CreateCamera(CGame::NUMCAMERA num);	//カメラの生成
	void FinishGame();	//ゲーム終了時の処理

private:
	//------------------
	// 定数
	//------------------
	static const int nSpeed_X = 9;
	static const int nSpeed_Y = 5;

	//-------------------
	// 静的メンバ変数
	//-------------------
	static CPolygon* pPolygon;
	static CPolygon2d* pPolygon2d;
	static CPlayer*	 pPlayer[MAX_PLAYER];
	static CMeshField* pMeshField;
	static CCamera* m_pCamera[nDefaultMaxCamera];	//カメラクラス

	static int m_nNumCamera;	//カメラの列挙型番号
	static int m_nMaxCamera;	//カメラの数
	static bool m_bStop;		//プログラムを停止する
};

#endif