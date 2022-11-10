//===================================================
//
// 順位の処理
// Author : Sato Teruto
//
//===================================================

//----------------------
// インクルード
//----------------------
#include <stdlib.h>
#include <math.h>
#include "num_rank.h"
#include "number.h"
#include "Editor.h"
#include "player.h"
#include "Goal.h"
#include "game.h"

//----------------------
// 静的メンバ変数宣言
//----------------------
int CRank::m_nRank[MAX_PLAYER] = {};		//順位
int CRank::m_nDistance[MAX_PLAYER] = {};	//距離

//=======================
// コンストラクタ
//=======================
CRank::CRank() : CObject2D(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	m_aPosTexU = 0;		//今の桁の数値
	m_nNumPlayer = 0;	//対応するプレイヤー番号
}

//=======================
// デストラクタ
//=======================
CRank::~CRank()
{

}

//=======================
// 初期化
//=======================
HRESULT CRank::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_pos = pos;	//位置

	//順位の初期設定
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		m_nRank[i] = (i + 1);
	}

	CObject2D::Init(m_pos);
	CObject2D::SetSize(100.0f, 100.0f);	//サイズの設定

	//テクスチャの設定
	SetTexture(m_nNumPlayer);

	return S_OK;
}

//=======================
// 終了
//=======================
void CRank::Uninit()
{
	CObject2D::Uninit();
}

//=======================
// 更新
//=======================
void CRank::Update()
{
	CObject2D::Update();

	//ゴールとプレイヤーの位置を取得
	//ゴールの位置を取得
	D3DXVECTOR3 goalPos(CEditor::GetGoal()->GetPosition());

	//プレイヤーの位置を取得
	D3DXVECTOR3 playrerPos[MAX_PLAYER];
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (CGame::GetPlayer(i) != nullptr)
		{
			playrerPos[i] = CGame::GetPlayer(i)->GetPosition();
		}
	}

	//ベクトルを計算、配列に入れる
	D3DXVECTOR3 vec[MAX_PLAYER];
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		vec[i] = D3DXVECTOR3((goalPos.x - playrerPos[i].x),0.0f,
								(goalPos.z - playrerPos[i].z));
	}

	//ゴールまでの距離を求める
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		//XとZの絶対値を取得
		int X = fabsf(vec[i].x);
		int Z = fabsf(vec[i].z);

		//加算
		m_nDistance[i] = X + Z;
	}

	//距離が小さい順に順位を設定
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		for (int j = i + 1; j < MAX_PLAYER; j++)
		{
			if (m_nDistance[i] > m_nDistance[j])
			{
				//順位の入れ替え
				int nData = m_nRank[i];
				m_nRank[i] = m_nRank[j];
				m_nRank[j] = nData;
			}
		}
	}

	//テクスチャの設定
	SetTexture(m_nNumPlayer);

	if (CGame::GetFinish())
	{//終了フラグが立っているなら
		Uninit();
	}
}

//=======================
// 描画
//=======================
void CRank::Draw()
{
	CObject2D::Draw();
}

//=======================
// 生成
//=======================
CRank *CRank::Create(D3DXVECTOR3 pos, int nNumPlayer)
{
	CRank *pRank = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pRank = new CRank;	//生成

	if (pRank != nullptr)
	{//NULLチェック
		//引数の設定
		pRank->m_nNumPlayer = nNumPlayer;

		//初期化
		pRank->Init(D3DXVECTOR3(pos));
	}

	return pRank;
}

//=======================
// テクスチャの設定
//=======================
void CRank::SetTexture(int nNumPlayer)
{
	switch (m_nRank[nNumPlayer])
	{
	case 1:
		CObject2D::SetTexture(CTexture::TEXTURE_FIRST);
		break;

	case 2:
		CObject2D::SetTexture(CTexture::TEXTURE_SECOND);
		break;

	case 3:
		CObject2D::SetTexture(CTexture::TEXTURE_THIRD);
		break;

	case 4:
		CObject2D::SetTexture(CTexture::TEXTURE_FOURTH);
		break;

	default:
		break;
	}
}