//=========================================
//
// ランキング処理
// Auther：Arita Meigen
//
//=========================================

//------------------------
// インクルード
//------------------------
#include "Ranking.h"
#include "input.h"
#include <math.h>
#include "application.h"
#include "renderer.h"
#include <string>
#include "Score.h"
#include <vector>
#include <algorithm>

//------------------------
// 静的メンバ変数宣言
//------------------------
int	CRanking::m_Time;					// 時間

//=========================================
//コンストラクタ
//=========================================
CRanking::CRanking()
{

}

//=========================================
//デストラクタ
//=========================================
CRanking::~CRanking()
{

}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CRanking::Init()
{
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		m_apScore[nCnt] = CScore::Create(D3DXVECTOR3(470.0f, 190.0f + 119.0f*nCnt, 0.0f));
	}
	Load();
	Ranking();
	Save();

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CRanking::Update()
{
	if (m_nRankUpdate!=-1)
	{
		//色の設定
		m_apScore[m_nRankUpdate]->SetCor(D3DXCOLOR(1.0f,0.0f,0.0f,1.0f));
	}
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CRanking::Uninit()
{

}

//=========================================
//オブジェクトのクリエイト
//=========================================
CRanking* CRanking::Create()
{
	CRanking* pRanking = nullptr;

	pRanking = new CRanking;

	if (pRanking != nullptr)
	{
		pRanking->Init();
	}

	return pRanking;
}

//=========================================
// 最新のスコアを取得
//=========================================
void CRanking::SetCurrentScore(int Time)
{
	m_Time = Time;
}

//=========================================
// 保存
//=========================================
void CRanking::Save()
{
	FILE*fp = fopen("data\\TXT\\Ranking.txt", "w");		//ファイル読み込み
	const int lenLine = 1024;							//1単語の最大数
	for (int i = 0; i < MAX_RANK; i++)
	{
		if (fp != NULL)
		{
			fprintf(fp, "%d\n", m_apScore[i]->GetTime());	//読み込んだ文字ごとに設定する
		}
	}

	fclose(fp);
}

//=========================================
// 読み込み
//=========================================
void CRanking::Load()
{
	FILE*fp = fopen("data\\TXT\\Ranking.txt", "r");		//ファイル読み込み
	const int lenLine = 1024;							//1単語の最大数
	int nrScore = 0;									//読み込み用の文字列

	for (int i = 0; i < MAX_RANK; i++)
	{//ランキングの読み込み
		if (fp != NULL)
		{
			fscanf(fp, "%d", &nrScore);			//読み込んだ文字ごとに設定する
			m_apScore[i]->SetTime(nrScore);		//設定
		}
	}

	fclose(fp);
}

//=========================================
// ランキングの並び替え
//=========================================
void CRanking::Ranking()
{
	//ランク外にしておく
	m_nRankUpdate = -1;
	if (m_Time <= m_apScore[MAX_RANK-1]->GetTime())
	{//最下位と比較
		m_apScore[MAX_RANK - 1]->SetTime(m_Time);

		//昇順に並び変える
		std::vector<int> rank = { m_apScore[0]->GetTime(), m_apScore[1]->GetTime(), m_apScore[2]->GetTime(),
			m_apScore[3]->GetTime(), m_apScore[4]->GetTime() };
		std::sort(rank.begin(), rank.end());
		for (int i = 0; i < MAX_RANK; i++)
		{//並び変えたやつを代入
			m_apScore[i]->SetTime(rank[i]);
			if (m_Time == rank[i])
			{
				m_nRankUpdate = i;
			}
		}
	}
}