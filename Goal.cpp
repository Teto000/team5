//===================================================
//
// ゴール関係のフラグ
// Author : Arita Meigen
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "Goal.h"
#include"player.h"
#include"game.h"
#include"application.h"
#include"meshfield.h"
#include"object2D.h"
#include"fade.h"
#include"renderer.h"
#include"game.h"
#include"time.h"
#include"Ranking.h"

//静的メンバ変数宣言
 bool CGoal::m_bGoal=nullptr;
 int CGoal::m_nWinner = -1;		//優勝したやつの番号
//===========================
// コンストラクタ
//===========================
CGoal::CGoal(int nPriority) : CObject(nPriority)
{
	m_nWinner = 100;
}

//===========================
// デストラクタ
//===========================
CGoal::~CGoal()
{
}

//===========================
// 初期化
//===========================
HRESULT CGoal::Init(D3DXVECTOR3 pos)
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
	D3DXLoadMeshFromX("data\\MODEL\\X_File\\Crown_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	//頂点数の取得
	int nNumVtx = m_pMesh->GetNumVertices();	//頂点数
	DWORD sizeFVF;								//頂点フォーマットのサイズ
	BYTE*pVtxBuff;								//頂点バッファへのポインタ

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//頂点バッファのロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
	//頂点座標の代入
	//すべての頂点のposを取得する
	 m_vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);	//最大値の保存用
	 m_vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);	//最小値の保存用
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;
		//頂点座標を比較してモデルの最小値最大値を取得
		if (vtx.x > m_vtxMax.x)
		{//Xの最大値を取得
			m_vtxMax.x = vtx.x;
		}
		if (vtx.x < m_vtxMin.x)
		{//Xの最小値を取得
			m_vtxMin.x = vtx.x;
		}
		if (vtx.y > m_vtxMax.y)
		{//Yの最大値を取得
			m_vtxMax.y = vtx.y;
		}
		if (vtx.y < m_vtxMin.y)
		{//Yの最小値を取得
			m_vtxMin.y = vtx.y;
		}
		if (vtx.z > m_vtxMax.z)
		{//Zの最大値を取得
			m_vtxMax.z = vtx.z;
		}
		if (vtx.z < m_vtxMin.z)
		{//Zの最小値を取得
			m_vtxMin.z = vtx.z;
		}
		//頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += sizeFVF;
	}
	//頂点の最大値と最小値を保存
	m_vtxMax = m_vtxMax;
	m_vtxMin = m_vtxMin;

	//頂点バッファのアンロック
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//===========================
// 終了
//===========================
void CGoal::Uninit()
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
	Release();
}

//===========================
// 更新
//===========================
void CGoal::Update()
{
	/*if (m_bGoal == true)
	{
		CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
	}*/
	Collision();
}

//========================
// 描画
//========================
void CGoal::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	D3DMATERIAL9 matDef;	//現在のマテリアル保存用

	D3DXMATERIAL *pMat;		//マテリアルデータへのポインタ

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

	//---------------------------------
	// キャラクターの描画
	//---------------------------------
	//現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &pTexture);

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//プレイヤーパーツの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	//保持しているマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//===========================
// 当たり判定
//===========================
bool CGoal::Collision()
{
	//カメラの最大数を取得
	int nMaxCamera = CRenderer::GetMaxCamera();

	//----------------------------
	// カメラの最大数の設定
	//----------------------------
	if (CGame::GetEnumCamera() == CGame::NUMCAMERA_THREE)
	{//カメラ列挙型が3なら
		//カメラの最大数を1減らす
		nMaxCamera = 3;
	}

	for (int i = 0; i < nMaxCamera; i++)
	{//プレイヤーの最大人数分回す
		CPlayer*pPlayer = CGame::GetPlayer(i);

		//横からぶつかった時用
		////Y軸
		//if (pPlayer->GetPosition().y < m_pos.y + m_vtxMax.y
		//	&&pPlayer->GetPosition().y  > m_pos.y - m_vtxMin.y)
		//{
		//}
		if (pPlayer != nullptr)
		{
			//X軸
			if (pPlayer->GetPosition().z < m_pos.z + m_vtxMax.z&&pPlayer->GetPosition().z  > m_pos.z + m_vtxMin.z)
			{
				//Z軸
				if (pPlayer->GetPosition().x < m_pos.x + m_vtxMax.x
					&&pPlayer->GetPosition().x > m_pos.x + m_vtxMin.x)
				{
					if (m_bGoal==false)
					{
						CRanking::SetCurrentScore(CTime::GetTime());
					}

					m_bGoal = true;
					m_nWinner = i;
				}
			}
		}
	}
	return m_bGoal;
}

//========================
// 生成
//========================
CGoal* CGoal::Create( D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{//モデルのパス , 親モデルから見た位置 , 親モデルから見た向き

	//----------------------------------
	// 生成と初期化
	//----------------------------------
	CGoal *pGoal = nullptr;
	//pGoal = new CGoal(CObject::OBJTYPE_GOAL);	//生成
	pGoal = new CGoal(0);	//生成


	if (pGoal != nullptr)
	{//NULLチェック
	 //メンバ変数に代入
		pGoal->m_pos = pos;
		pGoal->m_rot = rot;

		//初期化
		pGoal->Init(pos);
	}

	return pGoal;
}

//========================
// 位置の設定
//========================
void CGoal::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}


//===========================
// ゴールフラグの取得
//===========================
bool CGoal::GetGoalFrag()
{
	return m_bGoal;
}
//===========================
// 位置の取得
//===========================
D3DXVECTOR3 CGoal::GetPosition()
{
	return m_pos;
}

//===========================
// 幅の取得
//===========================
float CGoal::GetWidth()
{
	return 0.0f;
}

//===========================
// 高さの取得
//===========================
float CGoal::GetHeight()
{
	return 0.0f;
}

//===========================
// 優勝者の番号の取得
//===========================
int CGoal::GetWinner()
{
	return m_nWinner;
}
