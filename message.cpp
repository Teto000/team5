//===================================================
//
// ���b�Z�[�W�̏���
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
#include "Goal.h"

//=======================
// �R���X�g���N�^
//=======================
CMessage::CMessage() : CObject2D(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
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
	m_pos = pos;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fWidth = 500.0f;
	m_fHeight = 500.0f;

	CObject2D::Init(m_pos);

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

	//-------------------------
	// �����̐؂�ւ�����
	//-------------------------
	if (m_message == MESSAGE_START && CGame::GetFinish())
	{//�X�^�[�g�������o���� and �I���t���O�������Ă���Ȃ�
	 //���b�Z�[�W�̐؂�ւ�
		ChangeMessage();
	}

	//�e�N�X�`���̐ݒ�
	SetTexture();

	//���ԃJ�E���g
	m_nCntTime++;

	//-------------------------
	// �����ɂ���
	//-------------------------
	if (m_message == MESSAGE_START || m_message == MESSAGE_FINISH)
	{//�X�^�[�g���t�B�j�b�V���Ȃ�
		if (m_nCntTime >= 60)
		{//�J�E���g��60�ȏ�Ȃ�
			m_col.a -= 0.02f;	//�����x�̌���
		}
	}
	else if (m_message != MESSAGE_WINNER_ONE && m_message != MESSAGE_WINNER_TWO
		&& m_message != MESSAGE_WINNER_THREE && m_message != MESSAGE_WINNER_FOUR)
	{//���҂̕�������Ȃ��Ȃ�
		if (m_nCntTime >= 30)
		{//�J�E���g��30�ȏ�Ȃ�
			m_col.a -= 0.02f;	//�����x�̌���
		}
	}

	//�F�̐ݒ�
	CObject2D::SetColor(m_col);

	if (m_col.a <= 0.0f)
	{//���S�ɓ����ɂȂ�����
		if (m_message != MESSAGE_START)
		{//�X�^�[�g�ȊO�̕����Ȃ�
			//���b�Z�[�W�̐؂�ւ�
			ChangeMessage();
		}

		//�J�E���g�̃��Z�b�g
		m_nCntTime = 0;

		return;
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
CMessage *CMessage::Create(D3DXVECTOR3 pos, MESSAGE message)
{
	CMessage *pMessage = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pMessage = new CMessage;	//����

	if (pMessage != nullptr)
	{//NULL�`�F�b�N
		//�����̑��
		pMessage->m_message = message;	//�񋓌^

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

	case MESSAGE_WINNER_ONE:
		CObject2D::SetTexture(CTexture::TEXTURE_COUNT_ONE);
		break;

	case MESSAGE_WINNER_TWO:
		CObject2D::SetTexture(CTexture::TEXTURE_COUNT_TWO);
		break;

	case MESSAGE_WINNER_THREE:
		CObject2D::SetTexture(CTexture::TEXTURE_COUNT_THREE);
		break;

	case MESSAGE_WINNER_FOUR:
		CObject2D::SetTexture(CTexture::TEXTURE_WINNER_FOUR);
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
		m_message = MESSAGE_FINISH;
		break;

	case MESSAGE_FINISH:
		//���������v���C���[�̔ԍ����擾
		switch (CGoal::GetWinner())
		{
		case 0:
			m_message = MESSAGE_WINNER_ONE;
			break;

		case 1:
			m_message = MESSAGE_WINNER_TWO;
			break;

		case 2:
			m_message = MESSAGE_WINNER_THREE;
			break;

		case 3:
			m_message = MESSAGE_WINNER_FOUR;
			break;

		default:
			break;
		}

		//�ʒu�̕ύX
		m_pos.y = 550.0f;
		CObject2D::SetPosition(m_pos);

		//�T�C�Y�̕ύX
		m_fWidth = 700.0f;
		m_fHeight = 400.0f;
		CObject2D::SetSize(m_fWidth, m_fHeight);
		break;

	default:
		break;
	}

	//�����x�����ɖ߂�
	m_col.a = 1.0f;
}