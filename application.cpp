//===================================================
//
// �A�v���P�[�V�����̏���
// Author : Sato Teruto
//
//===================================================

 //------------------------
// �C���N���[�h
//------------------------
#include <stdlib.h>
#include "application.h"
#include "renderer.h"
#include "object3d.h"
#include "texture.h"
#include "sound.h"
#include "input.h"
#include "input_keybord.h"
#include "input_joypad.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "Goal.h"
#include "select_player.h"

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
CTitle*				CApplication::m_pTitle = nullptr;		//�^�C�g���N���X
CPSelect*			CApplication::m_pPSelect = nullptr;		//�v���C���[�l���w��N���X
CGame*				CApplication::m_pGame = nullptr;		//�Q�[���N���X
CResult*			CApplication::m_pResult = nullptr;		//���U���g�N���X
CTutorial*			CApplication::m_pTutorial = nullptr;	//�`���[�g���A���N���X
CFade*				CApplication::m_pFade = nullptr;		//�t�F�[�h�N���X
CApplication::MODE	CApplication::m_mode = MODE_MAX;		//�Q�[�����[�h

CRenderer*	CApplication::m_pRenderer = nullptr;			//�����_���[
CInput*		CApplication::m_pInput = nullptr;				//�C���v�b�g
CTexture*	CApplication::m_pTexture = nullptr;				//�e�N�X�`��
CSound*		CApplication::m_pSound = nullptr;				//�T�E���h
CLight*		CApplication::m_pLight = nullptr;				//���C�g

//===========================
// �R���X�g���N�^
//===========================
CApplication::CApplication()
{

}

//===========================
// �f�X�g���N�^
//===========================
CApplication::~CApplication()
{

}

//===========================
// ������
//===========================
HRESULT CApplication::Init(HINSTANCE hInstance, HWND hWnd)
{
	//----------------------------
	// �����_�����O�̐����Ə�����
	//----------------------------
	m_pRenderer = new CRenderer;
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{//���������������s�����ꍇ
		return -1;
	}

	//----------------------------
	// �C���v�b�g�̐����Ə�����
	//----------------------------
	m_pInput = new CInput;
	m_pInput->Init(hInstance, hWnd);

	//----------------------------
	// �e�N�X�`���̐���
	//----------------------------
	m_pTexture = new CTexture;

	//----------------------------
	// �T�E���h�̐����Ə�����
	//----------------------------
	//m_pSound = new CSound;
	//m_pSound->Init(hWnd);

	//----------------------------
	// ���C�g�̐����Ə�����
	//----------------------------
	m_pLight = new CLight;
	m_pLight->Init(GetRenderer()->GetDevice());

	//----------------------------
	// ���[�h�̐ݒ�
	//----------------------------
	m_pFade = new CFade;
	SetMode(MODE_TITLE);
	m_pFade->Init(MODE_TITLE);

	return S_OK;
}

//===========================
// �I��
//===========================
void CApplication::Uninit()
{
	//�I�u�W�F�N�g�̑S���
	CObject::ReleaseAll(true);

	//�e�N�X�`���̏I��
	if (m_pTexture != nullptr)
	{
		m_pTexture->ReleaseAll();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	//�����_�����O�̏I��
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	//�C���v�b�g�̏I��
	if (m_pInput != nullptr)
	{
		m_pInput->Uninit();
		delete m_pInput;
		m_pInput = nullptr;
	}

	//�T�E���h�̏I��
	/*if (m_pSound != nullptr)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}*/

	//���C�g�̏I��
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}

	//�Q�[���̏I��
	if (m_pGame != nullptr)
	{
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = nullptr;
	}

	//�t�F�[�h�̏I��
	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}
}

//===========================
// �X�V
//===========================
void CApplication::Update()
{
	//�C���v�b�g�̍X�V
	m_pInput->Update();	//�ŏ��ɂ��

	//�����_�����O�̍X�V
	m_pRenderer->Update();

	//���[�h���Ƃ̍X�V
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Update();
		break;

	case MODE_PSELECT:
		m_pPSelect->Update();
		break;

	case MODE_GAME:
		m_pGame->Update();
		break;

	case MODE_RESULT:
		m_pResult->Update();
		break;

	default:
		break;
	}

	//�t�F�[�h�̍X�V
	m_pFade->Update();
}

//===========================
// �`��
//===========================
void CApplication::Draw()
{
	//�����_�����O�̕`��
	m_pRenderer->Draw();

	//�t�F�[�h�̕`��
	m_pFade->Draw();
}

//===========================
// ���[�h�̐ݒ�
//===========================
void CApplication::SetMode(MODE mode)
{
	//���݂̃��[�h���I��
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Uninit();
		delete m_pTitle;
		m_pTitle = nullptr;
		break;

	case MODE_PSELECT:
		m_pPSelect->Uninit();
		delete m_pPSelect;
		m_pPSelect = nullptr;
		break;

	case MODE_GAME:
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = nullptr;
		break;

	case MODE_RESULT:
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = nullptr;
		break;

	default:
		break;
	}

	//�I�u�W�F�N�g�̑S���
	CObject::ReleaseAll(true);
	//m_pFade->Create2DObject();

	//���[�h�̐؂�ւ�
	m_mode = mode;

	//�V�������[�h�̐���
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle = nullptr;
		m_pTitle = new CTitle;
		m_pTitle->Init();
		break;

	case MODE_PSELECT:
		m_pPSelect = nullptr;
		m_pPSelect = new CPSelect;
		m_pPSelect->Init();
		break;

	case MODE_GAME:
		m_pGame = nullptr;
		m_pGame = new CGame;
		m_pGame->Init();

		//�J�����̑傫���̃��Z�b�g
		m_pGame->ResetCameraSize();
		break;

	case MODE_RESULT:
		m_pResult = nullptr;
		m_pResult = new CResult;
		m_pResult->Init();
		break;

	default:
		break;
	}

	//�t�F�[�h�̐���
	/*m_pFade = nullptr;
	m_pFade = new CFade;
	m_pFade->Init(m_mode);*/
}

//===========================
// ���[�h�̎擾
//===========================
CApplication::MODE CApplication::GetMode()
{
	return m_mode;
}

//===========================
// �Q�[���̎擾
//===========================
CGame* CApplication::GetGame()
{
	return m_pGame;
}

//===========================
// �����_�����O�̎擾
//===========================
CRenderer *CApplication::GetRenderer()
{
	return m_pRenderer;
}

//===========================
// �C���v�b�g�̎擾
//===========================
CInput *CApplication::GetInput()
{
	return m_pInput;
}

//===========================
// �e�N�X�`���̎擾
//===========================
CTexture *CApplication::GetTexture()
{
	return m_pTexture;
}

//===========================
// �T�E���h�̎擾
//===========================
CSound *CApplication::GetSound()
{
	return m_pSound;
}

//===========================
// ���C�g�̎擾
//===========================
CLight *CApplication::GetLight()
{
	return m_pLight;
}

//===========================
// �t�F�[�h�̎擾
//===========================
CFade *CApplication::GetFade()
{
	return m_pFade;
}

//===========================
// �Z���N�g�̎擾
//===========================
CPSelect *CApplication::GetPSelect()
{
	return m_pPSelect;
}