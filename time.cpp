//========================================================
//�X�R�A��ʂ̏���
//========================================================
#include "time.h"
#include "input.h"
#include "game.h"
#include <math.h>

int CTime::m_Time = 0;
int CTime::m_Seconds = 0;
int CTime::m_Minutes = 0;
//=========================================
//�R���X�g���N�^
//=========================================
CTime::CTime()
{
}

//=========================================
//�f�X�g���N�^
//=========================================
CTime::~CTime()
{

}

//=========================================
//�I�u�W�F�N�g�̏���������
//=========================================
HRESULT CTime::Init(D3DXVECTOR3 pos)
{
	CObject2D::Init(pos);

	//�T�C�Y�̐ݒ�
	CObject2D::SetSize(0.0f,0.0f);

	//�Q�[���̊J�n�������擾
	m_StartGameTime = timeGetTime();

	// �e�N�X�`���̐ݒ�	CObject2D::SetTexture(CTexture::TEXTURE_NONE);

	m_pos = GetPosition();

	for (int nCnt = 0; nCnt < MAX_DIGITS; nCnt++)
	{
		m_pNumber[nCnt] = CNumber::Create(D3DXVECTOR3(m_pos.x + 50.0f * nCnt, m_pos.y, m_pos.z));
	}

	Set();

	return S_OK;
}

//=========================================
//�I�u�W�F�N�g�̍X�V����
//=========================================
void CTime::Update()
{
	// ���݂̃Q�[�����Ԃ�\��
	m_Time = timeGetTime() - m_StartGameTime;
	m_Minutes = m_Time / 60000;					// ���ɕϊ�
	m_Seconds = m_Time - (m_Minutes * 60000);	// ���ɕϊ����������������l��b�ɓ����

	// ���Ԑݒ�
	Set();

	if (CGame::GetFinish())
	{//�I���t���O�������Ă���Ȃ�
		for (int i = 0; i < MAX_DIGITS; i++)
		{
			if (m_pNumber[i] != nullptr)
			{//null����Ȃ��Ȃ�
				m_pNumber[i]->Uninit();
				m_pNumber[i] = nullptr;
			}
		}
	}
}

//=========================================
//�I�u�W�F�N�g�̏I������
//=========================================
void CTime::Uninit()
{
	CObject2D::Uninit();
}

//=========================================
//�I�u�W�F�N�g�̕`�揈��
//=========================================
void CTime::Draw()
{
	CObject2D::Draw();
}

//=========================================
//�I�u�W�F�N�g�̃N���G�C�g
//=========================================
CTime* CTime::Create(const D3DXVECTOR3 &pos)
{
	CTime* pCTime = nullptr;

	pCTime = new CTime;

	if (pCTime != nullptr)
	{
		pCTime->Init(pos);
	}

	return pCTime;
}

//=========================================
// �X�R�A�̉��Z����
//=========================================
void CTime::Set()
{
	int aPosTexU[8];		//�e���̐������i�[

	for (int nCnt = 0; nCnt < MAX_DIGITS; nCnt++)
	{
		if (m_pNumber[nCnt] == nullptr)
		{//null����Ȃ��Ȃ�
			return;
		}

		// pow�Ō������o���B
		int nCntNumber = MAX_DIGITS - nCnt - 1;		// define����0���Ȃ��̂�-1
		int nNum0 = (int)pow(10, nCntNumber + 1);	// ������10000000~�̌`�ɂ���
		int nNum1 = (int)pow(10, nCntNumber);		// �������������Ȃ������`�ɂ���

		// 2����
		if (nCnt == 1)
		{// ��
			// (m_Score % 10^X+1) / 10^X
			aPosTexU[nCnt] = (m_Minutes % (int)pow(10,nCnt));
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
// �X�R�A�̉��Z����
//=========================================
void CTime::Add(int Value)
{
	m_Seconds += Value;
	Set();
}