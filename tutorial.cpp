//===================================================
//
// ���U���g�̏���
// Author : Sato Teruto
//
//===================================================

//------------------------
// �C���N���[�h
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
// �R���X�g���N�^
//===========================
CTutorial::CTutorial()
{

}

//===========================
// �f�X�g���N�^
//===========================
CTutorial::~CTutorial()
{

}

//===========================
// ������
//===========================
HRESULT CTutorial::Init()
{

	m_pObject2D = new CObject2D;
	m_pObject2D->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	m_pObject2D->SetTexture(CTexture::TEXTURE_TUTORIAL);
	m_pObject2D->SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

	//�T�E���h����
	CSound::PlaySound(CSound::SOUND_LABEL_SELECT);

	return S_OK;
}

//===========================
// �I��
//===========================
void CTutorial::Uninit()
{
	//�T�E���h��~
	CSound::StopSound();
}

//===========================
// �X�V
//===========================
void CTutorial::Update()
{
	if (CInputKeyboard::Trigger(DIK_RETURN) == true && CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{//Enter�Ŏ��̉�ʂɑJ�ڂ���
		CApplication::GetFade()->SetFade(CApplication::MODE_GAME);
	}

}