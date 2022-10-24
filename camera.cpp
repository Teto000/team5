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

//===========================
// �R���X�g���N�^
//===========================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDistance = 0.0f;
	m_TSPEED = 0.0f;
	m_MSPEED = 0.0f;
	POLOR_X = 0.0f;
	POLOR_Y = 0.0f;
	POLOR_Z = 0.0f;
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
	// ���_�E�����_�E�������ݒ肷��
	//---------------------------------
	m_posV = D3DXVECTOR3(0.0f, 200.0f, -400.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_TSPEED = 0.02f;		//�J�����̉�]���x(Turn)
	m_MSPEED = 3.0f;		//�J�����̈ړ����x(Move)

	//--------------------------
	// �r���[�|�[�g�\���̕ۑ�
	//--------------------------
	m_viewport.X = 0;			//�r���[�|�[�g�̊J�n�ʒu(X)
	m_viewport.Y = 0;			//�r���[�|�[�g�̊J�n�ʒu(Y)
	m_viewport.Width = 1280;	//�r���[�|�[�g�̕�
	m_viewport.Height = 720;	//�r���[�|�[�g�̍���
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
		m_rot.y -= m_TSPEED;			//��]�ʂ̑���
		m_posR.x = m_posV.x + POLOR_X;	//x�̋���
		m_posR.y = m_posV.y + POLOR_Y;	//y�̋���
		m_posR.z = m_posV.z + POLOR_Z;	//z�̋���
	}
	else if (CInputKeyboard::Press(DIK_E))	//�E��]
	{//E�L�[�������ꂽ
		m_rot.y += m_TSPEED;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	if (CInputKeyboard::Press(DIK_Y))	//���]
	{//Y�L�[�������ꂽ
		m_rot.x -= m_TSPEED;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	else if (CInputKeyboard::Press(DIK_B))	//����]
	{//B�L�[�������ꂽ
		m_rot.x += m_TSPEED;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}

	//------------------
	// ���_�̐���
	//------------------
	if (CInputKeyboard::Press(DIK_LEFT))	//����]
	{//���L�[�������ꂽ
		m_rot.y += m_TSPEED;			//��]�ʂ̑���
		m_posV.x = m_posR.x - POLOR_X;	//x�̋���
		m_posV.y = m_posR.y - POLOR_Y;	//y�̋���
		m_posV.z = m_posR.z - POLOR_Z;	//z�̋���
	}
	else if (CInputKeyboard::Press(DIK_RIGHT))	//�E��]
	{//�E�L�[�������ꂽ
		m_rot.y -= m_TSPEED;
		m_posV.x = m_posR.x - POLOR_X;
		m_posV.y = m_posR.y - POLOR_Y;
		m_posV.z = m_posR.z - POLOR_Z;
	}
	if (CInputKeyboard::Press(DIK_UP))	//���]
	{//��L�[�������ꂽ
		if (m_rot.x <= 3.0f)
		{
			m_rot.x += m_TSPEED;
			m_posV.x = m_posR.x - POLOR_X;
			m_posV.y = m_posR.y - POLOR_Y;
			m_posV.z = m_posR.z - POLOR_Z;
		}
	}
	else if (CInputKeyboard::Press(DIK_DOWN))	//����]
	{//���L�[�������ꂽ
		if (m_rot.x >= 1.7f)
		{
			m_rot.x -= m_TSPEED;
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
			m_posV.x += sinf(m_rot.y - D3DX_PI * 0.25f) * m_MSPEED;	//�����ړ�
			m_posV.z += cosf(m_rot.y - D3DX_PI * 0.25f) * m_MSPEED;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else if (CInputKeyboard::Press(DIK_S))
		{//S�L�[�������ꂽ
			m_posV.x += sinf(m_rot.y - D3DX_PI * 0.75f) * m_MSPEED;	//���O�ړ�
			m_posV.z += cosf(m_rot.y - D3DX_PI * 0.75f) * m_MSPEED;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else
		{
			m_posV.x -= sinf(m_rot.y + D3DX_PI * 0.5f) * m_MSPEED;	//���ړ�
			m_posV.z -= cosf(m_rot.y + D3DX_PI * 0.5f) * m_MSPEED;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
	}
	else if (CInputKeyboard::Press(DIK_D))
	{//D�L�[�������ꂽ
		if (CInputKeyboard::Press(DIK_W))
		{//W�L�[�������ꂽ
			m_posV.x += sinf(m_rot.y + D3DX_PI * 0.25f) * m_MSPEED;	//�E���ړ�
			m_posV.z += cosf(m_rot.y + D3DX_PI * 0.25f) * m_MSPEED;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else if (CInputKeyboard::Press(DIK_S))
		{//S�L�[�������ꂽ
			m_posV.x += sinf(m_rot.y + D3DX_PI * 0.75f) * m_MSPEED;	//�E�O�ړ�
			m_posV.z += cosf(m_rot.y + D3DX_PI * 0.75f) * m_MSPEED;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
		else
		{
			m_posV.x += sinf(m_rot.y + D3DX_PI * 0.5f) * m_MSPEED;	//�E�ړ�
			m_posV.z += cosf(m_rot.y + D3DX_PI * 0.5f) * m_MSPEED;
			m_posR.x = m_posV.x + POLOR_X;
			m_posR.y = m_posV.y + POLOR_Y;
			m_posR.z = m_posV.z + POLOR_Z;
		}
	}
	else if (CInputKeyboard::Press(DIK_W))
	{//W�L�[�������ꂽ
		m_posV.x -= sinf(m_rot.y + D3DX_PI * 1.0f) * m_MSPEED;	//���ړ�
		m_posV.z -= cosf(m_rot.y + D3DX_PI * 1.0f) * m_MSPEED;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	else if (CInputKeyboard::Press(DIK_S))
	{//S�L�[�������ꂽ
		m_posV.x -= sinf(m_rot.y + D3DX_PI * 0.0f) * m_MSPEED;	//�O�ړ�
		m_posV.z -= cosf(m_rot.y + D3DX_PI * 0.0f) * m_MSPEED;
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	if (CInputKeyboard::Press(DIK_O))	//�㏸
	{//O�L�[�������ꂽ
		m_posV.y -= tanf(m_rot.x + D3DX_PI * 1.0f) * m_MSPEED;	//��ړ�
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
	else if (CInputKeyboard::Press(DIK_L))	//���~
	{//L�L�[�������ꂽ
		m_posV.y += tanf(m_rot.x + D3DX_PI * 0.0f) * m_MSPEED;	//��ړ�
		m_posR.x = m_posV.x + POLOR_X;
		m_posR.y = m_posV.y + POLOR_Y;
		m_posR.z = m_posV.z + POLOR_Z;
	}
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