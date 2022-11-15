//===================================================
//
// �J�����w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _CAMERA_H_	//���̃}�N������`����ĂȂ�������
#define _CAMERA_H_	//2�d�C���N���[�h�h�~�̃}�N����`

//------------------------------
// �C���N���[�h
//------------------------------
#include <d3dx9.h>
#include "main.h"

//------------------------------
// �O���錾
//------------------------------
class CCameraPlayer;	//�v���C���[�N���X

//==================================================
// �J�����N���X�̒�`
//==================================================
class CCamera
{
public:
	CCamera();			//�R���X�g���N�^
	virtual ~CCamera();	//�f�X�g���N�^

	//------------------------------
	// �����o�֐�
	//------------------------------
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void SetCamera(LPDIRECT3DDEVICE9 pDevice);

	//------------------------------
	// �Z�b�^�[
	//------------------------------
	void SetPos(D3DXVECTOR3 pos);									//���_�E�����_�̐ݒ�
	void SetViewSize(DWORD X, DWORD Y, int fWidth, int fHeight);	//�r���[�|�[�g�̑傫���ݒ�
	void AddViewSize(DWORD X, DWORD Y, int fWidth, int fHeight);	//�r���[�|�[�g�̊g�k
	void SetAspect(LPDIRECT3DDEVICE9 pDevice, float fov,
					float fWidth, float fHeight);					//�A�X�y�N�g��̐ݒ�

	//------------------------------
	// �Q�b�^�[
	//------------------------------
	D3DXVECTOR3 GetPosV();		//���_�̎擾
	D3DXVECTOR3 GetRot();		//�����̎擾
	D3DVIEWPORT9 GetViewport();	//�r���[�|�[�g�̎擾

protected:
	//------------------------------
	// �萔
	//------------------------------
	static const float fTurnSpeed;	//���񑬓x
	static const float fMoveSpeed;	//�ړ����x

	//------------------------------
	// �����o�ϐ�
	//------------------------------
	D3DXVECTOR3 m_posV;			//���݂̎��_
	D3DXVECTOR3 m_posR;			//���݂̒����_
	D3DXVECTOR3 m_rot;			//����
	D3DXVECTOR3 m_posVDest;		//�ړI�̎��_
	D3DXVECTOR3 m_posRDest;		//�ړI�̒����_
	D3DXVECTOR3 m_rotDest;		//�ړI�̊p�x
	D3DXVECTOR3 m_vecU;			//������x�N�g��
	float m_fDistance;			//����
	float POLOR_X;				//�ɍ��W��X
	float POLOR_Y;				//�ɍ��W��Y
	float POLOR_Z;				//�ɍ��W��Z
	D3DXMATRIX m_mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;		//�r���[�}�g���b�N�X
	D3DVIEWPORT9 m_viewport;	//�r���[�|�[�g
};

#endif