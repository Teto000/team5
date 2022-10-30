//===================================================
//
// ゴール関係のフラグ
// Author : Arita Meigen
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "GoalFragManager.h"
#include"player.h"
#include"game.h"
#include"application.h"
#include"meshfield.h"
#include"object2D.h"
#include"fade.h"
#include"renderer.h"

//===========================
// コンストラクタ
//===========================
CGoalFragManager::CGoalFragManager()
{
}

//===========================
// デストラクタ
//===========================
CGoalFragManager::~CGoalFragManager()
{
}

//===========================
// 初期化
//===========================
HRESULT CGoalFragManager::Init()
{
	//フラグリセット
	m_bGoal = false;

	//-----------------------
	// デバイスの取得
	//-----------------------
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//-----------------------
	// Xファイルの読み込み
	//-----------------------
	D3DXLoadMeshFromX("data\\MODEL\\X_File\\legL.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	return S_OK;
}

//===========================
// 終了
//===========================
void CGoalFragManager::Uninit()
{
	//-----------------------
	// メッシュの破棄
	//-----------------------
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	//-----------------------
	// マテリアルの破壊
	//-----------------------
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}
}

//===========================
// 更新
//===========================
void CGoalFragManager::Update()
{
	if (m_bGoal == true)
	{
		CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
	}
}

//========================
// 描画
//========================
void CGoalFragManager::Draw()
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
}

//===========================
// 当たり判定
//===========================
bool CGoalFragManager::Collision()
{
	for (int i = 0; i < 2; i++)
	{
		CPlayer*pPlayer = CGame::GetPlayer(i);

		//横からぶつかった時用
		//Y軸
		if (pPlayer->GetPosition().y < m_pos.y + m_vtxMax.y
			&&pPlayer->GetPosition().y  > m_pos.y - m_vtxMin.y)
		{
			//X軸
			if (pPlayer->GetPosition().z < m_pos.z + m_vtxMax.z
				&&pPlayer->GetPosition().z  > m_pos.z + m_vtxMin.z)
			{
				m_bGoal = true;
			}
			//Z軸
			else if (pPlayer->GetPosition().x < m_pos.x + m_vtxMax.x
				&&pPlayer->GetPosition().x > m_pos.x + m_vtxMin.x)
			{
				m_bGoal = true;
			}
		}
	}
	return m_bGoal;
}

//========================
// 生成
//========================
CGoalFragManager* CGoalFragManager::Create( D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{//モデルのパス , 親モデルから見た位置 , 親モデルから見た向き

	//----------------------------------
	// 生成と初期化
	//----------------------------------
	CGoalFragManager *pGoal = nullptr;
	pGoal = new CGoalFragManager;	//生成

	if (pGoal != nullptr)
	{//NULLチェック
	 //メンバ変数に代入
		pGoal->m_pos = pos;
		pGoal->m_rot = rot;

		//初期化
		pGoal->Init();
	}

	return pGoal;
}


//===========================
// ゴールフラグの取得
//===========================
bool CGoalFragManager::GetGoalFrag()
{
	return m_bGoal;
}