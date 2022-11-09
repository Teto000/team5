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
	m_aPosTexU = 0;		//���̌��̐��l
	m_nRank = 0;		//����
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
	CObject2D::SetSize(50.0f, 50.0f);	//�T�C�Y�̐ݒ�
	CObject2D::SetTexture(CTexture::TEXTURE_WINNER_FOUR);

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
	D3DXVECTOR3 playrerPos[MAX_PLAYER];
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (CGame::GetPlayer(i) != nullptr)
		{
			playrerPos[i] = CGame::GetPlayer(i)->GetPosition();
		}
	}

	//�x�N�g�����v�Z�A�z��ɓ����
	D3DXVECTOR2 vec[MAX_PLAYER];
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		vec[i] = D3DXVECTOR2((goalPos.x - playrerPos[i].x),
								(goalPos.y - playrerPos[i].y));
	}

	//�x�N�g���̏��������ɑΉ�����v���C���[�Ƀ����N������

	if (CGame::GetFinish())
	{//�I���t���O�������Ă���Ȃ�
		Uninit();
	}
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