//=============================================================================
//
// �v���C���[
// Author : �L�c����
//
//=============================================================================

#ifndef _EDITOR_H_
#define _EDITOR_H_

//�C���N���[�h
#include"Application.h"

//�}�N����`
#define NUM_STAGE	(1)									//�X�e�[�W��
#define FILE_MAPDATA	"data\\TXT\\stage01.txt"	//�ǂݍ��ރt�@�C����
#define DEBUG_MAPDATA	"data\\TXT\\test.txt"		//�ǂݍ��ރt�@�C����


//�O���錾
class CEnemyManager;
class CPlayer;

//=============================================================================
// �\���̒�`
//=============================================================================
class CMapdata
{
public:
	enum OBJ
	{
		OBJ_PLAYER = 0,
	};
	CMapdata();
	~CMapdata();
	void Update();
	void Load();

private:
	//�����o�֐�
	OBJ			m_type;			//�I�u�W�F�N�g�̎��
	D3DXVECTOR3 m_pos;			//�o�����W
	int			m_frame;		//�o������
	int			m_ptn;			//�o���p�^�[��
	bool		bEnd;			//�I���t���O
	int			nNumplayer;		//���݂̐l��
	bool		bFlag;			//�����t���O
	static		 CPlayer*		pPlayer;


};
#endif 



#pragma once
