//===================================================
//
// ���ʂ̏���
// Author : Sato Teruto
//
//===================================================

//----------------------
// �C���N���[�h
//----------------------
#include "num_rank.h"
#include "number.h"
#include "Editor.h"
#include "player.h"
#include "Goal.h"
#include "game.h"

//=======================
// �R���X�g���N�^
//=======================
CRank::CRank() : CObject2D(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
	m_aPosTexU = 0;			//���̌��̐��l
	m_pNumber = nullptr;	//���l

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		m_nRank[i] = 0;	//����
	}
}

//=======================
// �f�X�g���N�^
//=======================
CRank::~CRank()
{

}

//=======================
// ������
//=======================
HRESULT CRank::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_pos = pos;		//�ʒu

	CObject2D::Init(m_pos);
	CObject2D::SetSize(0.0f, 0.0f);	//�T�C�Y�̐ݒ�

	//-----------------------
	// ���l�̐ݒ�
	//-----------------------
	m_pNumber = CNumber::Create(D3DXVECTOR3((m_pos.x + 40.0f), m_pos.y, m_pos.z));

	return S_OK;
}

//=======================
// �I��
//=======================
void CRank::Uninit()
{
	CObject2D::Uninit();
}

//=======================
// �X�V
//=======================
void CRank::Update()
{
	CObject2D::Update();

	//�S�[���ƃv���C���[�̈ʒu���擾
	//�S�[���̈ʒu���擾
	D3DXVECTOR3 goalPos(CEditor::GetGoal()->GetPosition());

	//�v���C���[�̈ʒu���擾
	D3DXVECTOR3 playrerPos[4];
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		playrerPos[i] = CGame::GetPlayer(i)->GetPosition();
	}

	//�x�N�g�����v�Z�A�z��ɓ����

	//�x�N�g���̏��������ɑΉ�����v���C���[�Ƀ����N������
}

//=======================
// �`��
//=======================
void CRank::Draw()
{
	CObject2D::Draw();
}

//=======================
// ����
//=======================
CRank *CRank::Create(D3DXVECTOR3 pos)
{
	CRank *pRank = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pRank = new CRank;	//����

	if (pRank != nullptr)
	{//NULL�`�F�b�N
		//������
		pRank->Init(D3DXVECTOR3(pos));
	}

	return pRank;
}