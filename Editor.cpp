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

//�ÓI�����o�ϐ�
CPlayer*	CEditor::pPlayer = nullptr;
CGoal*		CEditor::m_pGoal = nullptr;			//�S�[��
CMeshField*	CEditor::m_Map = nullptr;			//�}�b�v
CObject*	CEditor::m_SelectObj = nullptr;		//�I�𒆂̃I�u�W�F�N�g

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEditor::CEditor()
{
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
	m_Map = nullptr;
	pPlayer = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�o�����W
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�o�������ۂ̊p�x
	m_bEnd = false;									//�I���t���O
	m_bFlag = false;								//�����t���O
	m_nNumber = 0;									//���ݐݒ肷��u���b�N�̃^�C�v
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
	if (m_Map != nullptr)
	{
		m_Map = nullptr;
	}
	if (pPlayer != nullptr)
	{
		pPlayer = nullptr;
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
	if (CInputKeyboard::Trigger(DIK_P) == true)
	{
		pPlayer = CGame::GetPlayer(0);
		D3DXVECTOR3 pos = pPlayer->GetPosition();

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
			break;
		default:
			break;

		}
	}

	if (CInputKeyboard::Trigger(DIK_O) == true)
	{//O(�I�[)�L�[���������Ƃ�
		SaveObject();
	}

	if (CInputKeyboard::Trigger(DIK_0) == true)
	{//0(�[��)�L�[���������Ƃ�
		if (m_nNumber++ >= OBJ_MAX-1)
		{
			m_nNumber = 0;
		}
	}

	switch (m_nNumber)
	{
	case OBJ_GOAL:
		m_SelectObj = m_pGoal;
		break;

	case OBJ_MAP:
		//m_SelectObj = m_Map;
		break;

	case OBJ_GIMMICK:
		//m_SelectObj = m_Map;
		break;
	default:
		break;

	}

	D3DXVECTOR3 Selectpos= m_SelectObj->GetPosition();

	CDebugProc::Print("���ݒu���I�u�W�F�N�g:%d 0�L�[�Ŏ�ނ�ύX", m_nNumber);
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
									//m_Map = CMeshField::Create();
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
