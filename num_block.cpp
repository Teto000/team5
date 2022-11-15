//===================================================
//
// �u���b�N�̐��\���̏���
// Author : Sato Teruto
//
//===================================================

//------------------------------
// �C���N���[�h
//------------------------------
#include "num_block.h"
#include "number.h"
#include "game.h"

//==================================================
// �R���X�g���N�^
//==================================================
CNumBlock::CNumBlock() : CObject2D(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
	m_nNumBlock = 40;				//�u���b�N�̐�

	for (int i = 0; i < nMaxDigits; i++)
	{
		m_aPosTexU[i] = {};	//���̌��̐��l
		m_pNumber[i] = {};		//���l
	}
}

//==================================================
// �f�X�g���N�^
//==================================================
CNumBlock::~CNumBlock()
{

}

//==================================================
// ������
//==================================================
HRESULT CNumBlock::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_pos = pos;	//�ʒu

	CObject2D::Init(m_pos);
	CObject2D::SetSize(0.0f, 0.0f);	//�T�C�Y�̐ݒ�

	//------------------------------
	// ���l�̐ݒ�
	//------------------------------
	for (int i = 0; i < nMaxDigits; i++)
	{
		m_pNumber[i] = CNumber::Create(D3DXVECTOR3((m_pos.x + (40.0f * i)), m_pos.y, m_pos.z));
	}

	//���l�̐ݒ�
	SetNumber();

	return S_OK;
}

//==================================================
// �I��
//==================================================
void CNumBlock::Uninit()
{
	CObject2D::Uninit();
}

//==================================================
// �X�V
//==================================================
void CNumBlock::Update()
{
	CObject2D::Update();

	//���l�̐ݒ�
	SetNumber();

	//------------------------------
	// �I������
	//------------------------------
	if (CGame::GetFinish())
	{//�I���t���O�������Ă���Ȃ�
		for (int i = 0; i < nMaxDigits; i++)
		{
			if (m_pNumber[i] != nullptr)
			{//null����Ȃ��Ȃ�
				m_pNumber[i]->Uninit();
				m_pNumber[i] = nullptr;
			}
		}
	}
}

//==================================================
// �`��
//==================================================
void CNumBlock::Draw()
{
	CObject2D::Draw();
}

//==================================================
// ����
//==================================================
CNumBlock *CNumBlock::Create(D3DXVECTOR3 pos)
{
	CNumBlock *pNumBlock = nullptr;

	//------------------------------
	// �|���S���̐����Ə�����
	//------------------------------
	pNumBlock = new CNumBlock;	//����

	if (pNumBlock != nullptr)
	{//NULL�`�F�b�N
		//������
		pNumBlock->Init(D3DXVECTOR3(pos));
	}

	return pNumBlock;
}

//==================================================
// ���l�̐ݒ�
//==================================================
void CNumBlock::SetNumber()
{
	for (int i = 0; i < nMaxDigits; i++)
	{//��������
		if (m_pNumber[i] != nullptr)
		{//null����Ȃ��Ȃ�
			//pow�Ō������o���B
			int nCntNumber = nMaxDigits - i - 1;
			int nNum0 = (int)pow(10, nCntNumber + 1);
			int nNum1 = (int)pow(10, nCntNumber);

			//�����Ƃ̒l�����߂�
			m_aPosTexU[i] = (m_nNumBlock % nNum0) / nNum1;
			m_pNumber[i]->Set(m_aPosTexU[i]);
		}
	}
}

//==================================================
// ���l�̉��Z
//==================================================
int CNumBlock::AddNumber(int nNumber)
{
	if (m_nNumBlock >= 0 && m_nNumBlock <= 98)
	{//�l���͈͓���������
		//���l�̉��Z
		m_nNumBlock += nNumber;
	}
	else
	{//����ȊO�Ȃ�
		//98�ɌŒ�
		m_nNumBlock = 98;
	}

	//���l�̐ݒ�
	SetNumber();

	return m_nNumBlock;
}