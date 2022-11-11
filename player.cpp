//===================================================
//
// �v���C���[�̏���
// Author:Teruto Sato
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
#include "player.h"
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
#include "num_block.h"
#include "num_rank.h"
#include "block.h"
#include "sound.h"

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
const float CPlayer::fPlayerSpeed = 15.0f;
const float CPlayer::fGravity = 0.1f;

//========================
// �R���X�g���N�^
//========================
CPlayer::CPlayer() : CObject(OBJTYPE_MODEL)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ʒu
	m_respornPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���X�|�[���n�_
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړI�̌���
	m_nNumBlock = 0;								//�u���b�N��
	m_pNumBlock = nullptr;							//�u���b�N���̕\��
	m_pRank = nullptr;								//����
}

//========================
// �f�X�g���N�^
//========================
CPlayer::~CPlayer()
{
	// �v���C���[�̑ҋ@���
	m_state = IDOL_STATE;
}

//========================
// ������
//========================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	m_pos = pos;
	m_bJump = false;

	//-----------------------------
	// �u���b�N���̕\��
	//-----------------------------
	{
		//�r���[�|�[�g�̍��W���擾�A�ݒu�ꏊ�̌v�Z
		float X = CGame::GetCameraPlayer(m_nPlayerNum)->GetViewport().X + 50.0f;
		float Y = CGame::GetCameraPlayer(m_nPlayerNum)->GetViewport().Y + 
				(CGame::GetCameraPlayer(m_nPlayerNum)->GetViewport().Height - 50.0f);
		D3DXVECTOR3 Pos(X, Y, 0.0f);
		m_pNumBlock = CNumBlock::Create(Pos);
	}

	//-----------------------------
	// ���ʂ̐���
	//-----------------------------
	{
		//�r���[�|�[�g�̍��W���擾�A�ݒu�ꏊ�̌v�Z
		float X = CGame::GetCameraPlayer(m_nPlayerNum)->GetViewport().X +
			(CGame::GetCameraPlayer(m_nPlayerNum)->GetViewport().Width - 80.0f);

		float Y = CGame::GetCameraPlayer(m_nPlayerNum)->GetViewport().Y +
			(CGame::GetCameraPlayer(m_nPlayerNum)->GetViewport().Height - 50.0f);
		D3DXVECTOR3 Pos(X, Y, 0.0f);
		m_pRank = CRank::Create(Pos, m_nPlayerNum);
	}
	
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		m_pModel[nCnt] = nullptr;
	}

	CRead cRead;

	m_nMotionNum = cRead.ReadMotion("data/MOTION/motionplayer.txt");


	return S_OK;
}

//========================
// �I��
//========================
void CPlayer::Uninit()
{
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			m_pModel[nCnt]->Uninit();
			m_pModel[nCnt]->SetAboveFalse();
			delete m_pModel[nCnt];
			m_pModel[nCnt] = nullptr;
		}
	}

	Release();
}

//========================
// �X�V
//========================
void CPlayer::Update()
{
	//---------------
	// �ړ�
	//---------------
	m_posold = m_pos;	//�ʒu�̕ۑ�
	Move();

#ifdef _DEBUG
	//�f�o�b�N�p
	if (CInputKeyboard::Press(DIK_K))
	{
		m_BlockHave++;
	}

	if (CInputKeyboard::Trigger(DIK_Z))
	{
		m_pModel[0] = CBlock::Create(D3DXVECTOR3(m_pos.x+ 130.0f, -1.0f, m_pos.z), m_rot);
	}

	//----------------------
	// �u���b�N���̑���
	//----------------------
	if (CInputKeyboard::Press(DIK_UP) && m_nPlayerNum == 0)
	{
		m_nNumBlock = m_pNumBlock->AddNumber(1);
	}
	else if (CInputKeyboard::Press(DIK_DOWN) && m_nPlayerNum == 0)
	{
		m_nNumBlock = m_pNumBlock->AddNumber(-1);
	}
#endif // _DEBUG

	//----------------------
	// �����u���b�N���̉��Z
	//----------------------
	//for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	//{
	//	if (m_pModel[nCnt] != nullptr)
	//	{
	//		if (m_pModel[0]->GetHaveBlock())
	//		{
	//			if (m_pModel[0]->GetType() == CBlock::FIELD_BLOCK)
	//			{
	//				m_nNumBlock = m_pNumBlock->AddNumber(1);
	//				m_BlockCnt++;
	//				
	//				// ���f���̍폜
	//				m_pModel[0]->Uninit();
	//				delete m_pModel[nCnt];
	//				m_pModel[0] = nullptr;
	//			}
	//		}
	//	}
	//}

	//-------------------
	//�����蔻��
	//-------------------
	/*CMeshField *m_pMesh = CGame::GetMesh();
	m_pMesh->Collision(&m_pos);*/

	D3DXVECTOR3 pos = CMotionParts::AllCollision(m_nMotionNum,CGame::GetGroundNum(), m_pos, m_posold);
	
	if (m_pos != pos)
	{
		m_pos = pos;
		m_move.y = 0.0f;
		m_respornPos = m_pos;
	}
	else if (m_pos == pos && pos.y < -8.0f)
	{//�R�[�X�A�E�g
		SetBlock();
		CSound::PlaySound(CSound::SOUND_LABEL_SE_FALL1);

		if (m_pos.y <= -150.0f)
		{
			m_pos = m_respornPos;
		}
	}
	

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			m_pModel[nCnt]->Update();
		}
	}

	pos = m_pos;
	//pos.y += 50.0f;

	m_pos = CMotionParts::AllCollisionObstacle(m_nMotionNum, pos, m_posold);

	CMotionParts::MoveMotionModel(m_pos, GetRot(), m_nMotionNum, 1);

	m_rot.y = atan2f(m_move.x, m_move.z) + D3DX_PI;
}

//========================
// �`��
//========================
void CPlayer::Draw()
{
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			m_pModel[nCnt]->Draw();
		}
	}

}

//========================
// ����
//========================
CPlayer* CPlayer::Create(int PlayerNum)
{
	CPlayer *pPlayer = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pPlayer = new CPlayer;	//����

	if (pPlayer != nullptr)
	{//NULL�`�F�b�N
		//������
		pPlayer->m_nPlayerNum = PlayerNum;
		pPlayer->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//pPlayer->SetObjType(OBJTYPE_ENEMY);
	}

	return pPlayer;
}

//========================
// �ړ�
//========================
void CPlayer::Move()
{
	// �v���C���[�̔ԍ����Ƃɑ����ς��鏈��
	switch (m_nPlayerNum)
	{
	case 0:
		//�v���C���[1�̑���
		MoveKey(DIK_W, DIK_A, DIK_S, DIK_D,DIK_SPACE);
		break;

	case 1:
		//�v���C���[2�̑���
		MoveKey(DIK_T, DIK_F, DIK_G, DIK_H, DIK_BACKSPACE);
		break;

	case 2:
		//�v���C���[3�̑���
		MoveKey(DIK_I, DIK_J, DIK_K, DIK_L, DIK_P);
		break;

	default:
		break;
	}

	//�ʒu���X�V
	m_pos += m_move;

	//�����̎���
	m_move.x += (0.0f - m_move.x) * 0.1f;
	m_move.z += (0.0f - m_move.z) * 0.1f;

	////�d�͊֘A
	//if (m_pos.y <= 1.0f)
	//{
	//	m_bJump = false;
	//	m_move.y = 0.0f;
	//	
	//	////���Œn�ʂŎ~�܂鏈��
	//	//m_pos.y = 1.0f;
	//}
	
	Gravity();

	//-------------------------------
	// �W�����v��Ԃ̏���
	//-------------------------------
	if (m_state == JUMP_STATE)
	{//�W�����v��Ԃ̎��̂�
		m_bJump = true;
		Jump();
	}

	//-------------------------------
	// �ʒu�̐ݒ�
	//-------------------------------
	/*if (m_pModel[0])
	{
		m_pModel[0]->SetPos(m_pos);
	}*/

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

	//-------------------------------
	// �����̐ݒ�
	//-------------------------------
	/*if (m_pModel[0])
	{
		m_pModel[0]->SetRot(m_rot);
	}*/
}

//===========================
// �W�����v�̏���
//===========================
void CPlayer::Jump()
{
	m_move.y = 5.0f;
	m_nJumpCount++;

	if (m_nJumpCount == 10)
	{
		//�W�����v�J�E���g�����Z�b�g����
		m_nJumpCount = 0;
		//�v���C���[��ҋ@��Ԃɂ���
		m_state = IDOL_STATE;
	}
	CSound::PlaySound(CSound::SOUND_LABEL_SE_JUMP1);
}

//===========================
// �ʒu�̐ݒ�
//===========================
void CPlayer::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===========================
// �ʒu�̎擾
//===========================
D3DXVECTOR3 CPlayer::GetPosition()
{
	return m_pos;
}

//========================
// �����̎擾
//========================
D3DXVECTOR3 CPlayer::GetRot()
{
	return m_rot;
}

//===========================
// ���̎擾
//===========================
float CPlayer::GetWidth()
{
	return 0.0f;
}

//===========================
// �����̎擾
//===========================
float CPlayer::GetHeight()
{
	return 0.0f;
}

//===========================
// 1F�O�̍��W�̎擾
//===========================
D3DXVECTOR3 CPlayer::GetPosOld()
{
	return D3DXVECTOR3();
}

//=====================================================
// �ړ��L�[�̐ݒ�
// ���� : ��L�[�A���L�[�A���L�[�A�E�L�[�A�W�����v�L�[
//=====================================================
void CPlayer::MoveKey(int UPKey,int LEFTKey,int DOWNKey,int RIGHTKey,int JUMPKey)
{
	// �J�����̊p�x���擾
	D3DXVECTOR3 CameraRot = CGame::GetCameraPlayer(0)->GetRot();;

	//-------------------------------
	// �v���C���[�̑���
	//-------------------------------
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);            // �ړ��ʂ̏�����
	float moveLength = 0.0f;
	D3DXVECTOR2 moveInput;

	moveInput.x = 0.0f;
	moveInput.y = 0.0f;

	// ���f���̈ړ�
	if (CInputKeyboard::Press(UPKey))
	{
		moveInput.y += 1.0f;
		moveLength = 1.0f;
	}
	if (CInputKeyboard::Press(LEFTKey))
	{
		moveInput.x -= 1.0f;
		moveLength = 1.0f;
	}
	if (CInputKeyboard::Press(DOWNKey))
	{
		moveInput.y -= 1.0f;
		moveLength = 1.0f;
	}
	if (CInputKeyboard::Press(RIGHTKey))
	{
		moveInput.x += 1.0f;
		moveLength = 1.0f;
	}

	/*moveInput.y += 1.0f;
	moveLength = 1.0f;*/

	////�W�����v��Ԃł͂Ȃ��Ƃ���
	//if (m_state == IDOL_STATE)
	//{
	//	if (CInputKeyboard::Trigger(JUMPKey) && !m_bJump)
	//	{
	//		// �W�����v��ԂɈڍs
	//		m_state = JUMP_STATE;
	//	}
	//}

	if (moveLength > 0.0f)
	{

		D3DXVec2Normalize(&moveInput, &moveInput);

		float c = cosf(-CameraRot.y);
		float s = sinf(-CameraRot.y);

		// move �̒����� 1 �ɂȂ�B
		m_move.x = moveInput.x * c - moveInput.y * s;
		m_move.z = moveInput.x * s + moveInput.y * c;

		m_move.x *= fPlayerSpeed;
		m_move.z *= fPlayerSpeed;
	}
	else
	{ // ���͂���Ă��Ȃ��B
		return;
	}
}

//=========================================
// �u���b�N��ݒu����
//=========================================
void CPlayer::SetBlock()
{
	// �v���C���[��y���W��0�ȉ��̎�
	if (m_pos.y > 0.0f)
	{
		return;
	}

	// �u���b�N�̏�������1�ȏ�̎�
	if (m_nNumBlock <= 0)
	{
		return;
	}

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (m_pModel[nCnt]->GetBlockCollision())
		{// true��������
			return;
		}
	}

	if (m_pModel[m_BlockCnt] == nullptr)
	{// ���f���̐ݒ�
		m_pModel[m_BlockCnt] = CBlock::Create(D3DXVECTOR3(m_pos.x, -1.0f, m_pos.z), m_rot);
		m_pModel[m_BlockCnt]->SetAbove();
		m_BlockCnt++;
		m_nNumBlock = m_pNumBlock->AddNumber(-1);
		CSound::PlaySound(CSound::SOUND_LABEL_SE_BLOCK);
	}
}

//=========================================
// �d�͂̏���
//=========================================
void CPlayer::Gravity()
{
	//�d��
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (m_pModel[nCnt]->GetBlockCollision())
		{// true��������
			m_pos.y = 0.0f;
			m_move.y = 0.0f;
			SetPosition(m_pos);
			return;
		}
	}

	m_move.y -= fGravity;
}

//=========================================
// �������̉��Z����
//=========================================
void CPlayer::Add(int Add)
{
	// ���������Z
	m_nNumBlock = m_pNumBlock->AddNumber(Add);
}