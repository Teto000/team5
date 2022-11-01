//===================================================
//
// �v���C���[�̐l���̃Z���N�g.cpp
// Author : Tomoki Tomidokoro
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
#include <time.h>
#include "select_player.h"
#include "renderer.h"
#include "application.h"
#include "input.h"
#include "input_keybord.h"
#include "input_joypad.h"
#include "sound.h"
#include "player.h"
#include "meshfield.h"
#include "debug_proc.h"
#include "fade.h"
#include  "camera.h"
#include "time.h"

//===========================
// �R���X�g���N�^
//===========================
CPSelect::CPSelect()
{
	m_PlayerSelectNum = 0;
}

//===========================
// �f�X�g���N�^
//===========================
CPSelect::~CPSelect()
{

}

//===========================
// ������
//===========================
HRESULT CPSelect::Init()
{
	return S_OK;
}

//===========================
// �I��
//===========================
void CPSelect::Uninit()
{

}

//===========================
// �X�V
//===========================
void CPSelect::Update()
{
	// �v���C���[�̐����w�肷��
	SelectPlayerNum();

	// �J�����̌��̎w��
	CApplication::SetPlayerSelect(m_PlayerSelectNum);

	if (CInputKeyboard::Trigger(DIK_RETURN) == true && CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{//Enter�Ŏ��̉�ʂɑJ�ڂ���
		CApplication::GetFade()->SetFade(CApplication::MODE_GAME);
	}
}

//===========================
// �v���C���[���̑I��
//===========================
void CPSelect::SelectPlayerNum()
{
	// ��
	if (CInputKeyboard::Trigger(DIK_LEFT))
	{// ��O����
		if (m_PlayerSelectNum == 0)
		{// �v���C���[�̐���0���������A���炵����4�ɂȂ鏈��
			m_PlayerSelectNum = MAX_PLAYER;
		}
		else
		{// �E�������΃v���C���[����1����
			m_PlayerSelectNum--;
		}
	}
	// �E
	else if (CInputKeyboard::Trigger(DIK_RIGHT))
	{
		// ��O����
		if (m_PlayerSelectNum == MAX_PLAYER)
		{// �v���C���[�̐���MAX_PLAYER���������A���₵����0�ɂȂ鏈��
			m_PlayerSelectNum = 0;
		}
		else
		{// �E�������΃v���C���[����1������
			m_PlayerSelectNum++;
		}
	}
}
