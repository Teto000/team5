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
	// ゲッター
	//------------------
	D3DXVECTOR3 GetPosV();		//視点の取得
	D3DXVECTOR3 GetRot();		//向きの取得
	D3DVIEWPORT9 GetVieport();	//ビューポートの取得

	//------------------
	// 静的メンバ関数
	//------------------
	static CCamera* Create();
	static const int GetMaxCamera();	//最大数の取得

private:
	void Turn();	//旋回
	void Move();	//移動

private:
	//------------------
	// 定数
	//------------------
	static const int nMaxCamera = 2;	//カメラの最大数
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
	float m_fDistance;			//距離
	float POLOR_X;				//極座標のX
	float POLOR_Y;				//極座標のY
	float POLOR_Z;				//極座標のZ
	D3DXMATRIX m_mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX m_mtxView;		//ビューマトリックス
	D3DVIEWPORT9 m_viewport;	//ビューポート
};

#endif