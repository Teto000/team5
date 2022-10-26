//===================================================
//
// �J�����̏���
// Author:Teruto Sato
//
//===================================================

//----------------------
// �C���N���[�h
//----------------------
#include "camera.h"
#include "input.h"
#include "input_keybord.h"
#include "renderer.h"
#include "game.h"

//----------------------
// �ÓI�����o�ϐ��錾
//----------------------
const float CCamera::fTurnSpeed = 0.02f;	//���񑬓x
const float CCamera::fMoveSpeed = 3.0f;		//�ړ����x

//===========================
// �R���X�g���N�^
//===========================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���_
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�����_
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړI�̎��_
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړI�̒����_
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;	//�ړI�̊p�x
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//������x�N�g��
	m_fDistance = 0.0f;		//����
	POLOR_X = 0.0f;			//�ɍ��W��X
	POLOR_Y = 0.0f;			//�ɍ��W��Y
	POLOR_Z = 0.0f;			//�ɍ��W��Z
}

//===========================
// �f�X�g���N�^
//===========================
CCamera::~CCamera()
{

}

//========================
// �J�����̏���������
//========================
void CCamera::Init(void)
{
	//---------------------------------
	// �����l�̐ݒ�
	//---------------------------------
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//�����
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;

	{
		//----------------------------
		// �����̐ݒ�(�O�����̒藝)
		//----------------------------
		float X = m_posR.x - m_posV.x;
		float Y = m_posR.y - m_posV.y;
		float Z = m_posR.z - m_posV.z;
		m_fDistance = sqrtf((X * X) + (Y * Y) + (Z * Z));

		//----------------------------
		// �p�x�̐ݒ�
		//----------------------------
		float angle = -atan2f(Y, Z) + D3DX_PI * 0.5f;

		//----------------------------
		// �����̏�����
		//----------------------------
		m_rot = D3DXVECTOR3(angle, 0.0f, 0.0f);
		m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//========================
// �J�����̏I������
//========================
void CCamera::Uninit(void)
{

}

//========================
// �J�����̍X�V����
//========================
void CCamera::Update(void)
{
	//------------------
	// �ɍ��W��XYZ
	//------------------
	POLOR_X = sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	POLOR_Y = cosf(m_rot.x) * m_fDistance;
	POLOR_Z = sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	//------------------
	// �J�����̐���
	//------------------
	Turn();

	//------------------
	// �J�����̈ړ�
	//------------------
	Move();

	//------------------
	// �p�x�̐��K��
	//------------------
	//���E
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
}

//========================
// �J�����̐ݒ菈��
//========================
void CCamera::SetCamera(LPDIRECT3DDEVICE9 pDevice)
{
	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
					   &m_posV,
					   &m_posR,
					   &m_vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
							   D3DXToRadian(45.0f),
							   (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
							   10.0f,
							   1000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//========================================
// �J�����̐���
// (�J�n�ʒu X, �J�n�ʒu Y, ��, ����)
//========================================
CCamera* CCamera::Create(DWORD X, DWORD Y, DWORD Width, DWORD Height)
{
	CCamera *pCamera = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pCamera = new CCamera;	//����

	if (pCamera != nullptr)
	{//NULL�`�F�b�N
		//�r���[�|�[�g�\���̕ۑ�
		pCamera->m_viewport.X = X;				//�J�n�ʒu(X)
		pCamera->m_viewport.Y = Y;				//�J�n�ʒu(Y)
		pCamera->m_viewport.Width = Width;		//��
		pCamera->m_viewport.Height = Height;	//����

		//������
		pCamera->Init();
	}

	return pCamera;
}

//========================
// �J�����̐���
//========================
void CCamera::Turn()
{
	//------------------
	// �����_�̐���
	//------------------
	if (CInputKeyboard::Press(DIK_Q))	//����]
	{//Q�L�[�������ꂽ
		m_rot.y -= fTurnSpeed;			//��]�ʂ̑���
		m_posR.x = m_posV.x + POLOR_X;	//x�̋���
		m_posR.y = m_posV.y + POLOR_Y;	//y�̋���
		m_posR.z = m_posV.z + POLOR_Z;	//z�̋���
	}
	else if (CInputKeyboard::Press(DIK_E))	//�E��]
	{//E�L�[�������ꂽ
		m_rot.y += fTurnSpeed;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	if (CInputKeyboard::Press(DIK_Y))	//���]
	{//Y�L�[�������ꂽ
		m_rot.x -= fTurnSpeed;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	else if (CInputKeyboard::Press(DIK_B))	//����]
	{//B�L�[�������ꂽ
		m_rot.x += fTurnSpeed;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}

	//------------------
	// ���_�̐���
	//------------------
	if (CInputKeyboard::Press(DIK_LEFT))	//����]
	{//���L�[�������ꂽ
		m_rot.y += fTurnSpeed;			//��]�ʂ̑���
		m_posV.x = m_posR.x - POLOR_X;	//x�̋���
		m_posV.y = m_posR.y - POLOR_Y;	//y�̋���
		m_posV.z = m_posR.z - POLOR_Z;	//z�̋���
	}
	else if (CInputKeyboard::Press(DIK_RIGHT))	//�E��]
	{//�E�L�[�������ꂽ
		m_rot.y -= fTurnSpeed;
		m_posV.x = m_posR.x - POLOR_X;
		m_posV.y = m_posR.y - POLOR_Y;
		m_posV.z = m_posR.z - POLOR_Z;
	}
	if (CInputKeyboard::Press(DIK_UP))	//���]
	{//��L�[�������ꂽ
		if (m_rot.x <= 3.0f)
		{
			m_rot.x += fTurnSpeed;
			m_posV.x = m_posR.x - POLOR_X;
			m_posV.y = m_posR.y - POLOR_Y;
			m_posV.z = m_posR.z - POLOR_Z;
		}
	}
	else if (CInputKeyboard::Press(DIK_DOWN))	//����]
	{//���L�[�������ꂽ
		if (m_rot.x >= 1.7f)
		{
			m_rot.x -= fTurnSpeed;
			m_posV.x = m_posR.x - POLOR_X;
			m_posV.y = m_posR.y - POLOR_Y;
			m_posV.z = m_posR.z - POLOR_Z;
		}
	}
}

//========================
// �J�����̈ړ�
//========================
void CCamera::Move()
{
	if (CInputKeyboard::Press(DIK_A))
	{//A�L�[�������ꂽ
		if (CInputKeyboard::Press(DIK_W))
		{//W�L�[�������ꂽ
			m_posV.x += sinf(m_rot.y - D3DX_PI * 0.25f) * fMoveSpeed;	//�����ړ�
			m_posV.z += cosf(m_rot.y - D3DX_PI * 0.25f) * fMoveSpeed;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else if (CInputKeyboard::Press(DIK_S))
		{//S�L�[�������ꂽ
			m_posV.x += sinf(m_rot.y - D3DX_PI * 0.75f) * fMoveSpeed;	//���O�ړ�
			m_posV.z += cosf(m_rot.y - D3DX_PI * 0.75f) * fMoveSpeed;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else
		{
			m_posV.x -= sinf(m_rot.y + D3DX_PI * 0.5f) * fMoveSpeed;	//���ړ�
			m_posV.z -= cosf(m_rot.y + D3DX_PI * 0.5f) * fMoveSpeed;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
	}
	else if (CInputKeyboard::Press(DIK_D))
	{//D�L�[�������ꂽ
		if (CInputKeyboard::Press(DIK_W))
		{//W�L�[�������ꂽ
			m_posV.x += sinf(m_rot.y + D3DX_PI * 0.25f) * fMoveSpeed;	//�E���ړ�
			m_posV.z += cosf(m_rot.y + D3DX_PI * 0.25f) * fMoveSpeed;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else if (CInputKeyboard::Press(DIK_S))
		{//S�L�[�������ꂽ
			m_posV.x += sinf(m_rot.y + D3DX_PI * 0.75f) * fMoveSpeed;	//�E�O�ړ�
			m_posV.z += cosf(m_rot.y + D3DX_PI * 0.75f) * fMoveSpeed;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else
		{
			m_posV.x += sinf(m_rot.y + D3DX_PI * 0.5f) * fMoveSpeed;	//�E�ړ�
			m_posV.z += cosf(m_rot.y + D3DX_PI * 0.5f) * fMoveSpeed;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
	}
	else if (CInputKeyboard::Press(DIK_W))
	{//W�L�[�������ꂽ
		m_posV.x -= sinf(m_rot.y + D3DX_PI * 1.0f) * fMoveSpeed;	//���ړ�
		m_posV.z -= cosf(m_rot.y + D3DX_PI * 1.0f) * fMoveSpeed;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	else if (CInputKeyboard::Press(DIK_S))
	{//S�L�[�������ꂽ
		m_posV.x -= sinf(m_rot.y + D3DX_PI * 0.0f) * fMoveSpeed;	//�O�ړ�
		m_posV.z -= cosf(m_rot.y + D3DX_PI * 0.0f) * fMoveSpeed;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	if (CInputKeyboard::Press(DIK_O))	//�㏸
	{//O�L�[�������ꂽ
		m_posV.y -= tanf(m_rot.x + D3DX_PI * 1.0f) * fMoveSpeed;	//��ړ�
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	else if (CInputKeyboard::Press(DIK_L))	//���~
	{//L�L�[�������ꂽ
		m_posV.y += tanf(m_rot.x + D3DX_PI * 0.0f) * fMoveSpeed;	//��ړ�
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
}

//========================
// ���_�E�����_�̐ݒ�
//========================
void CCamera::SetPos(D3DXVECTOR3 pos)
{
	m_posR = pos;	//�����_
	m_posV = m_posR + D3DXVECTOR3(0.0f, 200.0f, -400.0f);	//���_
}

//========================
// ���_�̎擾
//========================
D3DXVECTOR3 CCamera::GetPosV()
{
	return m_posV;
}

//========================
// �p�x�̎擾
//========================
D3DXVECTOR3 CCamera::GetRot()
{
	return m_rot;
}

//========================
// �r���[�|�[�g�̎擾
//========================
D3DVIEWPORT9 CCamera::GetVieport()
{
	return m_viewport;
}