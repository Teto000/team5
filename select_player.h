//===================================================
//
// �v���C���[�̐l���̃Z���N�g.h
// Author : Tomoki Tomidokoro
//
//===================================================
#ifndef _SELECT_PLAYER_H_
#define _SELECT_PLAYER_H_	

#define MAX_PLAYER (4)
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
// P�Z���N�g�N���X�̒�`
//================================
class CPSelect
{
public:
	CPSelect();	//�R���X�g���N�^
	~CPSelect();	//�f�X�g���N�^

//------------------
// �����o�֐�
//------------------
	HRESULT Init();
	void Uninit();
	void Update();
	void SelectPlayerNum();
//------------------
// �Q�b�^�[
//------------------

private:
//-------------------
// �ÓI�����o�ϐ�
//-------------------
	static CPolygon* pPolygon;
	static CPolygon2d* pPolygon2d;
	int m_PlayerSelectNum;
};

#endif