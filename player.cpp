//===================================================
//
// プレイヤーの処理
// Author:Teruto Sato
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "player.h"
#include "object.h"
#include "application.h"
#include "renderer.h"
#include "input.h"
#include "input_keybord.h"
#include "camera.h"
#include "debug_proc.h"
#include "model.h"
#include "meshfield.h"
#include "game.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
const float CPlayer::fPlayerSpeed = 1.0f;
const float CPlayer::fGravity = 0.6f;
CShadow* CPlayer::m_pShadow = nullptr;
CBullet* CPlayer::m_pBullet = nullptr;
CModel*	 CPlayer::m_pModel[MAX_PARTS] = {};

//------------------------
// グローバル変数
//------------------------
CPlayer::KEY_SET g_aKeySet[] =	//キーセット情報
{
	//----------------------
	// キー1
	//----------------------
	{ 40,	//フレーム数
		//		Pos				Rot
		{{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//体
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//頭
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//右手
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//左手
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//右足
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//左手
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f }},	//剣
	},

	//----------------------
	// キー2
	//----------------------
	{ 40,	//フレーム数
		//		Pos				Rot
		{{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//体
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//頭
		{ 0.0f,0.0f,0.0f , 90.0f,0.0f,0.0f },		//右手
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//左手
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//右足
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//左手
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f }},	//剣
	},
};

//========================
// コンストラクタ
//========================
CPlayer::CPlayer() : CObject(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//目的の向き

	/* ↓ モーション情報 ↓ */
	m_nCurrentKey = 0;
	m_nCntMotion = 0;
}

//========================
// デストラクタ
//========================
CPlayer::~CPlayer()
{
	// プレイヤーの待機状態
	m_state = IDOL_STATE;
}

//========================
// 初期化
//========================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	m_pos = pos;
	m_nCntMotion = 1;
	m_bJump = false;
	//--------------------
	// モデルの生成
	//--------------------
	SetModel();

	return S_OK;
}

//========================
// 終了
//========================
void CPlayer::Uninit()
{
	//-------------------------
	// モデルの終了
	//-------------------------
	for (int i = 0; i < MAX_PARTS; i++)
	{//モデルパーツ数分回す
		if (m_pModel[i])
		{//モデルがnullじゃないなら
			m_pModel[i]->Uninit();
		}
	}

	CObject::Release();
}

//========================
// 更新
//========================
void CPlayer::Update()
{
	//-------------------------
	// モデルの更新
	//-------------------------
	for (int i = 0; i < MAX_PARTS; i++)
	{//モデルパーツ数分回す
		if (m_pModel[i])
		{//モデルがnullじゃないなら
			m_pModel[i]->Update();
		}
	}
	//---------------
	// 移動
	//---------------
	Move();

	//-------------------
	// モーション
	//-------------------
	SetMotion(true);

	//-------------------
	//当たり判定
	//-------------------
	CMeshField *m_pMesh = CGame::GetMesh();
	m_pMesh->Collision(&m_pos);
}

//========================
// 描画
//========================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//モデルのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//モデルの向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//モデルの位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-------------------------
	// モデルの描画
	//-------------------------
	for (int i = 0; i < MAX_PARTS; i++)
	{//モデルパーツ数分回す
		if (!m_pModel[i])
		{//モデルがnullなら
			return;
		}

		m_pModel[i]->Draw();
	}
}

//========================
// 生成
//========================
CPlayer* CPlayer::Create(int PlayerNum)
{
	CPlayer *pPlayer = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pPlayer = new CPlayer;	//生成

	if (pPlayer != nullptr)
	{//NULLチェック
		//初期化
		pPlayer->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pPlayer->m_nPlayerNum = PlayerNum;
		//pPlayer->SetObjType(OBJTYPE_ENEMY);
	}

	return pPlayer;
}

//========================
// モデルの生成
//========================
void CPlayer::SetModel()
{
	//モデル0
	m_pModel[0] = CModel::Create("data\\MODEL\\X_File\\body.x", D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pModel[0]->SetParent(nullptr);

	//モデル1
	m_pModel[1] = CModel::Create("data\\MODEL\\X_File\\head.x", D3DXVECTOR3(0.0f, 40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pModel[1]->SetParent(m_pModel[0]);

	//モデル2
	m_pModel[2] = CModel::Create("data\\MODEL\\X_File\\armR.x", D3DXVECTOR3(-20.0f, 40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pModel[2]->SetParent(m_pModel[0]);

	//モデル3
	m_pModel[3] = CModel::Create("data\\MODEL\\X_File\\armL.x", D3DXVECTOR3(20.0f, 40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pModel[3]->SetParent(m_pModel[0]);

	//モデル4
	m_pModel[4] = CModel::Create("data\\MODEL\\X_File\\legR.x", D3DXVECTOR3(-7.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pModel[4]->SetParent(m_pModel[0]);

	//モデル5
	m_pModel[5] = CModel::Create("data\\MODEL\\X_File\\legL.x", D3DXVECTOR3(7.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pModel[5]->SetParent(m_pModel[0]);

	//モデル6
	m_pModel[6] = CModel::Create("data\\MODEL\\X_File\\sword.x", D3DXVECTOR3(-22.0f, -14.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pModel[6]->SetParent(m_pModel[2]);
}

//========================
// モーションの設定
//========================
void CPlayer::SetMotion(bool bLoop)
{
	if (m_nCurrentKey + 1 >= MAX_KEY)
	{//キーが最大数に達したら
		if (bLoop)
		{
			m_nCurrentKey = 0;	//キー番号の初期化
		}
		else
		{
			return;
		}
	}

	for (int i = 0; i < MAX_PARTS; i++)
	{//モデルパーツ数分回す
		//-----------------------------------------
		// NULLチェック
		//-----------------------------------------
		if (!m_pModel[i])
		{//モデルパーツがnullなら
			return;
		}

		//キー情報を持った変数
		KEY key = g_aKeySet[m_nCurrentKey].aKey[i];
		KEY keyNext = g_aKeySet[m_nCurrentKey + 1].aKey[i];

		//-----------------------------------------
		// 現在値を取得
		//-----------------------------------------
		//位置を取得
		float fPosX = m_pModel[i]->GetPos().x;
		float fPosY = m_pModel[i]->GetPos().y;
		float fPosZ = m_pModel[i]->GetPos().z;

		//向きを取得
		float fRotX = m_pModel[i]->GetRot().x;
		float fRotY = m_pModel[i]->GetRot().y;
		float fRotZ = m_pModel[i]->GetRot().z;

		//-----------------------------------------
		// 差分の計算
		// (終了値 - 開始値)
		//-----------------------------------------
		//位置
		float fDifPosX = keyNext.fPosX - key.fPosX;
		float fDifPosY = keyNext.fPosY - key.fPosY;
		float fDifPosZ = keyNext.fPosZ - key.fPosZ;

		//向き
		float fDifRotX = keyNext.fRotX - key.fRotX;
		float fDifRotY = keyNext.fRotY - key.fRotY;
		float fDifRotZ = keyNext.fRotZ - key.fRotZ;

		//-----------------------------------------
		// 相対値の計算
		// (モーションカウンター / フレーム数)
		//-----------------------------------------
		float fNumRelative = m_nCntMotion / (float)g_aKeySet[m_nCurrentKey].nFrame;

		//-----------------------------------------
		// 現在値の計算
		// (開始値 + (差分 * 相対値))
		//-----------------------------------------
		//位置
		fPosX += key.fPosX + (fDifPosX * fNumRelative);
		fPosY += key.fPosY + (fDifPosY * fNumRelative);
		fPosZ += key.fPosZ + (fDifPosZ * fNumRelative);

		//向き
		fRotX = key.fRotX + (fDifRotX * fNumRelative);
		fRotY = key.fRotY + (fDifRotY * fNumRelative);
		fRotZ = key.fRotZ + (fDifRotZ * fNumRelative);

		//-----------------------------------------
		// モデル情報の設定
		//-----------------------------------------
		//位置の設定
		m_pModel[i]->SetPos(D3DXVECTOR3(fPosX, fPosY, fPosZ));

		//向きの設定
		m_pModel[i]->SetRot(D3DXVECTOR3(D3DXToRadian(fRotX), D3DXToRadian(fRotY), D3DXToRadian(fRotZ)));
	}

	//モーションカウンターを進める
	m_nCntMotion++;

	//-------------------------
	// 初期化
	//-------------------------
	if (m_nCntMotion >= g_aKeySet[m_nCurrentKey].nFrame)
	{//モーションカウンターが再生フレームに達したら
		m_nCurrentKey++;	//キー番号を加算
		m_nCntMotion = 0;	//モーションカウンターを初期化
	}
}

//========================
// 移動
//========================
void CPlayer::Move()
{
	switch (m_nPlayerNum)
	{
	case 0:
		//プレイヤー1の操作
		MoveKey(DIK_W, DIK_A, DIK_S, DIK_D,DIK_SPACE);
		break;
	case 1:
		//プレイヤー2の操作
		MoveKey(DIK_T, DIK_F, DIK_G, DIK_H, DIK_BACKSPACE);
		break;
	}

	//位置を更新
	m_pos += m_move;

	//慣性の実装
	m_move.x += (0.0f - m_move.x) * 0.1f;
	m_move.z += (0.0f - m_move.z) * 0.1f;

	//重力関連
	if (m_pos.y <= 1.0f)
	{
		m_bJump = false;
		m_move.y = 0.0f;
		
		//仮で地面で止まる処理
		m_pos.y = 1.0f;
	}
		//重力
		m_move.y -= fGravity;

	//-------------------------------
	// ジャンプ状態の処理
	//-------------------------------
	if (m_state == JUMP_STATE)
	{//ジャンプ状態の時のみ
		m_bJump = true;
		Jump();
	}

	//-------------------------------
	// 位置の設定
	//-------------------------------
	/*if (m_pModel[0])
	{
		m_pModel[0]->SetPos(m_pos);
	}*/

	//-------------------------------
	// 目的の角度の正規化
	//-------------------------------
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{//回転したい角度が180以上なら
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{//回転したい角度が-180以下なら
		m_rotDest.y += D3DX_PI * 2;
	}

	//-------------------------------
	// 目的の角度まで回転する
	//-------------------------------
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.04f;	//減衰処理

	//-------------------------------
	// 角度の正規化
	//-------------------------------
	if (m_rot.y > D3DX_PI)
	{//角度が180以上なら
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{//角度が-180以下なら
		m_rot.y += D3DX_PI * 2;
	}

	//-------------------------------
	// 向きの設定
	//-------------------------------
	/*if (m_pModel[0])
	{
		m_pModel[0]->SetRot(m_rot);
	}*/
}

//===========================
// ジャンプの処理
//===========================
void CPlayer::Jump()
{
	m_move.y = 5.0f;
	m_nJumpCount++;

	if (m_nJumpCount == 10)
	{
		//ジャンプカウントをリセットする
		m_nJumpCount = 0;
		//プレイヤーを待機状態にする
		m_state = IDOL_STATE;
	}
}

//===========================
// 位置の設定
//===========================
void CPlayer::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===========================
// 位置の取得
//===========================
D3DXVECTOR3 CPlayer::GetPosition()
{
	return m_pos;
}

//========================
// 向きの取得
//========================
D3DXVECTOR3 CPlayer::GetRot()
{
	return m_rot;
}

//===========================
// 幅の取得
//===========================
float CPlayer::GetWidth()
{
	return 0.0f;
}

//===========================
// 高さの取得
//===========================
float CPlayer::GetHeight()
{
	return 0.0f;
}

//===========================
// ワールドマトリックスの取得
//===========================
D3DXMATRIX CPlayer::GetmtxWorld()
{
	return m_mtxWorld;
}

//=====================================================
// 移動キーの設定
// 引数 : 上キー、左キー、下キー、右キー、ジャンプキー
//=====================================================
void CPlayer::MoveKey(int UPKey,int LEFTKey,int DOWNKey,int RIGHTKey,int JUMPKey)
{
	// カメラの角度情報取得
	D3DXVECTOR3 CameraRot = CApplication::GetCamera(0)->GetRot();;

	//-------------------------------
	// プレイヤーの操作
	//-------------------------------
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);            // 移動量の初期化
	float moveLength = 0.0f;
	D3DXVECTOR2 moveInput;

	moveInput.x = 0.0f;
	moveInput.y = 0.0f;

	// モデルの移動
	if (CInputKeyboard::Press(UPKey))
	{
		moveInput.y += 1.0f;
		moveLength = 1.0f;
	}
	if (CInputKeyboard::Press(LEFTKey))
	{
		moveInput.x -= 1.0f;
		moveLength = 1.0f;
	}
	if (CInputKeyboard::Press(DOWNKey))
	{
		moveInput.y -= 1.0f;
		moveLength = 1.0f;
	}
	if (CInputKeyboard::Press(RIGHTKey))
	{
		moveInput.x += 1.0f;
		moveLength = 1.0f;
	}

	//ジャンプ状態ではないときに
	if (m_state == IDOL_STATE)
	{
		if (CInputKeyboard::Trigger(JUMPKey) && !m_bJump)
		{
			// ジャンプ状態に移行
			m_state = JUMP_STATE;
		}
	}

	if (moveLength > 0.0f)
	{

		D3DXVec2Normalize(&moveInput, &moveInput);

		float c = cosf(-CameraRot.y);
		float s = sinf(-CameraRot.y);

		// move の長さは 1 になる。
		m_move.x = moveInput.x * c - moveInput.y * s;
		m_move.z = moveInput.x * s + moveInput.y * c;

		m_move.x *= 3.0f;
		m_move.z *= 3.0f;
	}
	else
	{ // 入力されていない。
		return;
	}
}