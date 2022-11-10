//=============================================================================
//
// �}�b�v�f�[�^�ǂݍ���
// Author : Arita Meigen
//
//=============================================================================

//�C���N���[�h
#include"Editor.h"
#include<string>
#include"player.h"
#include"game.h"
#include"renderer.h"
#include"Goal.h"
#include"meshfield.h"
#include"debug_proc.h"
#include"input_keybord.h"
#include"input.h"
#include"Map.h"
#include"objectX.h"
#include"Gimmick.h"


//�ÓI�����o�ϐ�
//CPlayer*	CEditor::pPlayer = nullptr;
CGoal*		CEditor::m_pGoal = nullptr;						//�S�[��
//CMap*		CEditor::m_pMap = nullptr;						//�}�b�v
//CObject*	CEditor::m_pSelectObj = nullptr;				//�I�𒆂̃I�u�W�F�N�g
//CModel*		CEditor::m_pPlanet[MAX_PLANET] = { nullptr };	//�ݒu����I�u�W�F�N�g
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEditor::CEditor()
{
	m_pPlayer = nullptr;
	//m_pGoal = nullptr;						//�S�[��
	m_pMap = nullptr;						//�}�b�v
	m_pSelectObj = nullptr;					//�I�𒆂̃I�u�W�F�N�g
	ZeroMemory(&m_pPlanet[0],sizeof(m_pPlanet[0])*MAX_PLANET);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEditor::~CEditor()
{
}


//=============================================================================
//������
//=============================================================================
void CEditor::Init()
{
	//������
	m_pGoal = nullptr;
	m_pMap = nullptr;
	m_pPlayer = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�o�����W
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�o�������ۂ̊p�x
	m_bEnd = false;									//�I���t���O
	m_bFlag = false;								//�����t���O
	m_nNumber = 0;									//���ݐݒ肷��u���b�N�̃^�C�v
	m_nNumpla = 0;

	Pass();
}

//=============================================================================
//�I��
//=============================================================================
void CEditor::Uninit()
{
	if (m_pGoal != nullptr)
	{
		m_pGoal = nullptr;
	}

	if (m_pMap != nullptr)
	{
		m_pMap = nullptr;
	}

	if (m_pPlayer != nullptr)
	{
		m_pPlayer = nullptr;
	}

	for (int i = 0; i < MAX_PLANET; i++)
	{
		if (m_pPlanet[i] != nullptr)
		{
			m_pPlanet[i] = nullptr;
		}
	}

	m_nNumber = NULL;
	m_bEnd = NULL;			//�I���t���O
	m_bFlag = NULL;			//�����t���O
}

//=============================================================================
//�X�V
//=============================================================================
void CEditor::Update()
{
	Input();

	switch (m_nNumber)
	{
	case OBJ_GOAL:
		m_pSelectObj = m_pGoal;
		break;

	case OBJ_MAP:
		//m_pSelectObj = m_pMap;
		break;

	case OBJ_GIMMICK:
		//m_pSelectObj = m_pMap;
		break;

	case OBJ_PLANET:
		m_pSelectObj = m_pPlanet[m_nNumpla];
		break;

	default:
		break;

	}

//	D3DXVECTOR3 Selectpos = m_pSelectObj->GetPosition();

	CDebugProc::Print("���ݒu���I�u�W�F�N�g:%d 0(�[��)�L�[�Ŏ�ނ�ύX", m_nNumber);
	CDebugProc::Print("O(�I�[)�L�[�ŃI�u�W�F�N�g�̍��W�t�@�C���ɏo�͂���");
	CDebugProc::Print("P�L�[�ŃI�u�W�F�N�g�̐���/�ړ�");
	if (m_nNumber == OBJ_PLANET)
	{
		CDebugProc::Print("���ݐݒu����f���̎��:%d 8/9�Ŏ�ނ�ύX", m_nNumpla);
	}
	else if (m_nNumber == OBJ_GIMMICK)
	{
		CDebugProc::Print("���ݐݒu����M�~�b�N�̎��:%d 8/9�Ŏ�ނ�ύX", m_nNumgim);
	}


	//CDebugProc::Print("���݂̃I�u�W�F�N�g�̍��W:x:%f y:%f z:%f", Selectpos.x, Selectpos.y, Selectpos.z);

}

//=============================================================================
// �}�b�v�f�[�^�ǂݍ���
//=============================================================================
void CEditor::Load()
{
	const int lenLine = 1024;	//1�P��̍ő吔
	char strLine[lenLine];		//�ǂݍ��ݗp�̕�����
								//DEBUG_MAPDATA
								//FILE_MAPDATA
	FILE*fp = fopen(FILE_MAPDATA, "r");		//�t�@�C���ǂݍ���
	if (fp != NULL)
	{
		for (int i = 0; i < lenLine; i++)
		{
			fgets(strLine, lenLine, fp);
			if (strncmp(strLine, "Object", 6) == 0)	//�Z������r���Ĉ�v���邩�ǂ������ׂ�
			{
				m_bEnd = false;
				while (m_bEnd == false)
				{
					fscanf(fp, "%s", &strLine[0]);	//�ǂݍ��񂾕������Ƃɐݒ肷��

					if (strncmp(strLine, "#", 1) == 0)
					{
						return;
					}
					else if (strncmp(strLine, "Goal", 4) == 0)
					{
						m_bFlag = true;
						m_type = OBJ_GOAL;
					}
					else if (strncmp(strLine, "Map", 3) == 0)
					{
						m_bFlag = true;
						m_type = OBJ_MAP;
					}
					else if (strncmp(strLine, "Gimmick", 7) == 0)
					{
						m_bFlag = true;
						m_type = OBJ_GIMMICK;
					}
					else if (strncmp(strLine, "Planet", 7) == 0)
					{
						m_bFlag = true;
						m_type = OBJ_PLANET;
					}
					else if (strncmp(strLine, "Pos", 3) == 0)
					{
						fscanf(fp, "%f%f%f", &m_pos.x, &m_pos.y, &m_pos.z);
					}
					else if (strncmp(strLine, "Rot", 3) == 0)
					{
						fscanf(fp, "%f%f%f", &m_rot.x, &m_rot.y, &m_rot.z);
					}
					else if (strncmp(strLine, "End", 3) == 0)
					{
						if (m_bFlag == true)
						{
							switch (m_type)
							{//�^�C�v�ɂ���ĕ�����
							case OBJ_GOAL:
								if (m_pGoal == nullptr)
								{
									m_pGoal = CGoal::Create(D3DXVECTOR3(m_pos), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
								}
								break;

							case OBJ_GIMMICK:
								break;

							case OBJ_MAP:
								if (m_pGoal == nullptr)
								{
									m_pMap = CMap::Create(m_pos);
								}
								break;

							case OBJ_PLANET:
								if (m_pPlanet[m_nNumpla] == nullptr)
								{
									m_pPlanet[m_nNumpla] = CObjectX::Create(m_nPlaFileName[m_nNumpla], m_pos, m_rot);
								}
								break;

							default:
								break;
							}
							m_bEnd = true;
							break;
						}
					}
				}
			}
		}
		fclose(fp);
	}
}

//=============================================================================
//����
//=============================================================================
CEditor * CEditor::Create()
{
	CEditor *pEditor = new CEditor;

	pEditor->Init();
	pEditor->Load();

	return pEditor;
}

//=============================================================================
//�I�u�W�F�N�g�̈ʒu�̕ۑ�
//=============================================================================
void CEditor::SaveObject()
{
	FILE*fp = fopen("data\\TXT\\ObjectPos.txt", "w");		//�t�@�C���ǂݍ���

	fprintf(fp, "Object\n");
	fprintf(fp, "Goal\n");
	fprintf(fp, "Pos %.1f %.1f %.1f\n", m_pGoal->GetPosition().x, m_pGoal->GetPosition().y, m_pGoal->GetPosition().z);
	fprintf(fp, "Rot 0.0f 0.0f 0.0f\n");
	fprintf(fp, "End\n");

	fclose(fp);
}

//=============================================================================
//�I�u�W�F�N�g�̈ʒu�̕ۑ�
//=============================================================================
void CEditor::Pass()
{
	//���̃I�u�W�F�N�g
	m_nPlaFileName[0] = "data\\MODEL\\X_File\\Earth_000.x";
	m_nPlaFileName[1] = "data\\MODEL\\X_File\\Sun_000.x";
	m_nPlaFileName[2] = "data\\MODEL\\X_File\\Moon_000.x";
	m_nPlaFileName[3] = "data\\MODEL\\X_File\\Mars_000.x";
	m_nPlaFileName[4] = "data\\MODEL\\X_File\\Jupiter_000.x";
	m_nPlaFileName[5] = "data\\MODEL\\X_File\\Venus_000.x";
	m_nPlaFileName[6] = "data\\MODEL\\X_File\\Saturn_000.x";
	m_nPlaFileName[7] = "data\\MODEL\\X_File\\Neptune_000.x";
	m_nPlaFileName[8] = "data\\MODEL\\X_File\\Uranus_000.x";
	m_nPlaFileName[9] = "data\\MODEL\\X_File\\Pluto_000.x";

	//���̃I�u�W�F�N�g
	m_nGimmickName[0] = "data/MOTION/Gimmick001Hummer.txt";
	m_nGimmickName[1] = "data/MOTION/Gimmick002RotateStick.txt";
	m_nGimmickName[2] = "data/MOTION/Gimmick003Gate.txt";
	m_nGimmickName[3] = "data/MOTION/Gimmick004ElasticStick.txt";
}


//=============================================================================
//���͏���
//=============================================================================
void CEditor::Input()
{
	if (CInputKeyboard::Trigger(DIK_O) == true)
	{//O(�I�[)�L�[���������Ƃ�
		SaveObject();
	}

	if (CInputKeyboard::Trigger(DIK_0) == true)
	{//0(�[��)�L�[���������Ƃ�
		if (m_nNumber++ >= OBJ_MAX - 1)
		{
			m_nNumber = 0;
		}
	}
	//9�L�[���������Ƃ�
	if (CInputKeyboard::Trigger(DIK_9)) 
	{
	 //�ő吔�𒴂����Ƃ�0�Ƀ��Z�b�g
		switch (m_nNumber)
		{
		case OBJ_PLANET:
			m_nNumpla++;

			if (m_nNumpla >= MAX_PLANET)
			{
				m_nNumpla = 0;
			}
			break;

		case OBJ_GIMMICK:
			m_nNumgim++;

			if (m_nNumgim>=MAX_GIMMICK)
			{
				m_nNumgim = 0;
			}
		default:
			break;
		}
	}
	//8�L�[���������Ƃ�
	if (CInputKeyboard::Trigger(DIK_8))
	{//������������MAX�ɂ���
		switch (m_nNumber)
		{
		case OBJ_PLANET:
			m_nNumpla--;

			if (m_nNumpla < 0)
			{
				m_nNumpla = MAX_PLANET - 1;
			}
			break;

		case OBJ_GIMMICK:
			m_nNumgim--;

			if (m_nNumgim < 0)
			{
				m_nNumgim = MAX_GIMMICK - 1;
			}
		default:
			break;
		}
	}
	

	//����/�ړ�
	if (CInputKeyboard::Trigger(DIK_P))
	{
		m_pPlayer = CGame::GetPlayer(0);
		D3DXVECTOR3 pos = m_pPlayer->GetPosition();

		switch (m_nNumber)
		{
		case OBJ_GOAL:
			if (m_pGoal == nullptr)
			{//�S�[���̈ʒu�����݂̃v���C���[�̈ʒu�Ɉړ�
				m_pGoal = CGoal::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
			else
			{
				m_pGoal->SetPosition(pos);
			}
			break;

		case OBJ_MAP:
			break;

		case OBJ_GIMMICK:
			//����
			if (m_nGimmickName[m_nNumgim] != nullptr)
			{
				CGimmick::Create(m_nGimmickName[m_nNumgim], pos);
			}
			break;

		case OBJ_PLANET:
			if (m_pPlanet[m_nNumpla] == nullptr)
			{//��������ĂȂ��ꍇ����
				m_pPlanet[m_nNumpla] = CObjectX::Create(m_nPlaFileName[m_nNumpla], pos, m_rot);
			}
			else
			{//���g������ꍇ�ړ�
				m_pPlanet[m_nNumpla]->SetPos(pos);
			}
			break;

		default:
			break;

		}
	}
}

//=============================================================================
// �S�[���̎擾
//=============================================================================
CGoal* CEditor::GetGoal()
{
	return m_pGoal;
}