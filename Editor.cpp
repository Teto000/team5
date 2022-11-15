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
#include"objectX.h"
#include"Gimmick.h"
#include"stack_block.h"

//�ÓI�����o�ϐ�
CGoal*		CEditor::m_pGoal = nullptr;										//�S�[��
CSBlock*	CEditor::m_pStackBlock[MAX_STACK_BLOCK] = { nullptr };			//�u���b�N�̎R
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEditor::CEditor()
{
	m_pGoal = nullptr;						//�S�[��
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
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�o�����W
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�o�������ۂ̊p�x
	m_bEnd = false;									//�I���t���O
	m_bFlag = false;								//�����t���O
	m_nNumber = 0;									//���ݐݒ肷��u���b�N�̃^�C�v
	m_nNumpla = 0;

	for (int nCnt = 0; nCnt < MAX_STACK_BLOCK; nCnt++)
	{
		if (m_pStackBlock[nCnt] != nullptr)
		{
			m_pStackBlock[nCnt] = nullptr;
		}
	}
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

	for (int i = 0; i < MAX_PLANET; i++)
	{
		if (m_pPlanet[i] != nullptr)
		{
			m_pPlanet[i] = nullptr;
		}
	}

	for (int nCnt = 0; nCnt < MAX_STACK_BLOCK; nCnt++)
	{
		if (m_pStackBlock[nCnt] != nullptr)
		{
			m_pStackBlock[nCnt] = nullptr;
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
#ifdef _DEBUG
	Input();

	//�f�o�b�O�p�����̕\��
	CDebugProc::Print("���ݒu���I�u�W�F�N�g:%d 0(�[��)�L�[�Ŏ�ނ�ύX", m_nNumber);
	CDebugProc::Print("O(�I�[)�L�[�ŃI�u�W�F�N�g�̍��W�t�@�C���ɏo�͂���");
	CDebugProc::Print("P�L�[�ŃI�u�W�F�N�g�̐���/�ړ�");

	//�I�𒆂̃I�u�W�F�N�g�ɉ����ĕ\������镶����ύX
	if (m_nNumber == OBJ_PLANET)
	{
		CDebugProc::Print("���ݐݒu����f���̎��:%d 8/9�Ŏ�ނ�ύX", m_nNumpla);
	}
	else if (m_nNumber == OBJ_GIMMICK)
	{
		CDebugProc::Print("���ݐݒu����M�~�b�N�̎��:%d 8/9�Ŏ�ނ�ύX", m_nNumgim);
	}
#endif // !_DEBUG_
}

//=============================================================================
// �}�b�v�f�[�^�ǂݍ���
//=============================================================================
void CEditor::Load()
{
	const int lenLine = 1024;	//1�P��̍ő吔
	char strLine[lenLine];		//�ǂݍ��ݗp�̕�����
	m_gim = 0;
								
	FILE*fp = fopen(FILE_MAPDATA, "r");		//�t�@�C���ǂݍ���
	if (fp != NULL)
	{
		for (int i = 0; i < lenLine; i++)
		{
			fgets(strLine, lenLine, fp);
			if (strncmp(strLine, "Object", 6) == 0)	//Object��ǂݍ��ނ܂Ń��[�v
			{
				m_bEnd = false;
				while (m_bEnd == false)
				{
					fscanf(fp, "%s", &strLine[0]);	//�ǂݍ��񂾕������Ƃɐݒ肷��

					if (strncmp(strLine, "#", 1) == 0)
					{//�R�����g�A�E�g
						continue;
					}
					else if (strncmp(strLine, "Goal", 4) == 0)
					{//��������I�u�W�F�N�g���S�[���Ɏw��
						m_bFlag = true;
						m_type = OBJ_GOAL;
					}
					else if (strncmp(strLine, "StackBlock", 10) == 0)
					{//��������I�u�W�F�N�g���u���b�N�Ɏw��
						m_bFlag = true;
						m_type = OBJ_STACK_BLOCK;
					}
					else if (strncmp(strLine, "Gimmick", 7) == 0)
					{//��������I�u�W�F�N�g����Q���Ɏw��
						m_bFlag = true;
						m_type = OBJ_GIMMICK;
					}
					else if (strncmp(strLine, "Planet", 7) == 0)
					{//��������I�u�W�F�N�g��f���Ɏw��
						m_bFlag = true;
						m_type = OBJ_PLANET;
					}
					else if (strncmp(strLine, "Pos", 3) == 0)
					{//��������I�u�W�F�N�g�̈ʒu���w��
						fscanf(fp, "%f%f%f", &m_pos.x, &m_pos.y, &m_pos.z);
					}
					else if (strncmp(strLine, "Rot", 3) == 0)
					{//��������I�u�W�F�N�g�̌������w��
						fscanf(fp, "%f%f%f", &m_rot.x, &m_rot.y, &m_rot.z);
					}
					else if (strncmp(strLine, "Type", 4) == 0)
					{//��������I�u�W�F�N�g�̎�ނ��w��
						fscanf(fp, "%d", &m_nNumgim);
						m_nNumpla = m_nNumgim;
					}
					else if (strncmp(strLine, "End", 3) == 0)
					{//�I�u�W�F�N�g�̐���
						if (m_bFlag == true)
						{
							switch (m_type)
							{//�^�C�v�ɂ���ĕ�����
							case OBJ_GOAL:
								if (m_pGoal == nullptr)
								{//�S�[���͈�̂̂ݐ��������悤�ɂ���
									m_pGoal = CGoal::Create(D3DXVECTOR3(m_pos), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
								}
								break;

							case OBJ_GIMMICK:
								if (m_nNumgim >= 0 && m_nNumgim < MAX_GIMMICK)
								{//��Q��
									m_pGimmick[m_gim]=	CGimmick::Create(m_nNumgim, m_pos, m_rot);
									m_gim++;
								}
								break;

							case OBJ_PLANET:
								if (m_pPlanet[m_nNumpla] == nullptr)
								{//�f��
									m_pPlanet[m_nNumpla] = CObjectX::Create(m_nPlaFileName[m_nNumpla], m_pos, m_rot);
								}
								break;

							case OBJ_STACK_BLOCK:
								for (int nCnt = 0; nCnt < MAX_STACK_BLOCK; nCnt++)
								{//�u���b�N
									if (m_pStackBlock[nCnt] == nullptr)
									{
										m_pStackBlock[nCnt] = CSBlock::Create(m_pos,m_rot);
										break;
									}
								}
								break;

							default:
								break;
							}
							m_bEnd = true;	//�I�u�W�F�N�g�𐶐������̂Ń��[�v�I��
							break;
						}
					}
				}
			}
		}
		fclose(fp);
	}

	//���l���Z�b�g
	m_nNumgim = 0;
	m_nNumpla = 0;
}

//=============================================================================
//����
//=============================================================================
CEditor * CEditor::Create()
{
	CEditor *pEditor = new CEditor;

	pEditor->Init();	//������
	pEditor->Load();	//�ǂݍ���

	return pEditor;
}

//=============================================================================
//�I�u�W�F�N�g�̈ʒu�̕ۑ�
//=============================================================================
void CEditor::SaveObject()
{
	//�I�u�W�F�N�g�̏��ۑ��p
	CObject* pObj;		//�I�u�W�F�N�g�̏��
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����

	FILE*fp = fopen("data\\TXT\\ObjectPos.txt", "w");		//�t�@�C���ǂݍ���

	//�S�[���̏���ۑ�
	fprintf(fp, "Object\n");
	fprintf(fp, "Goal\n");
	fprintf(fp, "Pos %.1f %.1f %.1f\n", m_pGoal->GetPosition().x, m_pGoal->GetPosition().y, m_pGoal->GetPosition().z);
	fprintf(fp, "Rot %.2f %.2f %.2f\n", m_pGoal->GetBaseRot().x, m_pGoal->GetBaseRot().y, m_pGoal->GetBaseRot().z);
	fprintf(fp, "End\n\n");

	//��Q���̏���ۑ�
	pObj = CObject::GetTop(CObject::OBJTYPE_GIMMICK);		//��Q���̃��X�g�\���̐擪���擾

	while (pObj)
	{//pObj��null����Ȃ��Ȃ�
	 //���̃I�u�W�F�N�g��ۑ�
		CObject* pObjNext = pObj->GetNext();

		//�I�u�W�F�N�g�̈ʒu�ƌ������擾
		 pos= pObj->GetPosition();
		 rot = pObj->GetBaseRot();

		fprintf(fp, "Object\n");
		fprintf(fp, "Gimmick\n");
		fprintf(fp, "Pos %.1f %.1f %.1f\n", pos.x, pos.y, pos.z);
		fprintf(fp, "Rot %.2f %.2f %.2f\n", rot.x, rot.y, rot.z);
		fprintf(fp, "Type %d\n",pObj->GetType());
		fprintf(fp, "End\n\n");

		//���̃I�u�W�F�N�g�̃A�h���X����
		pObj = pObjNext;
	}

	//�f���̏���ۑ�
	pObj = CObject::GetTop(CObject::OBJTYPE_PLANET);		//�f���̃��X�g�\���̐擪���擾

	while (pObj)
	{//pObj��null����Ȃ��Ȃ�
	 //���̃I�u�W�F�N�g��ۑ�
		CObject* pObjNext = pObj->GetNext();

		//�I������
		pos = pObj->GetPosition();
		rot = pObj->GetBaseRot();

		fprintf(fp, "Object\n");
		fprintf(fp, "Planet\n");
		fprintf(fp, "Pos %.1f %.1f %.1f\n", pos.x, pos.y, pos.z);
		fprintf(fp, "Rot %.2f %.2f %.2f\n", rot.x, rot.y, rot.z);
		fprintf(fp, "Type %d\n", pObj->GetType());
		fprintf(fp, "End\n\n");

		//���̃I�u�W�F�N�g�̃A�h���X����
		pObj = pObjNext;
	}

	fclose(fp);
}

//=============================================================================
//�f���̃p�X��ǂݍ���
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
	m_nPlaFileName[10] = "data\\MODEL\\X_File\\BG_000.x";
}

//=============================================================================
//���͏���
//=============================================================================
void CEditor::Input()
{
	//�I�u�W�F�N�g�̈ʒu��ۑ�
	if (CInputKeyboard::Trigger(DIK_O) == true)
	{//O(�I�[)�L�[���������Ƃ�
		SaveObject();
	}

	//�i���o�[�̐؂�ւ�
	if (CInputKeyboard::Trigger(DIK_0) == true)
	{//0(�[��)�L�[���������Ƃ�
		if (m_nNumber++ >= OBJ_MAX - 1)
		{
			m_nNumber = 0;
		}
	}
	
	//�I�u�W�F�N�g�̎�ނ̕ύX
	if (CInputKeyboard::Trigger(DIK_9)) 
	{//9�L�[���������Ƃ�
		switch (m_nNumber)
		{
		case OBJ_PLANET:
			m_nNumpla++;

			if (m_nNumpla >= MAX_PLANET)
			{//�ő吔�𒴂����Ƃ�0�Ƀ��Z�b�g
				m_nNumpla = 0;
			}
			break;

		case OBJ_GIMMICK:
			m_nNumgim++;

			if (m_nNumgim>=MAX_GIMMICK)
			{//�ő吔�𒴂����Ƃ�0�Ƀ��Z�b�g
				m_nNumgim = 0;
			}
		default:
			break;
		}
	}
	else if (CInputKeyboard::Trigger(DIK_8))
	{//8�L�[���������Ƃ�
		switch (m_nNumber)
		{
		case OBJ_PLANET:
			m_nNumpla--;

			if (m_nNumpla < 0)
			{//�����𒴂��������l�ɂ���
				m_nNumpla = MAX_PLANET - 1;
			}
			break;

		case OBJ_GIMMICK:
			m_nNumgim--;

			if (m_nNumgim < 0)
			{//�����𒴂��������l�ɂ���
				m_nNumgim = MAX_GIMMICK - 1;
			}
		default:
			break;
		}
	}
	
	//P�L�[�Ő���/�ړ�
	if (CInputKeyboard::Trigger(DIK_P))
	{
		CPlayer* m_pPlayer = CGame::GetPlayer(0);
		D3DXVECTOR3 pos = m_pPlayer->GetPosition();
		D3DXVECTOR3 rot = m_pPlayer->GetRot();

		switch (m_nNumber)
		{
		case OBJ_GOAL:
			if (m_pGoal == nullptr)
			{//��������Ă��Ȃ��ꍇ�̂ݐ���
				m_pGoal = CGoal::Create(pos, rot);
			}
			else
			{//���g������ꍇ���݂̃v���C���[�̈ʒu�Ɉړ�
				m_pGoal->SetPosition(pos);
			}
			break;

		case OBJ_GIMMICK:
			CGimmick::Create(m_nNumgim, pos, rot);	//����
			break;

		case OBJ_PLANET:
			if (m_pPlanet[m_nNumpla] == nullptr)
			{//��������Ă��Ȃ��ꍇ�̂ݐ���
				m_pPlanet[m_nNumpla] = CObjectX::Create(m_nPlaFileName[m_nNumpla], pos, rot);
			}
			else
			{//���g������ꍇ���݂̃v���C���[�̈ʒu�Ɉړ�
				m_pPlanet[m_nNumpla]->SetPos(pos);
			}
			break;

		case OBJ_STACK_BLOCK:
			if (m_pStackBlock[0] == nullptr)
			{//�e�X�g�p�Ɉ��������
				m_pStackBlock[0] = CSBlock::Create(m_pos, m_rot);
				break;
			}

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