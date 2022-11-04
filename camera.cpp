//==================================================
//
// カメラの処理
// Author:Teruto Sato
//
//==================================================

//----------------------
// インクルード
//----------------------
#include <stdlib.h>
#include "camera.h"
#include "input.h"
#include "input_keybord.h"
#include "renderer.h"
#include "game.h"

//----------------------
// 静的メンバ変数宣言
//----------------------
const float CCamera::fTurnSpeed = 0.02f;	//旋回速度
const float CCamera::fMoveSpeed = 3.0f;		//移動速度

//==================================================
// コンストラクタ
//==================================================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//注視点
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//目的の視点
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//目的の注視点
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;	//目的の角度
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//上方向ベクトル
	m_fDistance = 0.0f;		//距離
	POLOR_X = 0.0f;			//極座標のX
	POLOR_Y = 0.0f;			//極座標のY
	POLOR_Z = 0.0f;			//極座標のZ
}

//==================================================
// デストラクタ
//==================================================
CCamera::~CCamera()
{

}

//==================================================
// カメラの初期化処理
//==================================================
HRESULT CCamera::Init(void)
{
	//---------------------------------
	// 初期値の設定
	//---------------------------------
	m_posV = D3DXVECTOR3(0.0f, 200.0f, -400.0f);	//視点
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//上方向
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;

	{
		//----------------------------
		// 距離の設定(三平方の定理)
		//----------------------------
		float X = m_posR.x - m_posV.x;
		float Y = m_posR.y - m_posV.y;
		float Z = m_posR.z - m_posV.z;
		m_fDistance = sqrtf((X * X) + (Y * Y) + (Z * Z));

		//----------------------------
		// 角度の設定
		//----------------------------
		float angle = -atan2f(Y, Z) + D3DX_PI * 0.5f;

		//----------------------------
		// 向きの初期化
		//----------------------------
		m_rot = D3DXVECTOR3(angle, 0.0f, 0.0f);
		m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	return S_OK;
}

//==================================================
// カメラの終了処理
//==================================================
void CCamera::Uninit(void)
{

}

//==================================================
// カメラの更新処理
//==================================================
void CCamera::Update(void)
{
	//------------------------
	// 極座標のXYZ
	//------------------------
	POLOR_X = sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	POLOR_Y = cosf(m_rot.x) * m_fDistance;
	POLOR_Z = sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	//------------------------
	// 角度の正規化
	//------------------------
	//左右
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
}

//==================================================
// カメラの設定処理
// 引数 : デバイス
//==================================================
void CCamera::SetCamera(LPDIRECT3DDEVICE9 pDevice)
{
	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
					   &m_posV,
					   &m_posR,
					   &m_vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
							   D3DXToRadian(45.0f),
							   (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
							   10.0f,
							   1000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//==================================================
// 視点・注視点の設定
// 引数 : 位置
//==================================================
void CCamera::SetPos(D3DXVECTOR3 pos)
{
	m_posR = pos;	//注視点
	m_posV = m_posR + D3DXVECTOR3(0.0f, 200.0f, -400.0f);	//視点
}

//==================================================
// ビューポートの大きさ設定
// 引数 : 画面左上の座標X,Y、幅、高さ
//==================================================
void CCamera::SetViewSize(DWORD X, DWORD Y, int fWidth, int fHeight)
{
	//引数を代入
	m_viewport.X = X;
	m_viewport.Y = Y;
	m_viewport.Width = fWidth;
	m_viewport.Height = fHeight;
}

//==================================================
// ビューポートの拡縮
// 引数 : 開始位置X、開始位置Y、幅、高さ
//==================================================
void CCamera::AddViewSize(DWORD X, DWORD Y, int fWidth, int fHeight)
{
	//-------------------
	// 幅の加算
	//-------------------
	if (m_viewport.Width < SCREEN_WIDTH)
	{//幅がスクリーン内なら
		m_viewport.Width += fWidth;

		if (m_viewport.X > 0)
		{
			m_viewport.X += X;
		}
	}

	//-------------------
	// 高さの加算
	//-------------------
	if (m_viewport.Height < SCREEN_HEIGHT)
	{//幅がスクリーン内なら
		m_viewport.Height += fHeight;

		if (m_viewport.Y > 0)
		{
			m_viewport.Y += Y;
		}
	}
}

//==================================================
// アスペクト比の設定
// 引数 : デバイス、視野角、アスペクト比(幅、高さ)
//==================================================
void CCamera::SetAspect(LPDIRECT3DDEVICE9 pDevice, float fov, float fWidth, float fHeight)
{
	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(fov),
								fWidth / fHeight,
								10.0f,
								1000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//==================================================
// 視点の取得
//==================================================
D3DXVECTOR3 CCamera::GetPosV()
{
	return m_posV;
}

//==================================================
// 角度の取得
//==================================================
D3DXVECTOR3 CCamera::GetRot()
{
	return m_rot;
}

//==================================================
// ビューポートの取得
//==================================================
D3DVIEWPORT9 CCamera::GetViewport()
{
	return m_viewport;
}