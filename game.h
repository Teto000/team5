//===================================================
//
// �Q�[���w�b�_�[
// Author : Sato Teruto
//
//====================================================
#ifndef _GAME_H_
#define _GAME_H_	

//--------------------------------
// �C���N���[�h
//--------------------------------
#include <d3dx9.h>
#include "application.h"

//--------------------------------
// �O���錾
//--------------------------------
class CPolygon;		//�|���S��
class CPolygon2d;	//�|���S��2d
class CPlayer;		//�v���C���[
class CMeshField;	//���b�V���t�B�[���h

//================================
// �Q�[���N���X�̒�`
//================================
class CGame
{
public:
	CGame();	//�R���X�g���N�^
	~CGame();	//�f�X�g���N�^

	//------------------
	// �����o�֐�
	//------------------
	HRESULT Init();
	void Uninit();
	void Update();

	//------------------
	// �Q�b�^�[
	//------------------
	static CPlayer*	GetPlayer();	//�v���C���[�̎擾

private:
	//-------------------
	// �ÓI�����o�ϐ�
	//-------------------
	static CPolygon* pPolygon;
	static CPolygon2d* pPolygon2d;
	static CPlayer*	 pPlayer;
	static CMeshField* pMeshField;
};

#endif