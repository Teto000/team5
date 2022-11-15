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
#define FILE_MAPDATA	"data\\TXT\\Edit.txt"	//�ǂݍ��ރt�@�C����
#define	MAX_PLANET		(11)					//�z�u����f���̐�
#define	MAX_GIMMICK		(4)						//�M�~�b�N�̎��
#define MAX_STACK_BLOCK (100)					//�d�Ȃ��Ă���u���b�N�̐ݒu��
#define MAX_SETGIMMICK	(128)					//������M�~�b�N�̍ő吔

//=============================================================================
//�O���錾
//=============================================================================
class CGoal;
class CObject;
class CGimmick;
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
	int					m_nNumObj;						//�I�u�W�F�N�g�̍ő吔
	int					m_gim;							//�M�~�b�N�̐�

	LPCTSTR				m_nPlaFileName[MAX_PLANET];		//�f���̃t�@�C���̖��O


	//�X�^�e�B�b�N�֐�
	static CGoal*	m_pGoal;							//�S�[��
	CObjectX*		m_pPlanet[MAX_PLANET];				//�w�i�̐�
	CGimmick*		m_pGimmick[MAX_SETGIMMICK];			//�w�i�̐�

	static CSBlock*	m_pStackBlock[MAX_STACK_BLOCK];		//�d�Ȃ��Ă���u���b�N�̏���
};

#endif