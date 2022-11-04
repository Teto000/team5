//==================================================
//
// �J�����̏���
// Author:Teruto Sato
//
//==================================================

//----------------------
// �C���N���[�h
//----------------------
#include <stdlib.h>
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

//==================================================
// �R���X�g���N�^
//==================================================
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

//==================================================
// �f�X�g���N�^
//==================================================
CCamera::~CCamera()
{

}

//==================================================
// �J�����̏���������
//==================================================
HRESULT CCamera::Init(void)
{
	//---------------------------------
	// �����l�̐ݒ�
	//---------------------------------
	m_posV = D3DXVECTOR3(0.0f, 200.0f, -400.0f);	//���_
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

	return S_OK;
}

//==================================================
// �J�����̏I������
//==================================================
void CCamera::Uninit(void)
{

}

//==================================================
// �J�����̍X�V����
//==================================================
void CCamera::Update(void)
{
	//------------------------
	// �ɍ��W��XYZ
	//------------------------
	POLOR_X = sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	POLOR_Y = cosf(m_rot.x) * m_fDistance;
	POLOR_Z = sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	//------------------------
	// �p�x�̐��K��
	//------------------------
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

//==================================================
// �J�����̐ݒ菈��
// ���� : �f�o�C�X
//==================================================
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

//==================================================
// ���_�E�����_�̐ݒ�
// ���� : �ʒu
//==================================================
void CCamera::SetPos(D3DXVECTOR3 pos)
{
	m_posR = pos;	//�����_
	m_posV = m_posR + D3DXVECTOR3(0.0f, 200.0f, -400.0f);	//���_
}

//==================================================
// �r���[�|�[�g�̑傫���ݒ�
// ���� : ��ʍ���̍��WX,Y�A���A����
//==================================================
void CCamera::SetViewSize(DWORD X, DWORD Y, int fWidth, int fHeight)
{
	//��������
	m_viewport.X = X;
	m_viewport.Y = Y;
	m_viewport.Width = fWidth;
	m_viewport.Height = fHeight;
}

//==================================================
// �r���[�|�[�g�̊g�k
// ���� : �J�n�ʒuX�A�J�n�ʒuY�A���A����
//==================================================
void CCamera::AddViewSize(DWORD X, DWORD Y, int fWidth, int fHeight)
{
	//-------------------
	// ���̉��Z
	//-------------------
	if (m_viewport.Width < SCREEN_WIDTH)
	{//�����X�N���[�����Ȃ�
		m_viewport.Width += fWidth;

		if (m_viewport.X > 0)
		{
			m_viewport.X += X;
		}
	}

	//-------------------
	// �����̉��Z
	//-------------------
	if (m_viewport.Height < SCREEN_HEIGHT)
	{//�����X�N���[�����Ȃ�
		m_viewport.Height += fHeight;

		if (m_viewport.Y > 0)
		{
			m_viewport.Y += Y;
		}
	}
}

//==================================================
// �A�X�y�N�g��̐ݒ�
// ���� : �f�o�C�X�A����p�A�A�X�y�N�g��(���A����)
//==================================================
void CCamera::SetAspect(LPDIRECT3DDEVICE9 pDevice, float fov, float fWidth, float fHeight)
{
	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(fov),
								fWidth / fHeight,
								10.0f,
								1000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//==================================================
// ���_�̎擾
//==================================================
D3DXVECTOR3 CCamera::GetPosV()
{
	return m_posV;
}

//==================================================
// �p�x�̎擾
//==================================================
D3DXVECTOR3 CCamera::GetRot()
{
	return m_rot;
}

//==================================================
// �r���[�|�[�g�̎擾
//==================================================
D3DVIEWPORT9 CCamera::GetViewport()
{
	return m_viewport;
}