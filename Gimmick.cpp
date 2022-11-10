//===================================================
//
// プレイヤーの処理
// Author:Teruto Sato
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "Gimmick.h"
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

//------------------------
// 静的メンバ変数宣言
//------------------------
const float CGimmick::fPlayerSpeed = 1.0f;
const float CGimmick::fGravity = 0.1f;

//========================
// コンストラクタ
//========================
CGimmick::CGimmick() : CObject(OBJTYPE_GIMMICK)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//目的の向き
}

//========================
// デストラクタ
//========================
CGimmick::~CGimmick()
{

}

//========================
// 初期化
//========================
HRESULT CGimmick::Init(D3DXVECTOR3 pos)
{
	Load();
	m_pos = pos;

	CRead cRead;
	m_nMotionNum = cRead.ReadMotion(m_nGimmickName[GetType()]);

	return S_OK;
}

//========================
// 終了
//========================
void CGimmick::Uninit()
{
	Release();
}

//========================
// 更新
//========================
void CGimmick::Update()
{
	//---------------
	// 移動
	//---------------
	m_posold = m_pos;	//位置の保存
	Move();

	D3DXVECTOR3 pos = CMotionParts::AllCollision(m_nMotionNum, CGame::GetGroundNum(), m_pos, m_posold);

	//モーション再生
	CMotionParts::MoveMotionModel(m_pos, GetRot(), m_nMotionNum, 0);

	m_rot.y = atan2f(m_move.x, m_move.z) + D3DX_PI;
}

//========================
// 描画
//========================
void CGimmick::Draw()
{

}

void CGimmick::Load()
{
	//ギミックのオブジェクト
	m_nGimmickName[0] = "data/MOTION/Gimmick001Hummer.txt";
	m_nGimmickName[1] = "data/MOTION/Gimmick002RotateStick.txt";
	m_nGimmickName[2] = "data/MOTION/Gimmick003Gate.txt";
	m_nGimmickName[3] = "data/MOTION/Gimmick004ElasticStick.txt";
}

//========================
// 生成
//========================
CGimmick* CGimmick::Create(int Type, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CGimmick *pGimmick = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pGimmick = new CGimmick();	//生成

	if (pGimmick != nullptr)
	{//NULLチェック
	 //初期化
		pGimmick->SetType(Type) ;
		pGimmick->Init(D3DXVECTOR3(pos));
		pGimmick->m_rot = rot;
	}

	return pGimmick;
}

//========================
// 移動
//========================
void CGimmick::Move()
{
	//位置を更新
	m_pos += m_move;

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
}


//===========================
// 位置の設定
//===========================
void CGimmick::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===========================
// 位置の取得
//===========================
D3DXVECTOR3 CGimmick::GetPosition()
{
	return m_pos;
}

//========================
// 向きの取得
//========================
D3DXVECTOR3 CGimmick::GetRot()
{
	return m_rot;
}

//===========================
// 幅の取得
//===========================
float CGimmick::GetWidth()
{
	return 0.0f;
}

//===========================
// 高さの取得
//===========================
float CGimmick::GetHeight()
{
	return 0.0f;
}

//===========================
// 1F前の座標の取得
//===========================
D3DXVECTOR3 CGimmick::GetPosOld()
{
	return D3DXVECTOR3();
}
