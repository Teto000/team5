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
class CObject2D;	//�|���S��2d
class CPlayer;		//�v���C���[
class CMeshField;	//���b�V���t�B�[���h
class CUI_PSelect;	//UI

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
	void UI_Create();
	void UI_Uninit();
	//------------------
	// �Q�b�^�[
	//------------------

private:
	//-------------------
	// �����o�ϐ�
	//-------------------
	int m_PlayerSelectNum;
	CUI_PSelect *m_PUI[MAX_PLAYER];			// �v���C���[����UI
	CUI_PSelect *m_BGUI;					// �w�i��UI
};

#endif