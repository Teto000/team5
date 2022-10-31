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
#include "game.h"
#include "player.h"
#include "Goal.h"

//----------------------
// 静的メンバ変数宣言
//----------------------
const float CCamera::fTurnSpeed = 0.02f;	//旋回速度
const float CCamera::fMoveSpeed = 3.0f;		//移動速度

//===========================
// コンストラクタ
//===========================
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
	//------------------------
	// 極座標のXYZ
	//------------------------
	POLOR_X = sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	POLOR_Y = cosf(m_rot.x) * m_fDistance;
	POLOR_Z = sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	//------------------------
	// プレイヤーごとの処理
	//------------------------
	EachPlayer();

	//------------------------
	// カメラの追従
	//------------------------
	Following();

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

//========================================
// カメラの生成
// (開始位置 X, 開始位置 Y, 幅, 高さ)
//========================================
CCamera* CCamera::Create(DWORD X, DWORD Y, DWORD Width, DWORD Height)
{
	CCamera *pCamera = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pCamera = new CCamera;	//生成

	if (pCamera != nullptr)
	{//NULLチェック
		//ビューポート構成の保存
		pCamera->m_viewport.X = X;				//開始位置(X)
		pCamera->m_viewport.Y = Y;				//開始位置(Y)
		pCamera->m_viewport.Width = Width;		//幅
		pCamera->m_viewport.Height = Height;	//高さ

		//初期化
		pCamera->Init();
	}

	return pCamera;
}

//========================
// プレイヤーごとの処理
//========================
void CCamera::EachPlayer()
{
	switch (m_nNumPlayer)
	{
	//------------------
	// プレイヤー1
	//------------------
	case NUMPLAYER_ONE:
		Turn(DIK_Q, DIK_E);	//旋回
		break;

	//------------------
	// プレイヤー2
	//------------------
	case NUMPLAYER_TWO:
		Turn(DIK_R, DIK_Y);	//旋回
		break;

	//------------------
	// プレイヤー3
	//------------------
	case NUMPLAYER_THREE:
		Turn(DIK_U, DIK_O);	//旋回
		break;

	//------------------
	// プレイヤー4
	//------------------
	case NUMPLAYER_FOUR:
		Turn(DIK_LEFT, DIK_RIGHT);	//旋回
		break;

	default:
		break;
	}
}

//========================
// 旋回
// (左回転キー,右回転キー)
//========================
void CCamera::Turn(int nLeftKey,int nRightKey)
{
	//-------------------
	// 左回転
	//-------------------
	if (CInputKeyboard::Press(nLeftKey))
	{//キーが押された
		m_rot.y += fTurnSpeed;			//回転量の増加
		m_posR.x = m_posV.x + POLOR_X;	//xの距離
		m_posR.y = m_posV.y + POLOR_Y;	//yの距離
		m_posR.z = m_posV.z + POLOR_Z;	//zの距離
	}
	//-------------------
	// 右回転
	//-------------------
	else if (CInputKeyboard::Press(nRightKey))	//右回転
	{//キーが押された
		m_rot.y -= fTurnSpeed;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	/*if (CInputKeyboard::Press(DIK_UP))	//上回転
		{//上キーが押された
		if (m_rot.x <= 3.0f)
		{
		m_rot.x += fTurnSpeed;
		m_posV.x = m_posR.x - POLOR_X;
		m_posV.y = m_posR.y - POLOR_Y;
		m_posV.z = m_posR.z - POLOR_Z;
		}
		}
		else if (CInputKeyboard::Press(DIK_DOWN))	//下回転
		{//下キーが押された
		if (m_rot.x >= 1.8f)
		{
		m_rot.x -= fTurnSpeed;
		m_posV.x = m_posR.x - POLOR_X;
		m_posV.y = m_posR.y - POLOR_Y;
		m_posV.z = m_posR.z - POLOR_Z;
		}
		}*/
}

//========================
// 移動
// (上下左右の移動キー)
//========================
void CCamera::Move(int nUpKey, int nDownKey, int nLeftKey, int nRightKey)
{
	//-------------------
	// 左移動
	//-------------------
	if (CInputKeyboard::Press(nLeftKey))
	{//Aキーが押された
		if (CInputKeyboard::Press(nUpKey))
		{//Wキーが押された
			m_posV.x += sinf(m_rot.y - D3DX_PI * 0.25f) * fMoveSpeed;	//左奥移動
			m_posV.z += cosf(m_rot.y - D3DX_PI * 0.25f) * fMoveSpeed;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else if (CInputKeyboard::Press(nDownKey))
		{//Sキーが押された
			m_posV.x += sinf(m_rot.y - D3DX_PI * 0.75f) * fMoveSpeed;	//左前移動
			m_posV.z += cosf(m_rot.y - D3DX_PI * 0.75f) * fMoveSpeed;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else
		{
			m_posV.x -= sinf(m_rot.y + D3DX_PI * 0.5f) * fMoveSpeed;	//左移動
			m_posV.z -= cosf(m_rot.y + D3DX_PI * 0.5f) * fMoveSpeed;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
	}
	//-------------------
	// 右移動
	//-------------------
	else if (CInputKeyboard::Press(nRightKey))
	{//Dキーが押された
		if (CInputKeyboard::Press(nUpKey))
		{//Wキーが押された
			m_posV.x += sinf(m_rot.y + D3DX_PI * 0.25f) * fMoveSpeed;	//右奥移動
			m_posV.z += cosf(m_rot.y + D3DX_PI * 0.25f) * fMoveSpeed;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else if (CInputKeyboard::Press(nDownKey))
		{//Sキーが押された
			m_posV.x += sinf(m_rot.y + D3DX_PI * 0.75f) * fMoveSpeed;	//右前移動
			m_posV.z += cosf(m_rot.y + D3DX_PI * 0.75f) * fMoveSpeed;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else
		{
			m_posV.x += sinf(m_rot.y + D3DX_PI * 0.5f) * fMoveSpeed;	//右移動
			m_posV.z += cosf(m_rot.y + D3DX_PI * 0.5f) * fMoveSpeed;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
	}
	//-------------------
	// 奥移動
	//-------------------
	else if (CInputKeyboard::Press(nUpKey))
	{//Wキーが押された
		m_posV.x -= sinf(m_rot.y + D3DX_PI * 1.0f) * fMoveSpeed;	//奥移動
		m_posV.z -= cosf(m_rot.y + D3DX_PI * 1.0f) * fMoveSpeed;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	//-------------------
	// 手前移動
	//-------------------
	else if (CInputKeyboard::Press(nDownKey))
	{//Sキーが押された
		m_posV.x -= sinf(m_rot.y + D3DX_PI * 0.0f) * fMoveSpeed;	//前移動
		m_posV.z -= cosf(m_rot.y + D3DX_PI * 0.0f) * fMoveSpeed;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	/*if (CInputKeyboard::Press(DIK_O))	//上昇
	{//Oキーが押された
		m_posV.y -= tanf(m_rot.x + D3DX_PI * 1.0f) * fMoveSpeed;	//上移動
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	else if (CInputKeyboard::Press(DIK_L))	//下降
	{//Lキーが押された
		m_posV.y += tanf(m_rot.x + D3DX_PI * 0.0f) * fMoveSpeed;	//上移動
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}*/
}

//========================
// 追従
//========================
void CCamera::Following()
{
	if (!CApplication::GetGame())
	{//ゲームが終了してるなら
		return;
	}

	/* 1位のプレイヤー番号を取得 */
	int nFirstNumber = CGoal::GetWinner();

	if (CInputKeyboard::Trigger(DIK_Z))
	{
		nFirstNumber = 1;
	}

	if (nFirstNumber != 0)
	{//1位の値が0じゃないなら
		//1位のプレイヤーを追従する
		m_nNumPlayer = nFirstNumber;
	}

	//----------------------------
	// プレイヤーの情報を取得
	//----------------------------
	D3DXVECTOR3 playerPos(CGame::GetPlayer(m_nNumPlayer)->GetPosition());
	D3DXVECTOR3 playerRot(CGame::GetPlayer(m_nNumPlayer)->GetRot());

	//----------------------------
	// 目的の注視点を設定
	//----------------------------
	m_posRDest.x = playerPos.x + sinf(playerRot.x) * sinf(playerRot.y) * 50.0f;
	m_posRDest.y = playerPos.y + cosf(playerRot.x) * 50.0f;
	m_posRDest.z = playerPos.z + sinf(playerRot.x) * cosf(playerRot.y) * 50.0f;

	//----------------------------
	// 目的の視点を設定
	//----------------------------
	m_posVDest.x = playerPos.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posVDest.y = playerPos.y - cosf(m_rot.x) * m_fDistance;
	m_posVDest.z = playerPos.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	//----------------------------
	// 注視点を設定
	//----------------------------
	m_posR.x += (m_posRDest.x - m_posR.x) * 0.3f;
	m_posR.z += (m_posRDest.z - m_posR.z) * 0.3f;

	//----------------------------
	// 視点を設定
	//----------------------------
	m_posV.x += (m_posVDest.x - m_posV.x) * 0.3f;
	m_posV.z += (m_posVDest.z - m_posV.z) * 0.3f;
}

//========================
// 視点・注視点の設定
//========================
void CCamera::SetPos(D3DXVECTOR3 pos)
{
	m_posR = pos;	//注視点
	m_posV = m_posR + D3DXVECTOR3(0.0f, 200.0f, -400.0f);	//視点
}

//========================
// プレイヤー番号の設定
//========================
void CCamera::SetNumPlayer(int nNum)
{
	m_nNumPlayer = nNum;
}

//========================
// ビューポートの拡縮
//========================
void CCamera::AddViewSize(DWORD X, DWORD Y, int fWidth, int fHeight)
{
	//-------------------
	// 幅の加算
	//-------------------
	if (m_viewport.Width < SCREEN_WIDTH - 5)
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
	if (m_viewport.Height < SCREEN_HEIGHT - 5)
	{//幅がスクリーン内なら
		m_viewport.Height += fHeight;

		if (m_viewport.Y > 0)
		{
			m_viewport.Y += Y;
		}
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

//========================
// ビューポートの取得
//========================
D3DVIEWPORT9 CCamera::GetViewport()
{
	return m_viewport;
}