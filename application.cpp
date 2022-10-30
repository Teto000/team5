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

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
CTitle*				CApplication::m_pTitle = nullptr;		//�^�C�g���N���X
CGame*				CApplication::m_pGame = nullptr;		//�Q�[���N���X
CResult*			CApplication::m_pResult = nullptr;		//���U���g�N���X
CTutorial*			CApplication::m_pTutorial = nullptr;	//�`���[�g���A���N���X
CFade*				CApplication::m_pFade = nullptr;		//�t�F�[�h�N���X
CApplication::MODE	CApplication::m_mode = MODE_MAX;		//�Q�[�����[�h

CRenderer*	CApplication::m_pRenderer = nullptr;			//�����_���[
CInput*		CApplication::m_pInput = nullptr;				//�C���v�b�g
CTexture*	CApplication::m_pTexture = nullptr;				//�e�N�X�`��
CSound*		CApplication::m_pSound = nullptr;				//�T�E���h
CCamera*	CApplication::m_pCamera[nDefaultMaxCamera] = {};		//�J����
CLight*		CApplication::m_pLight = nullptr;				//���C�g

bool CApplication::m_bStop = false;	//�v���O�������~����

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
	// �J�����̐����Ə�����
	//----------------------------
	{
		//�J�����̍ő吔�̐ݒ�
		int nNumCamera = CRenderer::SetMaxCamera(NUMCAMERA_ONE);

		DWORD fWidth = SCREEN_WIDTH / 2;
		DWORD fHeight = SCREEN_HEIGHT / 2;

		switch (nNumCamera)
		{
		case NUMCAMERA_ONE:
			//�J�����̐���1�Ȃ�
			m_pCamera[0] = CCamera::Create(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			break;
			
		case NUMCAMERA_TWO:
			//�J�����̐���2�Ȃ�
			m_pCamera[0] = CCamera::Create(0, 0, fWidth, SCREEN_HEIGHT);
			m_pCamera[1] = CCamera::Create(fWidth, 0, fWidth, SCREEN_HEIGHT);
			break;

		case NUMCAMERA_FOUR:
			//�J�����̐���4�Ȃ�
			m_pCamera[0] = CCamera::Create(0, 0, fWidth, fHeight);				//����
			m_pCamera[1] = CCamera::Create(fWidth, 0, fWidth, fHeight);			//�E��
			m_pCamera[2] = CCamera::Create(0, fHeight, fWidth, fHeight);		//����
			m_pCamera[3] = CCamera::Create(fWidth, fHeight, fWidth, fHeight);	//�E��
			break;

		default:
			//�v���O�������~����
			m_bStop = true;

			//�x��
			MessageBox(hWnd, "�J�����̕�����������ł͂���܂���", "�x���I", MB_ICONWARNING);
			exit(m_bStop);	//�v���O�������~����
		}
	}

	//----------------------------
	// ���C�g�̐����Ə�����
	//----------------------------
	m_pLight = new CLight;
	m_pLight->Init(GetRenderer()->GetDevice());

	//----------------------------
	// ���[�h�̐ݒ�
	//----------------------------

	m_pFade = new CFade;
	SetMode(MODE_GAME);
	m_pFade->Init(MODE_GAME);
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

	//�J�����̏I��
	{
		int nNumCamera = CRenderer::GetMaxCamera();
		for (int i = 0; i < nNumCamera; i++)
		{
			if (m_pCamera[i] != nullptr)
			{//�J������null����Ȃ��Ȃ� 
				m_pCamera[i]->Uninit();
				delete m_pCamera[i];
				m_pCamera[i] = nullptr;
			}
		}
	}

	//���C�g�̏I��
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
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

	//�J�����̍X�V
	{
		int nNumCamera = CRenderer::GetMaxCamera();
		for (int i = 0; i < nNumCamera; i++)
		{
			m_pCamera[i]->Update();
		}

		//�Q�[���I�����̏���
		FinishGame();
	}

	//���[�h���Ƃ̍X�V
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Update();
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
		break;

	case MODE_GAME:
		m_pGame->Uninit();
		m_pGame = nullptr;
		break;

	case MODE_RESULT:
		m_pResult->Uninit();
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

	case MODE_GAME:
		m_pGame = nullptr;
		m_pGame = new CGame;
		m_pGame->Init();
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
// �Q�[���I�����̏���
//===========================
void CApplication::FinishGame()
{
	//-------------------------------
	// 1�ʂ̃r���[�|�[�g���g�傷��
	//-------------------------------

	/* 1�ʂ̃v���C���[�ԍ����擾 */
	int nFirstNumber = 1;

	if (CInputKeyboard::Press(DIK_Z) /* 1�ʂ�null����Ȃ��Ȃ� */)
	{//Z��������Ă���Ȃ�

		//-----------------------
		// �r���[�|�[�g���g��
		//-----------------------
		switch (nFirstNumber)
		{
		//-----------------------
		// 1�l�ڂ�������
		//-----------------------
		case CCamera::NUMPLAYER_ONE:
			m_pCamera[nFirstNumber]->AddViewSize(0, 0, nSpeed_X, nSpeed_Y);
			break;

		//-----------------------
		// 2�l�ڂ�������
		//-----------------------
		case CCamera::NUMPLAYER_TWO:
			m_pCamera[nFirstNumber]->AddViewSize(-nSpeed_X, 0, nSpeed_X, nSpeed_Y);
			break;

		//-----------------------
		// 3�l�ڂ�������
		//-----------------------
		case CCamera::NUMPLAYER_THREE:
			m_pCamera[nFirstNumber]->AddViewSize(0, -nSpeed_Y, nSpeed_X, nSpeed_Y);
			break;

		//-----------------------
		// 4�l�ڂ�������
		//-----------------------
		case CCamera::NUMPLAYER_FOUR:
			m_pCamera[nFirstNumber]->AddViewSize(-nSpeed_X, -nSpeed_Y, nSpeed_X, nSpeed_Y);
			break;

		default:
			break;
		}
	}
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
// �J�����̎擾
//===========================
CCamera *CApplication::GetCamera(int nCnt)
{
	return m_pCamera[nCnt];
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