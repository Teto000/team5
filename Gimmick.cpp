//===================================================
//
// �v���C���[�̏���
// Author:Teruto Sato
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
#include "Gimmick.h"
#include "object.h"
#include "application.h"
#include "renderer.h"
#include "input.h"
#include "input_keybord.h"
#include "camera_player.h"
#include "debug_proc.h"
#include "model.h"
#include "meshfield.h"
#include "game.h"
#include "motion_parts.h"
#include "read.h"

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
const float CGimmick::fPlayerSpeed = 1.0f;
const float CGimmick::fGravity = 0.1f;

//========================
// �R���X�g���N�^
//========================
CGimmick::CGimmick() : CObject(OBJTYPE_GIMMICK)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړI�̌���
}

//========================
// �f�X�g���N�^
//========================
CGimmick::~CGimmick()
{

}

//========================
// ������
//========================
HRESULT CGimmick::Init(D3DXVECTOR3 pos)
{
	Load();
	m_pos = pos;

	CRead cRead;
	m_nMotionNum = cRead.ReadMotion(m_nGimmickName[GetType()]);

	return S_OK;
}

//========================
// �I��
//========================
void CGimmick::Uninit()
{
	Release();
}

//========================
// �X�V
//========================
void CGimmick::Update()
{
	//---------------
	// �ړ�
	//---------------
	m_posold = m_pos;	//�ʒu�̕ۑ�
	Move();

	D3DXVECTOR3 pos = CMotionParts::AllCollision(m_nMotionNum, CGame::GetGroundNum(), m_pos, m_posold);

	//���[�V�����Đ�
	CMotionParts::MoveMotionModel(m_pos, GetRot(), m_nMotionNum, 0);

	m_rot.y = atan2f(m_move.x, m_move.z) + D3DX_PI;
}

//========================
// �`��
//========================
void CGimmick::Draw()
{

}

void CGimmick::Load()
{
	//�M�~�b�N�̃I�u�W�F�N�g
	m_nGimmickName[0] = "data/MOTION/Gimmick001Hummer.txt";
	m_nGimmickName[1] = "data/MOTION/Gimmick002RotateStick.txt";
	m_nGimmickName[2] = "data/MOTION/Gimmick003Gate.txt";
	m_nGimmickName[3] = "data/MOTION/Gimmick004ElasticStick.txt";
}

//========================
// ����
//========================
CGimmick* CGimmick::Create(int Type, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CGimmick *pGimmick = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pGimmick = new CGimmick();	//����

	if (pGimmick != nullptr)
	{//NULL�`�F�b�N
	 //������
		pGimmick->SetType(Type) ;
		pGimmick->Init(D3DXVECTOR3(pos));
		pGimmick->m_rot = rot;
	}

	return pGimmick;
}

//========================
// �ړ�
//========================
void CGimmick::Move()
{
	//�ʒu���X�V
	m_pos += m_move;

	//-------------------------------
	// �ړI�̊p�x�̐��K��
	//-------------------------------
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{//��]�������p�x��180�ȏ�Ȃ�
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{//��]�������p�x��-180�ȉ��Ȃ�
		m_rotDest.y += D3DX_PI * 2;
	}

	//-------------------------------
	// �ړI�̊p�x�܂ŉ�]����
	//-------------------------------
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.04f;	//��������

	//-------------------------------
	// �p�x�̐��K��
	//-------------------------------
	if (m_rot.y > D3DX_PI)
	{//�p�x��180�ȏ�Ȃ�
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{//�p�x��-180�ȉ��Ȃ�
		m_rot.y += D3DX_PI * 2;
	}
}


//===========================
// �ʒu�̐ݒ�
//===========================
void CGimmick::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===========================
// �ʒu�̎擾
//===========================
D3DXVECTOR3 CGimmick::GetPosition()
{
	return m_pos;
}

//========================
// �����̎擾
//========================
D3DXVECTOR3 CGimmick::GetRot()
{
	return m_rot;
}

//===========================
// ���̎擾
//===========================
float CGimmick::GetWidth()
{
	return 0.0f;
}

//===========================
// �����̎擾
//===========================
float CGimmick::GetHeight()
{
	return 0.0f;
}

//===========================
// 1F�O�̍��W�̎擾
//===========================
D3DXVECTOR3 CGimmick::GetPosOld()
{
	return D3DXVECTOR3();
}
