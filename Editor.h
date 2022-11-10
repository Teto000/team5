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
#define NUM_STAGE	(1)							//�X�e�[�W��
#define FILE_MAPDATA	"data\\TXT\\Edit.txt"	//�ǂݍ��ރt�@�C����
#define	MAX_PLANET	(10)						//�z�u����f���̐�
#define	MAX_GIMMICK	(4)							//�M�~�b�N�̎��
#define MAX_STACK_BLOCK (100)					//�d�Ȃ��Ă���u���b�N�̐ݒu��



//�O���錾
class CEnemyManager;
class CPlayer;
class CGoal;
class CMeshField;
class CObject;
class CGimmick;
class CMap;
class CObjectX;
class CGimmick;

class CSBlock;
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
		OBJ_PLANET,
		OBJ_STACK_BLOCK,
		OBJ_MAX
	};

	CEditor();
	~CEditor();
	void Init();
	void Update();
	void Load();
	void Uninit();
	static CEditor*Create();
	void SaveObject();
	void Pass();
	void Input();

	static CGoal* GetGoal();	//�S�[���̎擾

private:
	//�����o�֐�
	OBJ					m_type;							//�I�u�W�F�N�g�̎��
	D3DXVECTOR3			m_pos;							//�o�����W
	D3DXVECTOR3			m_rot;							//�o�������ۂ̊p�x
	bool				m_bEnd;							//�I���t���O
	bool				m_bFlag;						//�����t���O
	int					m_nNumber;						//���ݐݒ肷��u���b�N�̃^�C�v
	int					m_nNumpla;						//���݂̘f���̔ԍ�
	int					m_nNumgim;						//���݂̃M�~�b�N�̔ԍ�

	LPCTSTR				m_nPlaFileName[MAX_PLANET];		//�f���̃t�@�C���̖��O


	//�X�^�e�B�b�N�֐�
	CPlayer*	m_pPlayer;
	static CGoal*		m_pGoal;			//�S�[��
	CMap*		m_pMap;						//�}�b�v
	CObject*	m_pSelectObj;				//�I�𒆂̃I�u�W�F�N�g
	CObjectX*	m_pPlanet[MAX_PLANET];		//�w�i�̐�
	static CSBlock*	m_pStackBlock[MAX_STACK_BLOCK];	//�d�Ȃ��Ă���u���b�N�̏���
};
#endif



#pragma once
