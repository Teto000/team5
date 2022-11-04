//===================================================
//
// �^�C�g�����S
// Author : Arita Meigen
//
//===================================================

//----------------------
// �C���N���[�h
//----------------------
#include "renderer.h"
#include "input.h"
#include "input_keybord.h"
#include "Titlelogo.h"

//=======================
// �R���X�g���N�^
//=======================
CTitlelogo::CTitlelogo() : CObject2D(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//�F
	m_fWidth = 0.0f;	//��
	m_fHeight = 0.0f;	//����
}

//=======================
// �f�X�g���N�^
//=======================
CTitlelogo::~CTitlelogo()
{

}

//=======================
// ������
//=======================
HRESULT CTitlelogo::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_pos = pos;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fWidth = 500.0f;
	m_fHeight = 500.0f;

	CObject2D::Init(m_pos);

	CObject2D::SetSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

	return S_OK;
}

//=======================
// �I��
//=======================
void CTitlelogo::Uninit()
{
	CObject2D::Uninit();
}

//=======================
// �X�V
//=======================
void CTitlelogo::Update()
{
	CObject2D::Update();
}

//=======================
// �`��
//=======================
void CTitlelogo::Draw()
{
	CObject2D::Draw();
}

//=======================
// ����
//=======================
CTitlelogo *CTitlelogo::Create(D3DXVECTOR3 pos,CTexture::TEXTURE tex)
{
	CTitlelogo *pTitlelogo = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pTitlelogo = new CTitlelogo;	//����

	if (pTitlelogo != nullptr)
	{//NULL�`�F�b�N//������
		pTitlelogo->Init(D3DXVECTOR3(pos));

		//�e�N�X�`���̐ݒ�
		pTitlelogo->SetTexture(tex);
	}

	return pTitlelogo;
}