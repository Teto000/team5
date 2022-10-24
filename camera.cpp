//===================================================
//
// カメラの処理
// Author:Teruto Sato
//
//===================================================

//----------------------
// インクルード
//----------------------
#include "camera.h"
#include "input.h"
#include "input_keybord.h"
#include "renderer.h"

//===========================
// コンストラクタ
//===========================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDistance = 0.0f;
	m_TSPEED = 0.0f;
	m_MSPEED = 0.0f;
	POLOR_X = 0.0f;
	POLOR_Y = 0.0f;
	POLOR_Z = 0.0f;
}

//===========================
// デストラクタ
//===========================
CCamera::~CCamera()
{

}

//========================
// カメラの初期化処理
//========================
void CCamera::Init(void)
{
	//---------------------------------
	// 視点・注視点・上方向を設定する
	//---------------------------------
	m_posV = D3DXVECTOR3(0.0f, 200.0f, -400.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_TSPEED = 0.02f;		//カメラの回転速度(Turn)
	m_MSPEED = 3.0f;		//カメラの移動速度(Move)

	//--------------------------
	// ビューポート構成の保存
	//--------------------------
	m_viewport.X = 0;			//ビューポートの開始位置(X)
	m_viewport.Y = 0;			//ビューポートの開始位置(Y)
	m_viewport.Width = 1280;	//ビューポートの幅
	m_viewport.Height = 720;	//ビューポートの高さ
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
}

//========================
// カメラの終了処理
//========================
void CCamera::Uninit(void)
{

}

//========================
// カメラの更新処理
//========================
void CCamera::Update(void)
{
	//------------------
	// 極座標のXYZ
	//------------------
	POLOR_X = sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	POLOR_Y = cosf(m_rot.x) * m_fDistance;
	POLOR_Z = sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	//------------------
	// カメラの旋回
	//------------------
	Turn();

	//------------------
	// カメラの移動
	//------------------
	Move();

	//------------------
	// 角度の正規化
	//------------------
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

//========================
// カメラの設定処理
//========================
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

//========================
// カメラの旋回
//========================
void CCamera::Turn()
{
	//------------------
	// 注視点の旋回
	//------------------
	if (CInputKeyboard::Press(DIK_Q))	//左回転
	{//Qキーが押された
		m_rot.y -= m_TSPEED;			//回転量の増加
		m_posR.x = m_posV.x + POLOR_X;	//xの距離
		m_posR.y = m_posV.y + POLOR_Y;	//yの距離
		m_posR.z = m_posV.z + POLOR_Z;	//zの距離
	}
	else if (CInputKeyboard::Press(DIK_E))	//右回転
	{//Eキーが押された
		m_rot.y += m_TSPEED;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	if (CInputKeyboard::Press(DIK_Y))	//上回転
	{//Yキーが押された
		m_rot.x -= m_TSPEED;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	else if (CInputKeyboard::Press(DIK_B))	//下回転
	{//Bキーが押された
		m_rot.x += m_TSPEED;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}

	//------------------
	// 視点の旋回
	//------------------
	if (CInputKeyboard::Press(DIK_LEFT))	//左回転
	{//左キーが押された
		m_rot.y += m_TSPEED;			//回転量の増加
		m_posV.x = m_posR.x - POLOR_X;	//xの距離
		m_posV.y = m_posR.y - POLOR_Y;	//yの距離
		m_posV.z = m_posR.z - POLOR_Z;	//zの距離
	}
	else if (CInputKeyboard::Press(DIK_RIGHT))	//右回転
	{//右キーが押された
		m_rot.y -= m_TSPEED;
		m_posV.x = m_posR.x - POLOR_X;
		m_posV.y = m_posR.y - POLOR_Y;
		m_posV.z = m_posR.z - POLOR_Z;
	}
	if (CInputKeyboard::Press(DIK_UP))	//上回転
	{//上キーが押された
		if (m_rot.x <= 3.0f)
		{
			m_rot.x += m_TSPEED;
			m_posV.x = m_posR.x - POLOR_X;
			m_posV.y = m_posR.y - POLOR_Y;
			m_posV.z = m_posR.z - POLOR_Z;
		}
	}
	else if (CInputKeyboard::Press(DIK_DOWN))	//下回転
	{//下キーが押された
		if (m_rot.x >= 1.7f)
		{
			m_rot.x -= m_TSPEED;
			m_posV.x = m_posR.x - POLOR_X;
			m_posV.y = m_posR.y - POLOR_Y;
			m_posV.z = m_posR.z - POLOR_Z;
		}
	}
}

//========================
// カメラの移動
//========================
void CCamera::Move()
{
	if (CInputKeyboard::Press(DIK_A))
	{//Aキーが押された
		if (CInputKeyboard::Press(DIK_W))
		{//Wキーが押された
			m_posV.x += sinf(m_rot.y - D3DX_PI * 0.25f) * m_MSPEED;	//左奥移動
			m_posV.z += cosf(m_rot.y - D3DX_PI * 0.25f) * m_MSPEED;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else if (CInputKeyboard::Press(DIK_S))
		{//Sキーが押された
			m_posV.x += sinf(m_rot.y - D3DX_PI * 0.75f) * m_MSPEED;	//左前移動
			m_posV.z += cosf(m_rot.y - D3DX_PI * 0.75f) * m_MSPEED;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else
		{
			m_posV.x -= sinf(m_rot.y + D3DX_PI * 0.5f) * m_MSPEED;	//左移動
			m_posV.z -= cosf(m_rot.y + D3DX_PI * 0.5f) * m_MSPEED;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
	}
	else if (CInputKeyboard::Press(DIK_D))
	{//Dキーが押された
		if (CInputKeyboard::Press(DIK_W))
		{//Wキーが押された
			m_posV.x += sinf(m_rot.y + D3DX_PI * 0.25f) * m_MSPEED;	//右奥移動
			m_posV.z += cosf(m_rot.y + D3DX_PI * 0.25f) * m_MSPEED;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else if (CInputKeyboard::Press(DIK_S))
		{//Sキーが押された
			m_posV.x += sinf(m_rot.y + D3DX_PI * 0.75f) * m_MSPEED;	//右前移動
			m_posV.z += cosf(m_rot.y + D3DX_PI * 0.75f) * m_MSPEED;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else
		{
			m_posV.x += sinf(m_rot.y + D3DX_PI * 0.5f) * m_MSPEED;	//右移動
			m_posV.z += cosf(m_rot.y + D3DX_PI * 0.5f) * m_MSPEED;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
	}
	else if (CInputKeyboard::Press(DIK_W))
	{//Wキーが押された
		m_posV.x -= sinf(m_rot.y + D3DX_PI * 1.0f) * m_MSPEED;	//奥移動
		m_posV.z -= cosf(m_rot.y + D3DX_PI * 1.0f) * m_MSPEED;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	else if (CInputKeyboard::Press(DIK_S))
	{//Sキーが押された
		m_posV.x -= sinf(m_rot.y + D3DX_PI * 0.0f) * m_MSPEED;	//前移動
		m_posV.z -= cosf(m_rot.y + D3DX_PI * 0.0f) * m_MSPEED;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	if (CInputKeyboard::Press(DIK_O))	//上昇
	{//Oキーが押された
		m_posV.y -= tanf(m_rot.x + D3DX_PI * 1.0f) * m_MSPEED;	//上移動
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	else if (CInputKeyboard::Press(DIK_L))	//下降
	{//Lキーが押された
		m_posV.y += tanf(m_rot.x + D3DX_PI * 0.0f) * m_MSPEED;	//上移動
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
}

//========================
// 視点の取得
//========================
D3DXVECTOR3 CCamera::GetPosV()
{
	return m_posV;
}

//========================
// 角度の取得
//========================
D3DXVECTOR3 CCamera::GetRot()
{
	return m_rot;
}