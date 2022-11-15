//===================================================
//
// ���ʂ̏���
// Author : Sato Teruto
//
//===================================================

//------------------------------
// �C���N���[�h
//------------------------------
#include <stdlib.h>
#include <math.h>
#include "num_rank.h"
#include "number.h"
#include "Editor.h"
#include "player.h"
#include "Goal.h"
#include "game.h"

//------------------------------
// �ÓI�����o�ϐ��錾
//------------------------------
int CRank::m_nRank[MAX_PLAYER] = {};		//����
int CRank::m_nDistance[MAX_PLAYER] = {};	//����

//==================================================
// �R���X�g���N�^
//==================================================
CRank::CRank() : CObject2D(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
	m_aPosTexU = 0;		//���̌��̐��l
	m_nNumPlayer = 0;	//�Ή�����v���C���[�ԍ�
}

//==================================================
// �f�X�g���N�^
//==================================================
CRank::~CRank()
{

}

//==================================================
// ������
//==================================================
HRESULT CRank::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_pos = pos;	//�ʒu

	//���ʂ̏����ݒ�
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		m_nRank[i] = (i + 1);
	}

	CObject2D::Init(m_pos);
	CObject2D::SetSize(100.0f, 100.0f);	//�T�C�Y�̐ݒ�

	//�e�N�X�`���̐ݒ�
	SetTexture(m_nNumPlayer);

	return S_OK;
}

//==================================================
// �I��
//==================================================
void CRank::Uninit()
{
	CObject2D::Uninit();
}

//==================================================
// �X�V
//==================================================
void CRank::Update()
{
	CObject2D::Update();

	//------------------------------
	// �S�[���ƃv���C���[�̈ʒu
	//------------------------------
	//�S�[���̈ʒu���擾
	D3DXVECTOR3 goalPos(CEditor::GetGoal()->GetPosition());

	//�v���C���[�̈ʒu���擾
	D3DXVECTOR3 playrerPos(0.0f, 0.0f, 0.0f);
	if (CGame::GetPlayer(m_nNumPlayer) != nullptr)
	{
		playrerPos = CGame::GetPlayer(m_nNumPlayer)->GetPosition();
	}

	//------------------------------
	// �x�N�g�����v�Z�A�z��ɓ����
	//------------------------------
	D3DXVECTOR3 vec = D3DXVECTOR3((goalPos.x - playrerPos.x), 0.0f,
									(goalPos.z - playrerPos.z));

	//------------------------------
	// �S�[���܂ł̋��������߂�
	//------------------------------
	//X��Z�̐�Βl���擾
	int X = fabsf(vec.x);
	int Z = fabsf(vec.z);

	//���Z
	m_nDistance[m_nNumPlayer] = X + Z;

	//------------------------------
	// ���������������ɏ��ʂ�ݒ�
	//------------------------------

	//�e�N�X�`���̐ݒ�
	SetTexture(m_nNumPlayer);

	//------------------------------
	// �I������
	//------------------------------
	if (CGame::GetFinish())
	{//�I���t���O�������Ă���Ȃ�
		Uninit();
	}
}

//==================================================
// �`��
//==================================================
void CRank::Draw()
{
	CObject2D::Draw();
}

//==================================================
// ����
//==================================================
CRank *CRank::Create(D3DXVECTOR3 pos, int nNumPlayer)
{
	CRank *pRank = nullptr;

	//------------------------------
	// �|���S���̐����Ə�����
	//------------------------------
	pRank = new CRank;	//����

	if (pRank != nullptr)
	{//NULL�`�F�b�N
		//�����̐ݒ�
		pRank->m_nNumPlayer = nNumPlayer;

		//������
		pRank->Init(D3DXVECTOR3(pos));
	}

	return pRank;
}

//==================================================
// �e�N�X�`���̐ݒ�
//==================================================
void CRank::SetTexture(int nNumPlayer)
{
	switch (m_nRank[nNumPlayer])
	{
	//1��
	case 1:
		CObject2D::SetTexture(CTexture::TEXTURE_FIRST);
		break;

	//2��
	case 2:
		CObject2D::SetTexture(CTexture::TEXTURE_SECOND);
		break;

	//3��
	case 3:
		CObject2D::SetTexture(CTexture::TEXTURE_THIRD);
		break;

	//4��
	case 4:
		CObject2D::SetTexture(CTexture::TEXTURE_FOURTH);
		break;

	default:
		break;
	}
}