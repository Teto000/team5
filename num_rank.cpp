//===================================================
//
// 順位の処理
// Author : Sato Teruto
//
//===================================================

//----------------------
// インクルード
//----------------------
#include "num_rank.h"
#include "number.h"
#include "Editor.h"
#include "player.h"
#include "Goal.h"
#include "game.h"

//=======================
// コンストラクタ
//=======================
CRank::CRank() : CObject2D(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	m_aPosTexU = 0;		//今の桁の数値
	m_nRank = 0;		//順位
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
	m_pos = pos;		//位置

	CObject2D::Init(m_pos);
	CObject2D::SetSize(50.0f, 50.0f);	//サイズの設定
	CObject2D::SetTexture(CTexture::TEXTURE_WINNER_FOUR);

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
	D3DXVECTOR2 vec[MAX_PLAYER];
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		vec[i] = D3DXVECTOR2((goalPos.x - playrerPos[i].x),
								(goalPos.y - playrerPos[i].y));
	}

	//ベクトルの小さい順に対応するプレイヤーにランクを入れる

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
CRank *CRank::Create(D3DXVECTOR3 pos)
{
	CRank *pRank = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pRank = new CRank;	//生成

	if (pRank != nullptr)
	{//NULLチェック
		//初期化
		pRank->Init(D3DXVECTOR3(pos));
	}

	return pRank;
}