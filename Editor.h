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
class CGoal;
class CMeshField;

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
	void Init();
	void Update();
	void Load();
	void Uninit();
	static CEditor*Create();

private:
	//�����o�֐�
	OBJ					m_type;			//�I�u�W�F�N�g�̎��
	D3DXVECTOR3			m_pos;			//�o�����W
	D3DXVECTOR3			m_rot;			//�o�������ۂ̊p�x
	bool				bEnd;			//�I���t���O
	bool				bFlag;			//�����t���O
	int					nNumber;		//���ݐݒ肷��u���b�N�̃^�C�v

	//�X�^�e�B�b�N�֐�
	static	CPlayer*	pPlayer;
	static	CGoal*		m_pGoal;		//�S�[��
	static	CMeshField*	m_Map;			//�}�b�v


};
#endif 



#pragma once
