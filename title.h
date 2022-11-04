//===================================================
//
// �^�C�g���w�b�_�[
// Author : Sato Teruto
//
//===================================================
#ifndef _TITLE_H_
#define _TITLE_H_	

//--------------------------------
// �C���N���[�h
//--------------------------------
#include <d3dx9.h>

//--------------------------------
// �O���錾
//--------------------------------
class CObject2D;
class CMeshField;
class CCameraTitle;
class CPlayer;
class CTitlelogo;

//================================
// �^�C�g���N���X�̒�`
//================================
class CTitle
{
public:
	CTitle();	//�R���X�g���N�^
	~CTitle();	//�f�X�g���N�^

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

private:
	//------------------
	// �����o�ϐ�
	//------------------
	CObject2D*		m_pObject2D;

	//------------------
	// �ÓI�����o�ϐ�
	//------------------
	static CMeshField*	 m_pMeshField;		//���b�V���t�B�[���h
	static CCameraTitle* m_pCameraTitle;	//�^�C�g���J����
	static CTitlelogo*	 m_pTitlelogo;

};

#endif