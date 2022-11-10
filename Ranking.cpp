//========================================================
//�X�R�A��ʂ̏���
//========================================================
#include "Ranking.h"
#include "input.h"
#include <math.h>
#include "application.h"
#include "renderer.h"
#include <string>
#include "Score.h"
#include <iostream>
#include <vector>
#include <algorithm>

//
int	CRanking::m_Time;					// ����

//=========================================
//�R���X�g���N�^
//=========================================
CRanking::CRanking()
{
}

//=========================================
//�f�X�g���N�^
//=========================================
CRanking::~CRanking()
{

}

//=========================================
//�I�u�W�F�N�g�̏���������
//=========================================
HRESULT CRanking::Init()
{
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		m_apScore[nCnt] = CScore::Create(D3DXVECTOR3(470.0f, 190.0f + 119.0f*nCnt, 0.0f));
	}
	Load();
	Ranking();
	Save();

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CRanking::Update()
{

}

//=========================================
//�I�u�W�F�N�g�̏I������
//=========================================
void CRanking::Uninit()
{

}

//=========================================
//�I�u�W�F�N�g�̕`�揈��
//=========================================
void CRanking::Draw()
{

}

//=========================================
//�I�u�W�F�N�g�̃N���G�C�g
//=========================================
CRanking* CRanking::Create()
{
	CRanking* pRanking = nullptr;

	pRanking = new CRanking;

	if (pRanking != nullptr)
	{
		pRanking->Init();
	}

	return pRanking;
}

//=========================================
// �ŐV�̃X�R�A���擾
//=========================================
void CRanking::SetCurrentScore(int Time)
{
	m_Time = Time;
}

//=========================================
// �ۑ�
//=========================================
void CRanking::Save()
{
	FILE*fp = fopen("data\\TXT\\Ranking.txt", "w");		//�t�@�C���ǂݍ���
	const int lenLine = 1024;							//1�P��̍ő吔
	for (int i = 0; i < MAX_RANK; i++)
	{
		if (fp != NULL)
		{
			fprintf(fp, "%d\n", m_apScore[i]->GetTime());	//�ǂݍ��񂾕������Ƃɐݒ肷��
		}
	}

	fclose(fp);
}

//=========================================
// �ǂݍ���
//=========================================
void CRanking::Load()
{
	FILE*fp = fopen("data\\TXT\\Ranking.txt", "r");		//�t�@�C���ǂݍ���
	const int lenLine = 1024;							//1�P��̍ő吔
	int nrScore = 0;									//�ǂݍ��ݗp�̕�����

	for (int i = 0; i < MAX_RANK; i++)
	{
		if (fp != NULL)
		{
			fscanf(fp, "%d", &nrScore);	//�ǂݍ��񂾕������Ƃɐݒ肷��
			m_apScore[i]->SetTime(nrScore);
		}
	}

	fclose(fp);
}

//=========================================
// �����L���O�̕��ёւ�
//=========================================
void CRanking::Ranking()
{
	if (m_Time <= m_apScore[MAX_RANK-1]->GetTime())
	{//��r
		m_apScore[MAX_RANK - 1]->SetTime(m_Time);

		//�����ɕ��ѕς���
		std::vector<int> rank = { m_apScore[0]->GetTime(), m_apScore[1]->GetTime(), m_apScore[2]->GetTime(),
			m_apScore[3]->GetTime(), m_apScore[4]->GetTime() };
		std::sort(rank.begin(), rank.end());
		for (int i = 0; i < MAX_RANK; i++)
		{//���ѕς��������
			m_apScore[i]->SetTime(rank[i]);
			if (m_Time == rank[i])
			{
				m_nRankUpdate = i;
			}
		}
	}
}