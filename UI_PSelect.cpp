//========================================================
// プレイヤー選択画面のUIの処理
// Author : 冨所知生
//========================================================
#include "UI_PSelect.h"
#include "renderer.h"

//=========================================
//コンストラクタ
//=========================================
CUI_PSelect::CUI_PSelect()
{
}

//=========================================
//デストラクタ
//=========================================
CUI_PSelect::~CUI_PSelect()
{

}

//=========================================
//オブジェクトの初期化処理
//=========================================
HRESULT CUI_PSelect::Init(D3DXVECTOR3 pos)
{
	switch (m_Tui)
	{
	case UI_PLAYER:
		//サイズの設定
		CObject2D::SetSize(100.0f, 100.0f);
		// テクスチャの設定
		CObject2D::SetTexture(CTexture::TEXTURE_HUMAN);
		break;

	case UI_COMENT:
		//サイズの設定
		CObject2D::SetSize(500.0f, 200.0f);
		// テクスチャの設定
		CObject2D::SetTexture(CTexture::TEXTURE_SELECT_COMENT);
		break;

	case UI_BG:
		//サイズの設定
		CObject2D::SetSize(SCREEN_WIDTH,SCREEN_HEIGHT);
		// テクスチャの設定
		CObject2D::SetTexture(CTexture::TEXTURE_SELECT_BG);
		break;

	case UI_SELECT:
		//サイズの設定
		CObject2D::SetSize(400.0f, 50.0f);
		// テクスチャの設定
		// テクスチャの指定やばすぎるけど気にしないで
		CObject2D::SetTexture(CTexture::TEXTURE_SELECT_SELECT);
		break;
	}

	// オブジェクトの初期化
	CObject2D::Init(pos);

	return S_OK;
}

//=========================================
//オブジェクトの更新処理
//=========================================
void CUI_PSelect::Update()
{
	D3DXVECTOR3 pos;
	pos = GetPosition();
}

//=========================================
//オブジェクトの終了処理
//=========================================
void CUI_PSelect::Uninit()
{
	CObject2D::Uninit();
}

//=========================================
//オブジェクトの描画処理
//=========================================
void CUI_PSelect::Draw()
{
	CObject2D::Draw();
}

//=========================================
//オブジェクトのクリエイト
//=========================================
CUI_PSelect* CUI_PSelect::Create(const D3DXVECTOR3 &pos,const UI &texture)
{
	CUI_PSelect* pUI_PSelect = nullptr;

	pUI_PSelect = new CUI_PSelect;

	if (pUI_PSelect != nullptr)
	{
		pUI_PSelect->Set(texture);
		pUI_PSelect->Init(pos);
	}

	return pUI_PSelect;
}
