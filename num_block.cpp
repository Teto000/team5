//===================================================
//
// �u���b�N�̐��\���̏���
// Author : Sato Teruto
//
//===================================================

//----------------------
// �C���N���[�h
//----------------------
#include "num_block.h"
#include "number.h"

//=======================
// �R���X�g���N�^
//=======================
CNumBlock::CNumBlock() : CObject2D(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
	m_nNumBlock = 0;				//�u���b�N�̐�

	for (int i = 0; i < nMaxDigits; i++)
	{
		m_aPosTexU[i] = {};	//���̌��̐��l
		m_pNumber[i] = {};		//���l
	}
}

//=======================
// �f�X�g���N�^
//=======================
CNumBlock::~CNumBlock()
{

}

//=======================
// ������
//=======================
HRESULT CNumBlock::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_pos = pos;		//�ʒu

	CObject2D::Init(m_pos);
	CObject2D::SetSize(0.0f, 0.0f);	//�T�C�Y�̐ݒ�

	//-----------------------
	// ���l�̐ݒ�
	//-----------------------
	for (int i = 0; i < nMaxDigits; i++)
	{
		m_pNumber[i] = CNumber::Create(D3DXVECTOR3((m_pos.x + (40.0f * i)), m_pos.y, m_pos.z));
	}

	//���l�̐ݒ�
	SetNumber();

	return S_OK;
}

//=======================
// �I��
//=======================
void CNumBlock::Uninit()
{
	CObject2D::Uninit();
}

//=======================
// �X�V
//=======================
void CNumBlock::Update()
{
	CObject2D::Update();

	//���l�̐ݒ�
	SetNumber();
}

//=======================
// �`��
//=======================
void CNumBlock::Draw()
{
	CObject2D::Draw();
}

//=======================
// ����
//=======================
CNumBlock *CNumBlock::Create(D3DXVECTOR3 pos)
{
	CNumBlock *pNumBlock = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pNumBlock = new CNumBlock;	//����

	if (pNumBlock != nullptr)
	{//NULL�`�F�b�N
		//������
		pNumBlock->Init(D3DXVECTOR3(pos));
	}

	return pNumBlock;
}

//=======================
// ���l�̐ݒ�
//=======================
void CNumBlock::SetNumber()
{
	for (int i = 0; i < nMaxDigits; i++)
	{
		//pow�Ō������o���B
		int nCntNumber = nMaxDigits - i - 1;
		int nNum0 = (int)pow(10, nCntNumber + 1);
		int nNum1 = (int)pow(10, nCntNumber);

		//�����Ƃ̒l�����߂�
		m_aPosTexU[i] = (m_nNumBlock % nNum0) / nNum1;
		m_pNumber[i]->Set(m_aPosTexU[i]);
	}
}

//=======================
// ���l�̉��Z
//=======================
int CNumBlock::AddNumber(int nNumber)
{
	//���Z
	m_nNumBlock += nNumber;

	//���l�̐ݒ�
	SetNumber();

	return m_nNumBlock;
}