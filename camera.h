//===================================================
//
// カメラヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _CAMERA_H_	//このマクロが定義されてなかったら
#define _CAMERA_H_	//2重インクルード防止のマクロ定義

//--------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>
#include "main.h"

//================================
// カメラクラスの定義
//================================
class CCamera
{
public:
	//------------------------------
	// プレイヤー番号の列挙型
	//------------------------------
	enum NUMPLAYER
	{
		NUMPLAYER_ONE = 0,
		NUMPLAYER_TWO,
		NUMPLAYER_THREE,
		NUMPLAYER_FOUR,
		NUMPLAYER_MAX
	};

	CCamera();	//コンストラクタ
	~CCamera();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	void Init();
	void Uninit();
	void Update();
	void SetCamera(LPDIRECT3DDEVICE9 pDevice);

	//------------------
	// セッター
	//------------------
	void SetPos(D3DXVECTOR3 pos);	//視点・注視点の設定
	void SetNumPlayer(int nNum);	//プレイヤー番号の設定
	void AddViewSize(DWORD X, DWORD Y, int fWidth, int fHeight);	//ビューポートの拡縮

	//------------------
	// ゲッター
	//------------------
	D3DXVECTOR3 GetPosV();		//視点の取得
	D3DXVECTOR3 GetRot();		//向きの取得
	D3DVIEWPORT9 GetViewport();	//ビューポートの取得

	//------------------
	// 静的メンバ関数
	//------------------
	static CCamera* Create(DWORD X, DWORD Y, DWORD Width, DWORD Height);

private:
	void EachPlayer();	//プレイヤーごとの処理
	void Turn(int nLeftKey,int nRightKey);	//旋回
	void Move(int nUpKey, int nDownKey, int nLeftKey, int nRightKey);	//移動
	void Following();				//追従

private:
	//------------------
	// 定数
	//------------------
	static const float fTurnSpeed;		//旋回速度
	static const float fMoveSpeed;		//移動速度

	//------------------
	// メンバ変数
	//------------------
	D3DXVECTOR3 m_posV;			//現在の視点
	D3DXVECTOR3 m_posR;			//現在の注視点
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_posVDest;		//目的の視点
	D3DXVECTOR3 m_posRDest;		//目的の注視点
	D3DXVECTOR3 m_rotDest;		//目的の角度
	D3DXVECTOR3 m_vecU;			//上方向ベクトル
	int m_nNumPlayer;			//プレイヤー番号
	float m_fDistance;			//距離
	float POLOR_X;				//極座標のX
	float POLOR_Y;				//極座標のY
	float POLOR_Z;				//極座標のZ
	D3DXMATRIX m_mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX m_mtxView;		//ビューマトリックス
	D3DVIEWPORT9 m_viewport;	//ビューポート
};

#endif