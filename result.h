//===================================================
//
// ���U���g�w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _RESULT_H_
#define _RESULT_H_	

//-------------------------------
// �C���N���[�h
//--------------------------------
#include <d3dx9.h>

//================================
// �O���錾
//================================
class CObject2D;
class CCameraTitle;
class CScore;

//================================
// ���U���g�N���X�̒�`
//================================
class CResult
{
public:
	CResult();	//�R���X�g���N�^
	~CResult();	//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	HRESULT Init();
	void Uninit();
	void Update();
	
	//------------------
	// �Q�b�^�[
	//------------------
	static CCameraTitle* GetCameraTitle();	//�J�����̎擾

	//------------------
	// �����o�ϐ�
	//------------------
	CObject2D* m_pObject2D;
	static CCameraTitle* m_pCameraTitle;	//�^�C�g���J����
	CScore* apScore[5];
};

#endif