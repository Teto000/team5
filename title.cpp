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
#include "camera.h"
#include "player.h"

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
CMeshField*	CTitle::pMeshField = nullptr;
CCamera*	CTitle::m_pCamera = nullptr;	//�J�����N���X

//===========================
// �R���X�g���N�^
//===========================
CTitle::CTitle()
{

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
	m_pObject2D = new CObject2D;
	m_pObject2D->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	m_pObject2D->SetTexture(CTexture::TEXTURE_TITLELOGO);
	m_pObject2D->SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

	//���b�V���t�B�[���h�̐���
	pMeshField = CMeshField::Create();

	//�J�����̐���
	//�J�����E�v���C���[�̐l���ݒ�͂���
	// �J�����̐���4�Ȃ�
	m_pCamera = CCamera::Create(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);				//����

	return S_OK;
}

//===========================
// �I��
//===========================
void CTitle::Uninit()
{
	if (m_pCamera != nullptr)
	{//�J������null����Ȃ��Ȃ� 
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}
}

//===========================
// �X�V
//===========================
void CTitle::Update()
{
	if (CInputKeyboard::Trigger(DIK_RETURN) == true && CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		CApplication::GetFade()->SetFade(CApplication::MODE_PSELECT);
	}

	m_pCamera->Update();
}