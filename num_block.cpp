//===================================================
//
// ブロックの数表示の処理
// Author : Sato Teruto
//
//===================================================

//------------------------------
// インクルード
//------------------------------
#include "num_block.h"
#include "number.h"
#include "game.h"

//==================================================
// コンストラクタ
//==================================================
CNumBlock::CNumBlock() : CObject2D(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	m_nNumBlock = 40;				//ブロックの数

	for (int i = 0; i < nMaxDigits; i++)
	{
		m_aPosTexU[i] = {};	//今の桁の数値
		m_pNumber[i] = {};		//数値
	}
}

//==================================================
// デストラクタ
//==================================================
CNumBlock::~CNumBlock()
{

}

//==================================================
// 初期化
//==================================================
HRESULT CNumBlock::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_pos = pos;	//位置

	CObject2D::Init(m_pos);
	CObject2D::SetSize(0.0f, 0.0f);	//サイズの設定

	//------------------------------
	// 数値の設定
	//------------------------------
	for (int i = 0; i < nMaxDigits; i++)
	{
		m_pNumber[i] = CNumber::Create(D3DXVECTOR3((m_pos.x + (40.0f * i)), m_pos.y, m_pos.z));
	}

	//数値の設定
	SetNumber();

	return S_OK;
}

//==================================================
// 終了
//==================================================
void CNumBlock::Uninit()
{
	CObject2D::Uninit();
}

//==================================================
// 更新
//==================================================
void CNumBlock::Update()
{
	CObject2D::Update();

	//数値の設定
	SetNumber();

	//------------------------------
	// 終了処理
	//------------------------------
	if (CGame::GetFinish())
	{//終了フラグが立っているなら
		for (int i = 0; i < nMaxDigits; i++)
		{
			if (m_pNumber[i] != nullptr)
			{//nullじゃないなら
				m_pNumber[i]->Uninit();
				m_pNumber[i] = nullptr;
			}
		}
	}
}

//==================================================
// 描画
//==================================================
void CNumBlock::Draw()
{
	CObject2D::Draw();
}

//==================================================
// 生成
//==================================================
CNumBlock *CNumBlock::Create(D3DXVECTOR3 pos)
{
	CNumBlock *pNumBlock = nullptr;

	//------------------------------
	// ポリゴンの生成と初期化
	//------------------------------
	pNumBlock = new CNumBlock;	//生成

	if (pNumBlock != nullptr)
	{//NULLチェック
		//初期化
		pNumBlock->Init(D3DXVECTOR3(pos));
	}

	return pNumBlock;
}

//==================================================
// 数値の設定
//==================================================
void CNumBlock::SetNumber()
{
	for (int i = 0; i < nMaxDigits; i++)
	{//桁数分回す
		if (m_pNumber[i] != nullptr)
		{//nullじゃないなら
			//powで桁数を出す。
			int nCntNumber = nMaxDigits - i - 1;
			int nNum0 = (int)pow(10, nCntNumber + 1);
			int nNum1 = (int)pow(10, nCntNumber);

			//桁ごとの値を求める
			m_aPosTexU[i] = (m_nNumBlock % nNum0) / nNum1;
			m_pNumber[i]->Set(m_aPosTexU[i]);
		}
	}
}

//==================================================
// 数値の加算
//==================================================
int CNumBlock::AddNumber(int nNumber)
{
	if (m_nNumBlock >= 0 && m_nNumBlock <= 98)
	{//値が範囲内だったら
		//数値の加算
		m_nNumBlock += nNumber;
	}
	else
	{//それ以外なら
		//98に固定
		m_nNumBlock = 98;
	}

	//数値の設定
	SetNumber();

	return m_nNumBlock;
}