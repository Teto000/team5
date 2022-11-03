//===================================================
//
// �|���S��2D�̏���
// Author : Sato Teruto
//
//===================================================

//----------------------
// �C���N���[�h
//----------------------
#include "message.h"
#include "renderer.h"
#include "input.h"
#include "input_keybord.h"

//=======================
// �R���X�g���N�^
//=======================
CMessage::CMessage() : CObject2D(0)
{
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//�F
	m_nCntTime = 0;		//���ԃJ�E���g
	m_fWidth = 0.0f;	//��
	m_fHeight = 0.0f;	//����
	m_message = MESSAGE_COUNT_THREE;
}

//=======================
// �f�X�g���N�^
//=======================
CMessage::~CMessage()
{

}

//=======================
// ������
//=======================
HRESULT CMessage::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fWidth = 500.0f;
	m_fHeight = 500.0f;

	CObject2D::Init(pos);

	CObject2D::SetSize(500.0f, 500.0f);

	//�e�N�X�`���̐ݒ�
	SetTexture();

	return S_OK;
}

//=======================
// �I��
//=======================
void CMessage::Uninit()
{
	CObject2D::Uninit();
}

//=======================
// �X�V
//=======================
void CMessage::Update()
{
	CObject2D::Update();

	//�e�N�X�`���̐ݒ�
	SetTexture();

	//���ԃJ�E���g
	m_nCntTime++;

	if (m_nCntTime >= 30)
	{//�J�E���g��30�ȏ�Ȃ�
		//-------------------
		// �����ɂ���
		//-------------------
		m_col.a -= 0.02f;	//�����x�̌���

		//�F�̐ݒ�
		CObject2D::SetColor(m_col);

		if (m_col.a <= 0.0f)
		{//���S�ɓ����ɂȂ�����
			m_nCntTime = 0;

			ChangeMessage();
			return;
		}
	}
}

//=======================
// �`��
//=======================
void CMessage::Draw()
{
	CObject2D::Draw();
}

//=======================
// ����
//=======================
CMessage *CMessage::Create(D3DXVECTOR3 pos)
{
	CMessage *pMessage = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pMessage = new CMessage;	//����

	if (pMessage != nullptr)
	{//NULL�`�F�b�N
	 //������
		pMessage->Init(D3DXVECTOR3(pos));
		//pMessage->SetObjType(OBJTYPE_ENEMY);
	}

	return pMessage;
}

//=======================
// �e�N�X�`���̐ݒ�
//=======================
void CMessage::SetTexture()
{
	switch (m_message)
	{
	case MESSAGE_COUNT_ONE:
		CObject2D::SetTexture(CTexture::TEXTURE_COUNT_ONE);
		break;

	case MESSAGE_COUNT_TWO:
		CObject2D::SetTexture(CTexture::TEXTURE_COUNT_TWO);
		break;

	case MESSAGE_COUNT_THREE:
		CObject2D::SetTexture(CTexture::TEXTURE_COUNT_THREE);
		break;

	case MESSAGE_START:
		CObject2D::SetTexture(CTexture::TEXTURE_START);
		break;

	case MESSAGE_FINISH:
		CObject2D::SetTexture(CTexture::TEXTURE_FINISH);
		break;

	default:
		break;
	}
}

//=======================
// ���b�Z�[�W�̕ύX
//=======================
void CMessage::ChangeMessage()
{
	switch (m_message)
	{
	case MESSAGE_COUNT_ONE:
		m_message = MESSAGE_START;
		break;

	case MESSAGE_COUNT_TWO:
		m_message = MESSAGE_COUNT_ONE;
		break;

	case MESSAGE_COUNT_THREE:
		m_message = MESSAGE_COUNT_TWO;
		break;

	case MESSAGE_START:
		//���b�Z�[�W�̏���
		Uninit();
		break;

	default:
		break;
	}

	//�����x�����ɖ߂�
	m_col.a = 1.0f;
}