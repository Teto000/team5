//===================================================
//
// �J�����w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _CAMERA_H_	//���̃}�N������`����ĂȂ�������
#define _CAMERA_H_	//2�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------
// �C���N���[�h
//--------------------------------
#include <d3dx9.h>
#include "main.h"

//================================
// �J�����N���X�̒�`
//================================
class CCamera
{
public:
	CCamera();	//�R���X�g���N�^
	~CCamera();	//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	void Init();
	void Uninit();
	void Update();
	void SetCamera(LPDIRECT3DDEVICE9 pDevice);

	//------------------
	// �Q�b�^�[
	//------------------
	D3DXVECTOR3 GetPosV();
	D3DXVECTOR3 GetRot();

private:
	void Turn();
	void Move();

private:
	//------------------
	// �����o�ϐ�
	//------------------
	D3DXVECTOR3 m_posV;		//���݂̎��_
	D3DXVECTOR3 m_posR;		//���݂̒����_
	D3DXVECTOR3 m_rot;		//����
	D3DXVECTOR3 m_posVDest;	//�ړI�̎��_
	D3DXVECTOR3 m_posRDest;	//�ړI�̒����_
	D3DXVECTOR3 m_rotDest;	//�ړI�̊p�x
	D3DXVECTOR3 m_vecU;		//������x�N�g��
	float m_fDistance;		//����
	float m_TSPEED;
	float m_MSPEED;
	float POLOR_X;
	float POLOR_Y;
	float POLOR_Z;
	D3DXMATRIX m_mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;		//�r���[�}�g���b�N�X
	D3DVIEWPORT9 m_viewport;
};

#endif