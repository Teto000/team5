//===================================================
//
// リザルトの処理
// Author : Sato Teruto
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "tutorial.h"
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
CTutorial::CTutorial()
{

}

//===========================
// デストラクタ
//===========================
CTutorial::~CTutorial()
{

}

//===========================
// 初期化
//===========================
HRESULT CTutorial::Init()
{

	m_pObject2D = new CObject2D;
	m_pObject2D->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	m_pObject2D->SetTexture(CTexture::TEXTURE_TUTORIAL);
	m_pObject2D->SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

	//サウンド生成
	CSound::PlaySound(CSound::SOUND_LABEL_SELECT);

	return S_OK;
}

//===========================
// 終了
//===========================
void CTutorial::Uninit()
{
	//サウンド停止
	CSound::StopSound();
}

//===========================
// 更新
//===========================
void CTutorial::Update()
{
	if (CInputKeyboard::Trigger(DIK_RETURN) == true && CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{//Enterで次の画面に遷移する
		CApplication::GetFade()->SetFade(CApplication::MODE_GAME);
	}

}