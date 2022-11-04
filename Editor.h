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
#define FILE_MAPDATA	"data\\TXT\\Edit.txt"	//�ǂݍ��ރt�@�C����


//�O���錾
class CEnemyManager;
class CPlayer;

//=============================================================================
// �\���̒�`
//=============================================================================
class CEditor
{
public:
	enum OBJ
	{
		OBJ_GOAL = 0,
		OBJ_MAP,
		OBJ_GIMMICK,
		OBJ_MAX
	};

	CEditor();
	~CEditor();
	void Update();
	void Load();

private:
	//�����o�֐�
	OBJ			m_type;			//�I�u�W�F�N�g�̎��
	D3DXVECTOR3 m_pos;			//�o�����W
	D3DXVECTOR3 m_rot;			//�o�������ۂ̊p�x
	int			m_frame;		//�o������
	bool		bEnd;			//�I���t���O
	int			nNumplayer;		//���݂̐l��
	bool		bFlag;			//�����t���O
	static		 CPlayer*		pPlayer;


};
#endif 



#pragma once
