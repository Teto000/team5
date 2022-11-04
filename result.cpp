//===================================================
//
// ���U���g�̏���
// Author : Sato Teruto
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
#include "result.h"
#include "renderer.h"
#include "input.h"
#include "input_keybord.h"
#include "input_joypad.h"
#include "application.h"
#include "sound.h"
#include "fade.h"
#include "camera_title.h"

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
CCameraTitle* CResult::m_pCameraTitle = nullptr;

//===========================
// �R���X�g���N�^
//===========================
CResult::CResult()
{

}

//===========================
// �f�X�g���N�^
//===========================
CResult::~CResult()
{

}

//===========================
// ������
//===========================
HRESULT CResult::Init()
{
	//�J�����̐���
	m_pCameraTitle = CCameraTitle::Create(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	m_pObject2D = new CObject2D;
	m_pObject2D->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	m_pObject2D->SetTexture(CTexture::TEXTURE_RANKING);
	m_pObject2D->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return S_OK;
}

//===========================
// �I��
//===========================
void CResult::Uninit()
{
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
void CResult::Update()
{
	if (CInputKeyboard::Trigger(DIK_RETURN) == true && CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{//Enter�Ŏ��̉�ʂɑJ�ڂ���
		CApplication::GetFade()->SetFade(CApplication::MODE_TITLE);
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
CCameraTitle* CResult::GetCameraTitle()
{
	return m_pCameraTitle;
}