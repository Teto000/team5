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
//�X�V
//=============================================================================
void CEditor::Update()
{

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
				bEnd = false;
				while (bEnd == false)
				{
					fscanf(fp, "%s", &strLine[0]);	//�ǂݍ��񂾕������Ƃɐݒ肷��

					if (strncmp(strLine, "#", 1) == 0)
					{
						return;
					}
					else if (strncmp(strLine, "Goal", 4) == 0)
					{
						bFlag = true;
						m_type = OBJ_GOAL;
					}
					else if (strncmp(strLine, "Map", 3) == 0)
					{
						bFlag = true;
						m_type = OBJ_MAP;
					}
					else if (strncmp(strLine, "Gimmick", 7) == 0)
					{
						bFlag = true;
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
						switch (m_type)
						{//�^�C�v�ɂ���ĕ�����
						case OBJ_GOAL:
							CGoal::Create(D3DXVECTOR3(m_pos), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
							break;
						case OBJ_GIMMICK:
							break;
						case OBJ_MAP:
							CMeshField::Create();
							break;
						default:
							break;
						}
						bEnd = true;
						break;
					}
				}
			}
		}
		fclose(fp);
	}
}
