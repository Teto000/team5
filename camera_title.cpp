//==================================================
//
// �^�C�g���^�C�g���J�����̏���
// Author:Teruto Sato
//
//==================================================

//----------------------
// �C���N���[�h
//----------------------
#include "camera_title.h"
#include "camera.h"
#include "input.h"
#include "input_keybord.h"
#include "application.h"

//==================================================
// �R���X�g���N�^
//==================================================
CCameraTitle::CCameraTitle()
{

}

//==================================================
// �f�X�g���N�^
//==================================================
CCameraTitle::~CCameraTitle()
{

}

//==================================================
// �^�C�g���J�����̏���������
//==================================================
HRESULT CCameraTitle::Init(void)
{
	//�J�����̏�����
	CCamera::Init();

	return S_OK;
}

//==================================================
// �^�C�g���J�����̏I������
//==================================================
void CCameraTitle::Uninit(void)
{
	//�J�����̏I��
	CCamera::Uninit();
}

//==================================================
// �^�C�g���J�����̍X�V����
//==================================================
void CCameraTitle::Update(void)
{
	//�J�����̍X�V
	CCamera::Update();
}

//==================================================
// �^�C�g���J�����̐ݒ菈��
// ���� : �f�o�C�X
//==================================================
void CCameraTitle::SetCamera(LPDIRECT3DDEVICE9 pDevice)
{
	//�J�����̐ݒ�
	CCamera::SetCamera(pDevice);
}

//==================================================
// �^�C�g���J�����̐���
// ���� : ����̍��W X, ����̍��W Y, ��, ����
//==================================================
CCameraTitle* CCameraTitle::Create(DWORD X, DWORD Y, DWORD Width, DWORD Height)
{
	CCameraTitle *pCameraTitle = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pCameraTitle = new CCameraTitle;	//����

	if (pCameraTitle != nullptr)
	{//NULL�`�F�b�N
	 //�r���[�|�[�g�\���̕ۑ�
		pCameraTitle->m_viewport.X = X;				//�J�n�ʒu(X)
		pCameraTitle->m_viewport.Y = Y;				//�J�n�ʒu(Y)
		pCameraTitle->m_viewport.Width = Width;		//��
		pCameraTitle->m_viewport.Height = Height;	//����

		//������
		pCameraTitle->Init();
	}

	return pCameraTitle;
}