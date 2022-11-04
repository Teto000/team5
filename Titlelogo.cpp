//===================================================
//
// タイトルロゴ
// Author : Arita Meigen
//
//===================================================

//----------------------
// インクルード
//----------------------
#include "renderer.h"
#include "input.h"
#include "input_keybord.h"
#include "Titlelogo.h"

//=======================
// コンストラクタ
//=======================
CTitlelogo::CTitlelogo() : CObject2D(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//色
	m_fWidth = 0.0f;	//幅
	m_fHeight = 0.0f;	//高さ
}

//=======================
// デストラクタ
//=======================
CTitlelogo::~CTitlelogo()
{

}

//=======================
// 初期化
//=======================
HRESULT CTitlelogo::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_pos = pos;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fWidth = 500.0f;
	m_fHeight = 500.0f;

	CObject2D::Init(m_pos);

	CObject2D::SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

	return S_OK;
}

//=======================
// 終了
//=======================
void CTitlelogo::Uninit()
{
	CObject2D::Uninit();
}

//=======================
// 更新
//=======================
void CTitlelogo::Update()
{
	CObject2D::Update();
}

//=======================
// 描画
//=======================
void CTitlelogo::Draw()
{
	CObject2D::Draw();
}

//=======================
// 生成
//=======================
CTitlelogo *CTitlelogo::Create(D3DXVECTOR3 pos,CTexture::TEXTURE tex)
{
	CTitlelogo *pTitlelogo = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pTitlelogo = new CTitlelogo;	//生成

	if (pTitlelogo != nullptr)
	{//NULLチェック//初期化
		pTitlelogo->Init(D3DXVECTOR3(pos));

		//テクスチャの設定
		pTitlelogo->SetTexture(tex);
	}

	return pTitlelogo;
}