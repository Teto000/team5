//========================================================
//スコア画面の処理
//========================================================
#include "Score.h"
#include "input.h"
#include <math.h>

int CScore::m_Time = 0;
int CScore::m_Seconds = 0;
int CScore::m_Minutes = 0;

//=========================================
//コンストラクタ
//=========================================
CScore::CScore()
{
}

//=========================================
//デストラクタ
//=========================================
CScore::~CScore()
{

}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CScore::Init(D3DXVECTOR3 pos)
{
	CObject2D::Init(pos);

	//サイズの設定
	CObject2D::SetSize(0.0f, 0.0f);

	//// テクスチャの設定
	//CObject2D::SetTexture(CTexture::TEXTURE_NONE);

	m_pos = GetPosition();

	for (int nCnt = 0; nCnt < MAX_DIGITS; nCnt++)
	{
		m_pNumber[nCnt] = CNumber::Create(D3DXVECTOR3(m_pos.x + 50.0f * nCnt, m_pos.y, m_pos.z));
	}

	// クリアタイムを表示
	m_Minutes = m_Time / 60000;					// 分に変換
	m_Seconds = m_Time - (m_Minutes * 60000);	// 分に変換した分を引いた値を秒に入れる
	Set();

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CScore::Update()
{

}

//=========================================
//オブジェクトの終了処理
//=========================================
void CScore::Uninit()
{
	CObject2D::Uninit();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CScore::Draw()
{
	CObject2D::Draw();
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CScore* CScore::Create(const D3DXVECTOR3 &pos)
{
	CScore* pCScore = nullptr;

	pCScore = new CScore;

	if (pCScore != nullptr)
	{
		pCScore->Init(pos);
	}

	return pCScore;
}

//=========================================
// スコアの加算処理
//=========================================
void CScore::Set()
{
	int aPosTexU[8];		//各桁の数字を格納

	for (int nCnt = 0; nCnt < MAX_DIGITS; nCnt++)
	{
		// powで桁数を出す。
		int nCntNumber = MAX_DIGITS - nCnt - 1;		// defineだと0がないので-1
		int nNum0 = (int)pow(10, nCntNumber + 1);	// 桁数を10000000~の形にする
		int nNum1 = (int)pow(10, nCntNumber);		// 桁数を上より一つ少ない同じ形にする

		// 2桁目
		if (nCnt == 1)
		{// 分
		 // (m_Score % 10^X+1) / 10^X
			aPosTexU[nCnt] = (m_Minutes % (int)pow(10, nCnt));
			m_pNumber[nCnt]->Set(aPosTexU[nCnt]);
		}
		// 1桁目、0割が起きてしまうので↑と同時に処理できない・・・
		else if (nCnt == 0)
		{
			// (m_Score % 10^X+1) / 10^X
			aPosTexU[nCnt] = (m_Minutes * 0.1f);
			m_pNumber[nCnt]->Set(aPosTexU[nCnt]);
		}
		else if (nCnt == 2 || nCnt == 5)
		{// セミコロン
		 // 三桁目にセミコロンを設置する。
			aPosTexU[nCnt] = 11;

			m_pNumber[nCnt]->Set(aPosTexU[nCnt]);
		}
		else if (nCnt == 6 || nCnt == 7)
		{// セミコロン後の数字
			int nNumDec = nNum0 * 10;
			int nNumDec2 = nNum1 * 10;

			// (m_Score % 10^X+1) / 10^X
			aPosTexU[nCnt] = (m_Seconds % nNumDec) / nNumDec2;
			m_pNumber[nCnt]->Set(aPosTexU[nCnt]);
		}
		else
		{// セミコロンとセミコロンの間の数字(なにもしてない)
		 // (m_Score % 10^X+1) / 10^X
			aPosTexU[nCnt] = (m_Seconds % nNum0) / nNum1;
			m_pNumber[nCnt]->Set(aPosTexU[nCnt]);
		}
	}
}

//=========================================
// 最新のスコアを取得
//=========================================
void CScore::SetCurrentScore(int Time)
{
	m_Time = Time;
}
