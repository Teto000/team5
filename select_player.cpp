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
#include "UI_PSelect.h"

#define MAX_WINDOW (3)
//===========================
// �R���X�g���N�^
//===========================
CPSelect::CPSelect()
{
	m_PlayerSelectNum = 0;

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_PUI[nCnt] = nullptr;
	}
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
	UI_Create();

	//m_BGUI = CUI_PSelect::Create(D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2.0f, 0.0f));

	//m_BGUI->SetSize(SCREEN_WIDTH,SCREEN_HEIGHT * 0.5f);

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
	CGame::SetPlayerNum(m_PlayerSelectNum);

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
			m_PlayerSelectNum = MAX_WINDOW;
			// 0�ɂȂ����̂ōĐ���
			UI_Create();
		}
		else
		{
			// �E�������΃v���C���[����1����
			m_PlayerSelectNum--;
			//UI�̐���
			UI_Create();
		}
	}
	// �E
	else if (CInputKeyboard::Trigger(DIK_RIGHT))
	{
		// ��O����
		if (m_PlayerSelectNum == MAX_WINDOW)
		{
			// �v���C���[�̐���MAX_PLAYER���������A���₵����0�ɂȂ鏈��
			m_PlayerSelectNum = 0;
			// 0�ɂȂ����̂ōĐ���
			UI_Create();
		}
		else
		{
		// �E�������΃v���C���[����1������
			m_PlayerSelectNum++;
		//UI�̐���
			UI_Create();
		}
	}
}

//===========================
// �v���C���[���ɉ�����UI�̐���
//===========================
void CPSelect::UI_Create()
{
	if (m_PUI[0] != nullptr)
	{
		// �\�����Ă���UI�̍폜
		UI_Uninit();
	}

	switch (m_PlayerSelectNum)
	{
	case 0:
		//UI�̐���
		m_PUI[0] = CUI_PSelect::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
		break;

	case 1:
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			m_PUI[nCnt] = CUI_PSelect::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - 100.0f + nCnt * 200.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
		}
		break;

	case 2:
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			m_PUI[nCnt] = CUI_PSelect::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - 150.0f + nCnt * 150.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
		}
		break;

	case 3:
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			m_PUI[nCnt] = CUI_PSelect::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - 300.0f + nCnt * 200.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
		}
		break;
	}
}

//===========================
// �S�Ă̔j��
//===========================
void CPSelect::UI_Uninit()
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_PUI[nCnt] != nullptr)
		{
			m_PUI[nCnt]->Uninit();
			m_PUI[nCnt] = nullptr;
		}
	}
}