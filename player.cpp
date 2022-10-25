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
#include "camera.h"
#include "debug_proc.h"
#include "model.h"

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
const float CPlayer::fPlayerSpeed = 1.0f;
const float CPlayer::fGravity = 0.6f;
CShadow* CPlayer::m_pShadow = nullptr;
CBullet* CPlayer::m_pBullet = nullptr;
CModel*	 CPlayer::m_pModel[MAX_PARTS] = {};

//------------------------
// �O���[�o���ϐ�
//------------------------
CPlayer::KEY_SET g_aKeySet[] =	//�L�[�Z�b�g���
{
	//----------------------
	// �L�[1
	//----------------------
	{ 40,	//�t���[����
		//		Pos				Rot
		{{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//��
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//��
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//�E��
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//����
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//�E��
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//����
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f }},	//��
	},

	//----------------------
	// �L�[2
	//----------------------
	{ 40,	//�t���[����
		//		Pos				Rot
		{{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//��
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//��
		{ 0.0f,0.0f,0.0f , 90.0f,0.0f,0.0f },		//�E��
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//����
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//�E��
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//����
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f }},	//��
	},
};

//========================
// �R���X�g���N�^
//========================
CPlayer::CPlayer() : CObject(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړI�̌���

	/* �� ���[�V������� �� */
	m_nCurrentKey = 0;
	m_nCntMotion = 0;
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
	m_nCntMotion = 1;
	m_bJump = false;
	//--------------------
	// ���f���̐���
	//--------------------
	SetModel();

	return S_OK;
}

//========================
// �I��
//========================
void CPlayer::Uninit()
{
	//-------------------------
	// ���f���̏I��
	//-------------------------
	for (int i = 0; i < MAX_PARTS; i++)
	{//���f���p�[�c������
		if (m_pModel[i])
		{//���f����null����Ȃ��Ȃ�
			m_pModel[i]->Uninit();
		}
	}

	CObject::Release();
}

//========================
// �X�V
//========================
void CPlayer::Update()
{
	//-------------------------
	// ���f���̍X�V
	//-------------------------
	for (int i = 0; i < MAX_PARTS; i++)
	{//���f���p�[�c������
		if (m_pModel[i])
		{//���f����null����Ȃ��Ȃ�
			m_pModel[i]->Update();
		}
	}
	//---------------
	// �ړ�
	//---------------
	Move();

	//-------------------
	// ���[�V����
	//-------------------
	SetMotion(true);
}

//========================
// �`��
//========================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//�f�o�C�X�̎擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���f���̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//���f���̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//���f���̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-------------------------
	// ���f���̕`��
	//-------------------------
	for (int i = 0; i < MAX_PARTS; i++)
	{//���f���p�[�c������
		if (!m_pModel[i])
		{//���f����null�Ȃ�
			return;
		}

		m_pModel[i]->Draw();
	}
}

//========================
// ����
//========================
CPlayer* CPlayer::Create()
{
	CPlayer *pPlayer = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pPlayer = new CPlayer;	//����

	if (pPlayer != nullptr)
	{//NULL�`�F�b�N
		//������
		pPlayer->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//pPlayer->SetObjType(OBJTYPE_ENEMY);
	}

	return pPlayer;
}

//========================
// ���f���̐���
//========================
void CPlayer::SetModel()
{
	//���f��0
	m_pModel[0] = CModel::Create("data\\MODEL\\body.x", D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pModel[0]->SetParent(nullptr);

	//���f��1
	m_pModel[1] = CModel::Create("data\\MODEL\\head.x", D3DXVECTOR3(0.0f, 40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pModel[1]->SetParent(m_pModel[0]);

	//���f��2
	m_pModel[2] = CModel::Create("data\\MODEL\\armR.x", D3DXVECTOR3(-20.0f, 40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pModel[2]->SetParent(m_pModel[0]);

	//���f��3
	m_pModel[3] = CModel::Create("data\\MODEL\\armL.x", D3DXVECTOR3(20.0f, 40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pModel[3]->SetParent(m_pModel[0]);

	//���f��4
	m_pModel[4] = CModel::Create("data\\MODEL\\legR.x", D3DXVECTOR3(-7.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pModel[4]->SetParent(m_pModel[0]);

	//���f��5
	m_pModel[5] = CModel::Create("data\\MODEL\\legL.x", D3DXVECTOR3(7.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pModel[5]->SetParent(m_pModel[0]);

	//���f��6
	m_pModel[6] = CModel::Create("data\\MODEL\\sword.x", D3DXVECTOR3(-22.0f, -14.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pModel[6]->SetParent(m_pModel[2]);
}

//========================
// ���[�V�����̐ݒ�
//========================
void CPlayer::SetMotion(bool bLoop)
{
	if (m_nCurrentKey + 1 >= MAX_KEY)
	{//�L�[���ő吔�ɒB������
		if (bLoop)
		{
			m_nCurrentKey = 0;	//�L�[�ԍ��̏�����
		}
		else
		{
			return;
		}
	}

	for (int i = 0; i < MAX_PARTS; i++)
	{//���f���p�[�c������
		//-----------------------------------------
		// NULL�`�F�b�N
		//-----------------------------------------
		if (!m_pModel[i])
		{//���f���p�[�c��null�Ȃ�
			return;
		}

		//�L�[�����������ϐ�
		KEY key = g_aKeySet[m_nCurrentKey].aKey[i];
		KEY keyNext = g_aKeySet[m_nCurrentKey + 1].aKey[i];

		//-----------------------------------------
		// ���ݒl���擾
		//-----------------------------------------
		//�ʒu���擾
		float fPosX = m_pModel[i]->GetPos().x;
		float fPosY = m_pModel[i]->GetPos().y;
		float fPosZ = m_pModel[i]->GetPos().z;

		//�������擾
		float fRotX = m_pModel[i]->GetRot().x;
		float fRotY = m_pModel[i]->GetRot().y;
		float fRotZ = m_pModel[i]->GetRot().z;

		//-----------------------------------------
		// �����̌v�Z
		// (�I���l - �J�n�l)
		//-----------------------------------------
		//�ʒu
		float fDifPosX = keyNext.fPosX - key.fPosX;
		float fDifPosY = keyNext.fPosY - key.fPosY;
		float fDifPosZ = keyNext.fPosZ - key.fPosZ;

		//����
		float fDifRotX = keyNext.fRotX - key.fRotX;
		float fDifRotY = keyNext.fRotY - key.fRotY;
		float fDifRotZ = keyNext.fRotZ - key.fRotZ;

		//-----------------------------------------
		// ���Βl�̌v�Z
		// (���[�V�����J�E���^�[ / �t���[����)
		//-----------------------------------------
		float fNumRelative = m_nCntMotion / (float)g_aKeySet[m_nCurrentKey].nFrame;

		//-----------------------------------------
		// ���ݒl�̌v�Z
		// (�J�n�l + (���� * ���Βl))
		//-----------------------------------------
		//�ʒu
		fPosX += key.fPosX + (fDifPosX * fNumRelative);
		fPosY += key.fPosY + (fDifPosY * fNumRelative);
		fPosZ += key.fPosZ + (fDifPosZ * fNumRelative);

		//����
		fRotX = key.fRotX + (fDifRotX * fNumRelative);
		fRotY = key.fRotY + (fDifRotY * fNumRelative);
		fRotZ = key.fRotZ + (fDifRotZ * fNumRelative);

		//-----------------------------------------
		// ���f�����̐ݒ�
		//-----------------------------------------
		//�ʒu�̐ݒ�
		m_pModel[i]->SetPos(D3DXVECTOR3(fPosX, fPosY, fPosZ));

		//�����̐ݒ�
		m_pModel[i]->SetRot(D3DXVECTOR3(D3DXToRadian(fRotX), D3DXToRadian(fRotY), D3DXToRadian(fRotZ)));
	}

	//���[�V�����J�E���^�[��i�߂�
	m_nCntMotion++;

	//-------------------------
	// ������
	//-------------------------
	if (m_nCntMotion >= g_aKeySet[m_nCurrentKey].nFrame)
	{//���[�V�����J�E���^�[���Đ��t���[���ɒB������
		m_nCurrentKey++;	//�L�[�ԍ������Z
		m_nCntMotion = 0;	//���[�V�����J�E���^�[��������
	}
}

//========================
// �ړ�
//========================
void CPlayer::Move()
{
	//�J�����̏��擾
	D3DXVECTOR3 cameraRot(CApplication::GetCamera()->GetRot());

	//-------------------------------
	// �v���C���[�̑���
	//-------------------------------
	if (CInputKeyboard::Press(DIK_A))
	{//A�L�[�������ꂽ
		if (CInputKeyboard::Press(DIK_W))
		{//W�L�[�������ꂽ
			m_move.x += sinf(cameraRot.y - D3DX_PI * 0.25f) * fPlayerSpeed;	//�����ړ�
			m_move.z += cosf(cameraRot.y - D3DX_PI * 0.25f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.75f;	//�����̐؂�ւ�
		}
		else if (CInputKeyboard::Press(DIK_S))
		{//S�L�[�������ꂽ
			m_move.x += sinf(cameraRot.y - D3DX_PI * 0.75f) * fPlayerSpeed;	//���O�ړ�
			m_move.z += cosf(cameraRot.y - D3DX_PI * 0.75f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.25f;
		}
		else
		{
			m_move.x -= sinf(cameraRot.y + D3DX_PI * 0.5f) * fPlayerSpeed;	//���ړ�
			m_move.z -= cosf(cameraRot.y + D3DX_PI * 0.5f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.5f;
		}
	}
	else if (CInputKeyboard::Press(DIK_D))
	{//D�L�[�������ꂽ
		if (CInputKeyboard::Press(DIK_W))
		{//W�L�[�������ꂽ
			m_move.x += sinf(cameraRot.y + D3DX_PI * 0.25f) * fPlayerSpeed;	//�E���ړ�
			m_move.z += cosf(cameraRot.y + D3DX_PI * 0.25f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.75f;
		}
		else if (CInputKeyboard::Press(DIK_S))
		{//S�L�[�������ꂽ
			m_move.x += sinf(cameraRot.y + D3DX_PI * 0.75f) * fPlayerSpeed;	//�E�O�ړ�
			m_move.z += cosf(cameraRot.y + D3DX_PI * 0.75f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.25f;
		}
		else
		{
			m_move.x += sinf(cameraRot.y + D3DX_PI * 0.5f) * fPlayerSpeed;	//�E�ړ�
			m_move.z += cosf(cameraRot.y + D3DX_PI * 0.5f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.5f;
		}
	}
	else if (CInputKeyboard::Press(DIK_W))
	{//W�L�[�������ꂽ
		m_move.x -= sinf(cameraRot.y + D3DX_PI * 1.0f) * fPlayerSpeed;	//���ړ�
		m_move.z -= cosf(cameraRot.y + D3DX_PI * 1.0f) * fPlayerSpeed;
		m_rotDest.y = cameraRot.y + D3DX_PI * 1.0f;
	}
	else if (CInputKeyboard::Press(DIK_S))
	{//S�L�[�������ꂽ
		m_move.x -= sinf(cameraRot.y + D3DX_PI * 0.0f) * fPlayerSpeed;	//�O�ړ�
		m_move.z -= cosf(cameraRot.y + D3DX_PI * 0.0f) * fPlayerSpeed;
		m_rotDest.y = cameraRot.y + D3DX_PI * 0.0f;
	}

	//�W�����v��Ԃł͂Ȃ��Ƃ���
	if (m_state == IDOL_STATE)
	{
		if (CInputKeyboard::Trigger(DIK_SPACE) && !m_bJump)
		{
			// �W�����v��ԂɈڍs
			m_state = JUMP_STATE;
		}
	}
	
	//�ړ����x����
	if (m_move.x >= MAX_SPEED)
	{
		m_move.x = MAX_SPEED;
	}
	else if (m_move.x <= -MAX_SPEED)
	{
		m_move.x = -MAX_SPEED;
	}
	else if (m_move.z >= MAX_SPEED)
	{
		m_move.z = MAX_SPEED;
	}
	else if (m_move.z <= -MAX_SPEED)
	{
		m_move.z = -MAX_SPEED;
	}

	//�d�͊֘A
	if (m_pos.y <= 1.0f)
	{
		m_bJump = false;
		m_move.y = 0.0f;
		
		//���Œn�ʂŎ~�܂鏈��
		m_pos.y = 1.0f;
	}
		//�d��
		m_move.y -= fGravity;

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

	//�ʒu���X�V
	m_pos += m_move;
	
	//�����̎���
	m_move.x += (0.0f - m_move.x) * 0.1f;
	m_move.z += (0.0f - m_move.z) * 0.1f;

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
// ���[���h�}�g���b�N�X�̎擾
//===========================
D3DXMATRIX CPlayer::GetmtxWorld()
{
	return m_mtxWorld;
}