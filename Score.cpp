//========================================================
//�X�R�A��ʂ̏���
//========================================================
#include "Score.h"
#include "input.h"
#include <math.h>
#include "application.h"
#include "renderer.h"
#include<string>

//=========================================
//�R���X�g���N�^
//=========================================
CScore::CScore()
{
}

//=========================================
//�f�X�g���N�^
//=========================================
CScore::~CScore()
{

}

//=========================================
//�I�u�W�F�N�g�̏���������
//=========================================
HRESULT CScore::Init(D3DXVECTOR3 pos)
{
	CObject2D::Init(pos);

	//�T�C�Y�̐ݒ�
	CObject2D::SetSize(0.0f, 0.0f);

	//// �e�N�X�`���̐ݒ�
	//CObject2D::SetTexture(CTexture::TEXTURE_NONE);

	m_pos = GetPosition();

	for (int nCnt = 0; nCnt < MAX_DIGITS; nCnt++)
	{
		m_pNumber[nCnt] = CNumber::Create(D3DXVECTOR3(m_pos.x + 50.0f * nCnt, m_pos.y, m_pos.z));
	}

	// �N���A�^�C����\��
	m_Minutes = m_Time / 60000;					// ���ɕϊ�
	m_Seconds = m_Time - (m_Minutes * 60000);	// ���ɕϊ����������������l��b�ɓ����
	Set();

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CScore::Update()
{
	// �N���A�^�C����\��
	m_Minutes = m_Time / 60000;					// ���ɕϊ�
	m_Seconds = m_Time - (m_Minutes * 60000);	// ���ɕϊ����������������l��b�ɓ����
	Set();
}

//=========================================
//�I�u�W�F�N�g�̏I������
//=========================================
void CScore::Uninit()
{
	CObject2D::Uninit();
}

//=========================================
//�I�u�W�F�N�g�̕`�揈��
//=========================================
void CScore::Draw()
{
	CObject2D::Draw();
}

//=========================================
//�I�u�W�F�N�g�̃N���G�C�g
//=========================================
CScore* CScore::Create(const D3DXVECTOR3 &pos)
{
	CScore* pCScore = nullptr;

	pCScore = new CScore;

	if (pCScore != nullptr)
	{
		pCScore->Init(pos);
	}

	return pCScore;
}

//=========================================
// �X�R�A�̉��Z����
//=========================================
void CScore::Set()
{
	int aPosTexU[8];		//�e���̐������i�[

	for (int nCnt = 0; nCnt < MAX_DIGITS; nCnt++)
	{
		// pow�Ō������o���B
		int nCntNumber = MAX_DIGITS - nCnt - 1;		// define����0���Ȃ��̂�-1
		int nNum0 = (int)pow(10, nCntNumber + 1);	// ������10000000~�̌`�ɂ���
		int nNum1 = (int)pow(10, nCntNumber);		// �������������Ȃ������`�ɂ���

		// 2����
		if (nCnt == 1)
		{// ��
		 // (m_Score % 10^X+1) / 10^X
			aPosTexU[nCnt] = (m_Minutes % (int)pow(10, nCnt));
			m_pNumber[nCnt]->Set(aPosTexU[nCnt]);
		}
		// 1���ځA0�����N���Ă��܂��̂Ł��Ɠ����ɏ����ł��Ȃ��E�E�E
		else if (nCnt == 0)
		{
			// (m_Score % 10^X+1) / 10^X
			aPosTexU[nCnt] = (m_Minutes * 0.1f);
			m_pNumber[nCnt]->Set(aPosTexU[nCnt]);
		}
		else if (nCnt == 2 || nCnt == 5)
		{// �Z�~�R����
		 // �O���ڂɃZ�~�R������ݒu����B
			aPosTexU[nCnt] = 11;

			m_pNumber[nCnt]->Set(aPosTexU[nCnt]);
		}
		else if (nCnt == 6 || nCnt == 7)
		{// �Z�~�R������̐���
			int nNumDec = nNum0 * 10;
			int nNumDec2 = nNum1 * 10;

			// (m_Score % 10^X+1) / 10^X
			aPosTexU[nCnt] = (m_Seconds % nNumDec) / nNumDec2;
			m_pNumber[nCnt]->Set(aPosTexU[nCnt]);
		}
		else
		{// �Z�~�R�����ƃZ�~�R�����̊Ԃ̐���(�Ȃɂ����ĂȂ�)
		 // (m_Score % 10^X+1) / 10^X
			aPosTexU[nCnt] = (m_Seconds % nNum0) / nNum1;
			m_pNumber[nCnt]->Set(aPosTexU[nCnt]);
		}
	}
}

//=========================================
// ���Ԃ̐ݒ�
//=========================================
void CScore::SetTime(int Time)
{
	m_Time = Time;
}

//=========================================
// ���Ԃ̎擾
//=========================================
int CScore::GetTime()
{
	return m_Time;
}

//=========================================
// ���̎擾
//=========================================
int CScore::GetSec()
{
	return m_Seconds;
}

//=========================================
// �b�̎擾
//=========================================
int CScore::GetMin()
{
	return m_Minutes;
}

//=========================================
// �ǂݍ���
//=========================================
void CScore::Load()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	FILE*fp = fopen("data\\TXT\\Ranking.txt", "r");		//�t�@�C���ǂݍ���
	const int lenLine = 1024;							//1�P��̍ő吔
	int nrScore = 0;									//�ǂݍ��ݗp�̕�����

	for (int i = 0; i < MAX_RANK; i++)
	{
		if (fp != NULL)
		{
			fscanf(fp, "%d", &m_apScore[i]);	//�ǂݍ��񂾕������Ƃɐݒ肷��
		}
	}

	fclose(fp);
}

////=========================================
//// �����L���O�̕��ёւ�
////=========================================
//void CScore::Ranking()
//{
//	int nSave;
//	m_nRankUpdate = -1;
//	m_nTimerRanking = 0;
//	if (m_Time <= m_apScore[0])
//	{//��r
//		m_nRankUpdate++;
//		m_apScore[0];
//		for (int i = 0; i < MAX_RANK - 1; i++)
//		{
//			if (m_apScore[i] > m_apScore[i + 1])
//			{//���בւ�
//				nSave = m_apScore[i + 1];
//				m_apScore[i + 1] = m_apScore[i];
//				m_apScore[i] = nSave;
//				m_nRankUpdate++;
//			}
//		}
//	}
//}