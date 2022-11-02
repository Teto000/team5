//========================================================
// プレイヤー選択画面のUIの処理
// Author : 冨所知生
//========================================================
#include "UI_PSelect.h"

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
	//サイズの設定
	CObject2D::SetSize(50.0f, 50.0f);

	// オブジェクトの初期化
	CObject2D::Init(pos);

	// テクスチャの設定
	CObject2D::SetTexture(CTexture::TEXTURE_HUMAN);

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
CUI_PSelect* CUI_PSelect::Create(const D3DXVECTOR3 &pos)
{
	CUI_PSelect* pUI_PSelect = nullptr;

	pUI_PSelect = new CUI_PSelect;

	if (pUI_PSelect != nullptr)
	{
		pUI_PSelect->Init(pos);
	}

	return pUI_PSelect;
}
