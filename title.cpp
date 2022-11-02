//===================================================
//
// タイトルの処理
// Author : Sato Teruto
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "title.h"
#include "renderer.h"
#include "input.h"
#include "input_keybord.h"
#include "input_joypad.h"
#include "application.h"
#include "sound.h"
#include "game.h"
#include "fade.h"

//===========================
// コンストラクタ
//===========================
CTitle::CTitle()
{

}

//===========================
// デストラクタ
//===========================
CTitle::~CTitle()
{

}

//===========================
// 初期化
//===========================
HRESULT CTitle::Init()
{
	m_pObject2D = new CObject2D;
	m_pObject2D->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	m_pObject2D->SetTexture(CTexture::TEXTURE_TITLELOGO);
	m_pObject2D->SetSize(SCREEN_WIDTH, 460.0f);

	return S_OK;
}

//===========================
// 終了
//===========================
void CTitle::Uninit()
{
	m_pObject2D->Uninit();
}

//===========================
// 更新
//===========================
void CTitle::Update()
{
	if (CInputKeyboard::Trigger(DIK_RETURN) == true && CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		CApplication::GetFade()->SetFade(CApplication::MODE_GAME);
	}
}