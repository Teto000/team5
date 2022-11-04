//==================================================
//
// プレイヤーカメラの処理
// Author:Teruto Sato
//
//==================================================

//----------------------
// インクルード
//----------------------
#include "camera_player.h"
#include "camera.h"
#include "input.h"
#include "input_keybord.h"
#include "application.h"
#include "game.h"
#include "player.h"

//==================================================
// コンストラクタ
//==================================================
CCameraPlayer::CCameraPlayer()
{
	m_nNumPlayer = -1;		//対応するプレイヤー番号
	m_nNumFieldCamera = 0;	//フィールドカメラのプレイヤー番号
	m_nChangeTime = 0;		//フィールドカメラ切り替え時間
}

//==================================================
// デストラクタ
//==================================================
CCameraPlayer::~CCameraPlayer()
{

}

//==================================================
// カメラの初期化処理
//==================================================
HRESULT CCameraPlayer::Init(void)
{
	//カメラの初期化
	CCamera::Init();

	return S_OK;
}

//==================================================
// カメラの終了処理
//==================================================
void CCameraPlayer::Uninit(void)
{
	//カメラの終了
	CCamera::Uninit();
}

//==================================================
// カメラの更新処理
//==================================================
void CCameraPlayer::Update(void)
{
	//カメラの更新
	CCamera::Update();

	//------------------------
	// プレイヤーごとの処理
	//------------------------
	EachPlayer();

	//------------------------
	// フィールドカメラの旋回
	//------------------------
	if (CApplication::GetGame()->GetEnumCamera() == CGame::NUMCAMERA_THREE
		&& m_nNumPlayer == -1)
	{//カメラ列挙型がthree かつ 対応するプレイヤー番号が0未満なら
		m_rot.y -= fTurnSpeed / 16;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}

	//------------------------
	// カメラの追従
	//------------------------
	Following();
}

//==================================================
// カメラの設定処理
// 引数 : デバイス
//==================================================
void CCameraPlayer::SetCamera(LPDIRECT3DDEVICE9 pDevice)
{
	//カメラの設定
	CCamera::SetCamera(pDevice);
}

//==================================================
// カメラの生成
// 引数 : 左上の座標 X, 左上の座標 Y, 幅, 高さ
//==================================================
CCameraPlayer* CCameraPlayer::Create(DWORD X, DWORD Y, DWORD Width, DWORD Height)
{
	CCameraPlayer *pCamera = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pCamera = new CCameraPlayer;	//生成

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

//==================================================
// プレイヤーごとの処理
//==================================================
void CCameraPlayer::EachPlayer()
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

//==================================================
// 旋回
// 引数 : 左回転キー,右回転キー
//==================================================
void CCameraPlayer::Turn(int nLeftKey, int nRightKey)
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

//==================================================
// 移動
// 引数 : 上キー、下キー、左キー、右キー
//==================================================
void CCameraPlayer::Move(int nUpKey, int nDownKey, int nLeftKey, int nRightKey)
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

//==================================================
// 追従
//==================================================
void CCameraPlayer::Following()
{
	if (!CApplication::GetGame())
	{//ゲームが終了してるなら
		return;
	}

	D3DXVECTOR3 playerPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	D3DXVECTOR3 playerRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//------------------------
	// フィールドカメラの設定
	//------------------------
	if (CApplication::GetGame()->GetEnumCamera() == CGame::NUMCAMERA_THREE
		&& m_nNumPlayer == -1)
	{//カメラ列挙型がthree かつ 対応するプレイヤー番号が0未満なら

	 //切り替え時間の加算
		m_nChangeTime++;

		if (m_nChangeTime % 360 == 0)
		{//一定時間が経過したら
		 //ランダムなプレイヤーに切り替え
		 //m_nNumFieldCamera = rand() % 3;

		 //次のプレイヤーに切り替え
			m_nNumFieldCamera++;

			if (m_nNumFieldCamera >= 3)
			{//プレイヤー人数が最大なら
			 //最初のプレイヤーを追従
				m_nNumFieldCamera = 0;
			}

			//カウントのリセット
			m_nChangeTime = 0;
		}

		//プレイヤーの情報を取得
		playerPos = CGame::GetPlayer(m_nNumFieldCamera)->GetPosition();
		playerRot = CGame::GetPlayer(m_nNumFieldCamera)->GetRot();
	}
	else
	{//それ以外なら
	 //----------------------------
	 // プレイヤーの情報を取得
	 //----------------------------
		playerPos = CGame::GetPlayer(m_nNumPlayer)->GetPosition();
		playerRot = CGame::GetPlayer(m_nNumPlayer)->GetRot();
	}

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

//==================================================
// プレイヤー番号の設定
//==================================================
void CCameraPlayer::SetNumPlayer(int nNum)
{
	m_nNumPlayer = nNum;
}