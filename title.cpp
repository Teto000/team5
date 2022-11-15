//===================================================
//
// �^�C�g���̏���
// Author : Sato Teruto
//
//===================================================

//------------------------
// �C���N���[�h
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
#include "meshfield.h"
#include "camera_title.h"
#include "player.h"
#include "Titlelogo.h"
#include "read.h"
#include "3dobject.h"
#include "motion_parts.h"
#include "joypad.h"

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
CMeshField*   CTitle::m_pMeshField = nullptr;
CCameraTitle* CTitle::m_pCameraTitle = nullptr;
CTitlelogo*	  CTitle::m_pTitlelogo = nullptr;


//===========================
// �R���X�g���N�^
//===========================
CTitle::CTitle()
{
	m_pObject2D = nullptr;
}

//===========================
// �f�X�g���N�^
//===========================
CTitle::~CTitle()
{

}

//===========================
// ������
//===========================
HRESULT CTitle::Init()
{
	//���b�V���t�B�[���h�̐���
	//m_pMeshField = CMeshField::Create();
	CRead cRead;
	int nNum = cRead.ReadMotion("data/MOTION/motionground.txt");
	CMotionParts::MoveMotionModel(D3DXVECTOR3(0.0f, -500.0f, 5000.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), nNum, 0);

	//�J�����̐���
	m_pCameraTitle = CCameraTitle::Create(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//�^�C�g�����S�̐���
	m_pTitlelogo = CTitlelogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),CTexture::TEXTURE_TITLELOGO);
	m_pTitlelogo = CTitlelogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), CTexture::TEXTURE_TITLELOGO2);

	//�T�E���h����
	CSound::PlaySound(CSound::SOUND_LABEL_TITLE);

	return S_OK;
}

//===========================
// �I��
//===========================
void CTitle::Uninit()
{
	//�T�E���h��~
	CSound::StopSound();

	C3DObject::UninitAllModel();

	CMotionParts::ALLUninit();

	//---------------------
	// �J�����̏I��
	//---------------------
	if (m_pCameraTitle != nullptr)
	{//�J������null����Ȃ��Ȃ� 
		//�I��
		m_pCameraTitle->Uninit();

		//����
		delete m_pCameraTitle;
		m_pCameraTitle = nullptr;
	}
}

//===========================
// �X�V
//===========================
void CTitle::Update()
{
	if (CInputKeyboard::Trigger(DIK_RETURN) == true && CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		//�T�E���h����
		CSound::PlaySound(CSound::SOUND_LABEL_SE_GAME_START);
		CApplication::GetFade()->SetFade(CApplication::MODE_PSELECT);
	}

	//---------------------
	// �J�����̍X�V
	//---------------------
	if (m_pCameraTitle != nullptr)
	{//�J������null����Ȃ��Ȃ� 
		//�X�V
		m_pCameraTitle->Update();
	}
}

//===========================
// �^�C�g���J�����̎擾
//===========================
CCameraTitle* CTitle::GetCameraTitle()
{
	return m_pCameraTitle;
}