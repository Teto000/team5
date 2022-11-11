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
#include "camera_player.h"
#include "debug_proc.h"
#include "model.h"
#include "meshfield.h"
#include "game.h"
#include "motion_parts.h"
#include "read.h"
#include "num_block.h"
#include "num_rank.h"
#include "block.h"
#include "sound.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
const float CPlayer::fPlayerSpeed = 15.0f;
const float CPlayer::fGravity = 0.1f;

//========================
// コンストラクタ
//========================
CPlayer::CPlayer() : CObject(OBJTYPE_MODEL)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置
	m_respornPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//リスポーン地点
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向き
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//目的の向き
	m_nNumBlock = 0;								//ブロック数
	m_pNumBlock = nullptr;							//ブロック数の表示
	m_pRank = nullptr;								//順位
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
	m_bJump = false;

	//-----------------------------
	// ブロック数の表示
	//-----------------------------
	{
		//ビューポートの座標を取得、設置場所の計算
		float X = CGame::GetCameraPlayer(m_nPlayerNum)->GetViewport().X + 50.0f;
		float Y = CGame::GetCameraPlayer(m_nPlayerNum)->GetViewport().Y + 
				(CGame::GetCameraPlayer(m_nPlayerNum)->GetViewport().Height - 50.0f);
		D3DXVECTOR3 Pos(X, Y, 0.0f);
		m_pNumBlock = CNumBlock::Create(Pos);
	}

	//-----------------------------
	// 順位の生成
	//-----------------------------
	{
		//ビューポートの座標を取得、設置場所の計算
		float X = CGame::GetCameraPlayer(m_nPlayerNum)->GetViewport().X +
			(CGame::GetCameraPlayer(m_nPlayerNum)->GetViewport().Width - 80.0f);

		float Y = CGame::GetCameraPlayer(m_nPlayerNum)->GetViewport().Y +
			(CGame::GetCameraPlayer(m_nPlayerNum)->GetViewport().Height - 50.0f);
		D3DXVECTOR3 Pos(X, Y, 0.0f);
		m_pRank = CRank::Create(Pos, m_nPlayerNum);
	}
	
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		m_pModel[nCnt] = nullptr;
	}

	CRead cRead;

	m_nMotionNum = cRead.ReadMotion("data/MOTION/motionplayer.txt");


	return S_OK;
}

//========================
// 終了
//========================
void CPlayer::Uninit()
{
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			m_pModel[nCnt]->Uninit();
			m_pModel[nCnt]->SetAboveFalse();
			delete m_pModel[nCnt];
			m_pModel[nCnt] = nullptr;
		}
	}

	Release();
}

//========================
// 更新
//========================
void CPlayer::Update()
{
	//---------------
	// 移動
	//---------------
	m_posold = m_pos;	//位置の保存
	Move();

#ifdef _DEBUG
	//デバック用
	if (CInputKeyboard::Press(DIK_K))
	{
		m_BlockHave++;
	}

	if (CInputKeyboard::Trigger(DIK_Z))
	{
		m_pModel[0] = CBlock::Create(D3DXVECTOR3(m_pos.x+ 130.0f, -1.0f, m_pos.z), m_rot);
	}

	//----------------------
	// ブロック数の増減
	//----------------------
	if (CInputKeyboard::Press(DIK_UP) && m_nPlayerNum == 0)
	{
		m_nNumBlock = m_pNumBlock->AddNumber(1);
	}
	else if (CInputKeyboard::Press(DIK_DOWN) && m_nPlayerNum == 0)
	{
		m_nNumBlock = m_pNumBlock->AddNumber(-1);
	}
#endif // _DEBUG

	//----------------------
	// 所持ブロック数の加算
	//----------------------
	//for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	//{
	//	if (m_pModel[nCnt] != nullptr)
	//	{
	//		if (m_pModel[0]->GetHaveBlock())
	//		{
	//			if (m_pModel[0]->GetType() == CBlock::FIELD_BLOCK)
	//			{
	//				m_nNumBlock = m_pNumBlock->AddNumber(1);
	//				m_BlockCnt++;
	//				
	//				// モデルの削除
	//				m_pModel[0]->Uninit();
	//				delete m_pModel[nCnt];
	//				m_pModel[0] = nullptr;
	//			}
	//		}
	//	}
	//}

	//-------------------
	//当たり判定
	//-------------------
	/*CMeshField *m_pMesh = CGame::GetMesh();
	m_pMesh->Collision(&m_pos);*/

	D3DXVECTOR3 pos = CMotionParts::AllCollision(m_nMotionNum,CGame::GetGroundNum(), m_pos, m_posold);
	
	if (m_pos != pos)
	{
		m_pos = pos;
		m_move.y = 0.0f;
		m_respornPos = m_pos;
	}
	else if (m_pos == pos && pos.y < -8.0f)
	{//コースアウト
		SetBlock();
		CSound::PlaySound(CSound::SOUND_LABEL_SE_FALL1);

		if (m_pos.y <= -150.0f)
		{
			m_pos = m_respornPos;
		}
	}
	

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			m_pModel[nCnt]->Update();
		}
	}

	pos = m_pos;
	//pos.y += 50.0f;

	m_pos = CMotionParts::AllCollisionObstacle(m_nMotionNum, pos, m_posold);

	CMotionParts::MoveMotionModel(m_pos, GetRot(), m_nMotionNum, 1);

	m_rot.y = atan2f(m_move.x, m_move.z) + D3DX_PI;
}

//========================
// 描画
//========================
void CPlayer::Draw()
{
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			m_pModel[nCnt]->Draw();
		}
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
		pPlayer->m_nPlayerNum = PlayerNum;
		pPlayer->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//pPlayer->SetObjType(OBJTYPE_ENEMY);
	}

	return pPlayer;
}

//========================
// 移動
//========================
void CPlayer::Move()
{
	// プレイヤーの番号ごとに操作を変える処理
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

	case 2:
		//プレイヤー3の操作
		MoveKey(DIK_I, DIK_J, DIK_K, DIK_L, DIK_P);
		break;

	default:
		break;
	}

	//位置を更新
	m_pos += m_move;

	//慣性の実装
	m_move.x += (0.0f - m_move.x) * 0.1f;
	m_move.z += (0.0f - m_move.z) * 0.1f;

	////重力関連
	//if (m_pos.y <= 1.0f)
	//{
	//	m_bJump = false;
	//	m_move.y = 0.0f;
	//	
	//	////仮で地面で止まる処理
	//	//m_pos.y = 1.0f;
	//}
	
	Gravity();

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
	CSound::PlaySound(CSound::SOUND_LABEL_SE_JUMP1);
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
// 1F前の座標の取得
//===========================
D3DXVECTOR3 CPlayer::GetPosOld()
{
	return D3DXVECTOR3();
}

//=====================================================
// 移動キーの設定
// 引数 : 上キー、左キー、下キー、右キー、ジャンプキー
//=====================================================
void CPlayer::MoveKey(int UPKey,int LEFTKey,int DOWNKey,int RIGHTKey,int JUMPKey)
{
	// カメラの角度情報取得
	D3DXVECTOR3 CameraRot = CGame::GetCameraPlayer(0)->GetRot();;

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

	/*moveInput.y += 1.0f;
	moveLength = 1.0f;*/

	////ジャンプ状態ではないときに
	//if (m_state == IDOL_STATE)
	//{
	//	if (CInputKeyboard::Trigger(JUMPKey) && !m_bJump)
	//	{
	//		// ジャンプ状態に移行
	//		m_state = JUMP_STATE;
	//	}
	//}

	if (moveLength > 0.0f)
	{

		D3DXVec2Normalize(&moveInput, &moveInput);

		float c = cosf(-CameraRot.y);
		float s = sinf(-CameraRot.y);

		// move の長さは 1 になる。
		m_move.x = moveInput.x * c - moveInput.y * s;
		m_move.z = moveInput.x * s + moveInput.y * c;

		m_move.x *= fPlayerSpeed;
		m_move.z *= fPlayerSpeed;
	}
	else
	{ // 入力されていない。
		return;
	}
}

//=========================================
// ブロックを設置する
//=========================================
void CPlayer::SetBlock()
{
	// プレイヤーのy座標が0以下の時
	if (m_pos.y > 0.0f)
	{
		return;
	}

	// ブロックの所持数が1以上の時
	if (m_nNumBlock <= 0)
	{
		return;
	}

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (m_pModel[nCnt]->GetBlockCollision())
		{// trueだった時
			return;
		}
	}

	if (m_pModel[m_BlockCnt] == nullptr)
	{// モデルの設定
		m_pModel[m_BlockCnt] = CBlock::Create(D3DXVECTOR3(m_pos.x, -1.0f, m_pos.z), m_rot);
		m_pModel[m_BlockCnt]->SetAbove();
		m_BlockCnt++;
		m_nNumBlock = m_pNumBlock->AddNumber(-1);
		CSound::PlaySound(CSound::SOUND_LABEL_SE_BLOCK);
	}
}

//=========================================
// 重力の処理
//=========================================
void CPlayer::Gravity()
{
	//重力
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (m_pModel[nCnt]->GetBlockCollision())
		{// trueだった時
			m_pos.y = 0.0f;
			m_move.y = 0.0f;
			SetPosition(m_pos);
			return;
		}
	}

	m_move.y -= fGravity;
}

//=========================================
// 所持数の加算処理
//=========================================
void CPlayer::Add(int Add)
{
	// 所持数加算
	m_nNumBlock = m_pNumBlock->AddNumber(Add);
}