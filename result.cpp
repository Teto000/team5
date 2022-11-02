//===================================================
//
// リザルトの処理
// Author : Sato Teruto
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "result.h"
#include "renderer.h"
#include "input.h"
#include "input_keybord.h"
#include "input_joypad.h"
#include "application.h"
#include "sound.h"
#include "fade.h"

//===========================
// コンストラクタ
//===========================
CResult::CResult()
{

}

//===========================
// デストラクタ
//===========================
CResult::~CResult()
{

}

//===========================
// 初期化
//===========================
HRESULT CResult::Init()
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
void CResult::Uninit()
{

}

//===========================
// 更新
//===========================
void CResult::Update()
{
	if (CInputKeyboard::Trigger(DIK_RETURN) == true && CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{//Enterで次の画面に遷移する
		CApplication::GetFade()->SetFade(CApplication::MODE_TITLE);
	}
}