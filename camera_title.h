//===================================================
//
// �^�C�g���J�����w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _CAMERA_TITLE_H_	//���̃}�N������`����ĂȂ�������
#define _CAMERA_TITLE_H_	//2�d�C���N���[�h�h�~�̃}�N����`

//--------------------------------
// �C���N���[�h
//--------------------------------
#include <d3dx9.h>
#include "camera.h"

//================================
// �^�C�g���J�����N���X�̒�`
//================================
class CCameraTitle : public CCamera
{
public:
	CCameraTitle();				//�R���X�g���N�^
	~CCameraTitle() override;	//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void SetCamera(LPDIRECT3DDEVICE9 pDevice) override;

	//------------------
	// �ÓI�����o�֐�
	//------------------
	static CCameraTitle* Create(DWORD X, DWORD Y, DWORD Width, DWORD Height);
};

#endif