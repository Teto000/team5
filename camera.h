//===================================================
//
// カメラヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _CAMERA_H_	//このマクロが定義されてなかったら
#define _CAMERA_H_	//2重インクルード防止のマクロ定義

//------------------------------
// インクルード
//------------------------------
#include <d3dx9.h>
#include "main.h"

//------------------------------
// 前方宣言
//------------------------------
class CCameraPlayer;	//プレイヤークラス

//==================================================
// カメラクラスの定義
//==================================================
class CCamera
{
public:
	CCamera();			//コンストラクタ
	virtual ~CCamera();	//デストラクタ

	//------------------------------
	// メンバ関数
	//------------------------------
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void SetCamera(LPDIRECT3DDEVICE9 pDevice);

	//------------------------------
	// セッター
	//------------------------------
	void SetPos(D3DXVECTOR3 pos);									//視点・注視点の設定
	void SetViewSize(DWORD X, DWORD Y, int fWidth, int fHeight);	//ビューポートの大きさ設定
	void AddViewSize(DWORD X, DWORD Y, int fWidth, int fHeight);	//ビューポートの拡縮
	void SetAspect(LPDIRECT3DDEVICE9 pDevice, float fov,
					float fWidth, float fHeight);					//アスペクト比の設定

	//------------------------------
	// ゲッター
	//------------------------------
	D3DXVECTOR3 GetPosV();		//視点の取得
	D3DXVECTOR3 GetRot();		//向きの取得
	D3DVIEWPORT9 GetViewport();	//ビューポートの取得

protected:
	//------------------------------
	// 定数
	//------------------------------
	static const float fTurnSpeed;	//旋回速度
	static const float fMoveSpeed;	//移動速度

	//------------------------------
	// メンバ変数
	//------------------------------
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