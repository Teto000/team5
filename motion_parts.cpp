//=============================================================================
//
// motion_parts.cpp
// Author : kozuna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "motion_parts.h"
#include <assert.h>
#include "application.h"
#include "renderer.h"

const D3DXVECTOR3 CMotionParts::INIT_POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
CMotionParts *CMotionParts::m_pMotionPartsTop = nullptr;
CMotionParts *CMotionParts::m_pMotionPartsCurrent = nullptr;
int CMotionParts::m_nModelMax = 0;
int CMotionParts::m_nMotionPlayMotonNum[MAX_MOTION_ALL] = {};
int CMotionParts::m_nMotionDestMotonNum[MAX_MOTION_ALL] = {};
int CMotionParts::m_nMotionRegistrationNum[MAX_MOTION_ALL] = {};
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CMotionParts::CMotionParts()
{
	m_nFrame = 0;
	m_nKey = 0;
	m_pRarent = nullptr;

	if (m_pMotionPartsTop == nullptr)
	{
		m_pMotionPartsTop = this;
	}

	m_pNextMotionParts = nullptr;

	m_pLastTimeMotionParts = GetCurrentMotionParts();

	if (m_pLastTimeMotionParts != nullptr)
	{
		m_pLastTimeMotionParts->SetNextMotionParts(this);

	}
	SetCurrentMotionParts(this);

	m_nModelObjNum = m_nModelMax;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CMotionParts::~CMotionParts()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CMotionParts::Init()
{
	if (FAILED(C3DObject::Init()))
	{
		return -1;
	}
	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CMotionParts::Uninit()
{
	if (m_pMotionPartsTop == this)
	{
		m_pMotionPartsTop = m_pNextMotionParts;
	}

	if (m_pMotionPartsCurrent == this)
	{
		m_pMotionPartsCurrent = m_pLastTimeMotionParts;
	}

	if (m_pLastTimeMotionParts != nullptr)
	{
		m_pLastTimeMotionParts->SetNextMotionParts(m_pNextMotionParts);
	}

	if (m_pNextMotionParts != nullptr)
	{
		m_pNextMotionParts->SetLastTimeMotionParts(m_pLastTimeMotionParts);
	}

	for (int nMotion = 0; nMotion < m_nMotionRegistrationNum[m_nModelObjNum]; nMotion++)
	{
		if (m_MotionKey[nMotion].pKey != nullptr)
		{
			delete[] m_MotionKey[nMotion].pKey;
			m_MotionKey[nMotion].pKey = nullptr;
		}
	}


	C3DObject::Uninit();

	delete this;
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CMotionParts::Update()
{
	// �ړI�̃t���[��
	int nObjectiveFrame = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[m_nKey].nFrame;

	// �t���[���̉��Z
	m_nFrame++;
	// �ړI�̃t���[���@���@���݂̃t���[��
	if (nObjectiveFrame < m_nFrame)
	{
		// ���̈ʒu�܂ł̌v�Z
		NextMotionPosition();
		//�L�[���̊m�F
		KeyCheck();
		m_nFrame = 0;
	}

	// pos�̉��Z
	AddPosMove(m_PosMove);
	// rot�̉��Z
	AddRot(m_RotMove);

	D3DXVECTOR3 rot = GetRot();

	//���K��
	rot = NormalizationRot(rot);

	SetRot(rot);


}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CMotionParts::Draw()
{
	D3DXMATRIX mtxRarent;
	D3DXMATRIX mtx;
	CRenderer *pRenderer = CApplication::GetRenderer();


	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pRenderer->GetDevice();
	// �e�����݂���ꍇ
	if (m_pRarent != nullptr)
	{
		mtxRarent = m_pRarent->GetMatrix();
	}
	else
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxRarent);
	}
	
	//���g�̃}�g���b�N�X�̌v�Z
	CalculationMatrix();
	mtx = GetMatrix();

	//���f���̃}�g���b�N�X�@���@�e�̃��[���h�}�g���b�N�X
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRarent);

	SetMatrix(mtx);

	C3DObject::Draw();
}

//*****************************************************************************
//	�I����������
//*****************************************************************************
bool CMotionParts::IsUnused()
{
	return false;
}

//*****************************************************************************
//	���[�V�����ԍ��w��ŃZ�b�g����
//*****************************************************************************
void CMotionParts::SetMotion(int nMotionNum)
{
	m_nFrame = 0;
	m_nKey = 0;
	D3DXVECTOR3 pos = m_MotionKey[nMotionNum].pKey[m_nKey].pos;
	D3DXVECTOR3 rot = m_MotionKey[nMotionNum].pKey[m_nKey].rot;
	SetPosMove(pos);
	SetRot(rot);
}

//*****************************************************************************
//���ۂ̓����̓o�^
//*****************************************************************************
void CMotionParts::SetMotionData(KEY_SET KeyData)
{
	m_MotionKey[m_nMotionRegistrationNum[m_nModelObjNum]].bLoop = KeyData.bLoop;
	m_MotionKey[m_nMotionRegistrationNum[m_nModelObjNum]].nKeyMax = KeyData.nKeyMax;
	m_MotionKey[m_nMotionRegistrationNum[m_nModelObjNum]].pKey = KeyData.pKey;
}

//*****************************************************************************
// �L�[���̊m�F
//*****************************************************************************
void CMotionParts::KeyCheck()
{
	//�ړI�̃L�[��
	int nDestKey = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].nKeyMax;

	//�L�[���K���葽���ꍇ���Z�b�g
	if (m_nKey >= nDestKey)
	{
		m_nKey = 0;

		//���̃��[�V�����ԍ�
		int nNowMotionNum = m_nMotionPlayMotonNum[m_nModelObjNum];
		//���̃��[�V�����ԍ�
		//int nDestMotionNum = m_nMotionDestMotonNum[m_nModelObjNum];
		//���̃��[�V���������[�v���邩�ǂ���
		if (!m_MotionKey[nNowMotionNum].bLoop)
		{
			//���̃p�[�c�����f���Q�̖��[���ǂ���
			//if (GetMotionPartsPointer(m_nModelObjNum, m_nPartsNum + 1) == nullptr)
			if (m_nPartsNum == 0)
			{
				nNowMotionNum = 0;
				m_nMotionPlayMotonNum[m_nModelObjNum] = nNowMotionNum;
			}
			SetMotion(nNowMotionNum);
		}

	}
}

//*****************************************************************************
// ���̈ʒu�܂ł̌v�Z
//*****************************************************************************
void CMotionParts::NextMotionPosition()
{
	D3DXVECTOR3 nowPos, nextPos,nowRot, nextRot;
	int nFrameRatio;
	//�ړI�̃L�[��
	int nDestKey = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].nKeyMax;
	
	//���݂�KEY���ړI�𒴂�����
	if (m_nKey >= nDestKey - 1)
	{
		//���̈ʒu
		nowPos = GetPosMove();
		//���̈ʒu
		nextPos = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[0].pos;
		//���̌���
		nowRot = GetRot();
		//���̌���
		nextRot = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[0].rot;
		//�S�̃t���[�����ɑ΂��Ă̊���
		nFrameRatio = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[0].nFrame;
	}
	else
	{
		//���̈ʒu
		nowPos = GetPosMove();
		//���̈ʒu
		nextPos = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[m_nKey + 1].pos;
		//���̌���
		nowRot = GetRot();
		
		//�S�̃t���[�����ɑ΂��Ă̊���
		nFrameRatio = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[m_nKey + 1].nFrame;

		//���̌���
		nextRot = m_MotionKey[m_nMotionPlayMotonNum[m_nModelObjNum]].pKey[m_nKey + 1].rot;
	}
	
	//1�t���[��������̓�������
	D3DXVECTOR3 rotMove;
	//1�t���[��������̓����ʒu
	D3DXVECTOR3 posMove;
	
	//�ړI�̈ʒu�@�[�@���݂̈ʒu
	D3DXVECTOR3 pos = nextPos - nowPos;

	//�ړI�̌����@�[�@���݂̌���
	D3DXVECTOR3 rot = nextRot - nowRot;

	//���K��
	rot = NormalizationRot(rot);

	//�����v�Z
	//�ʒu
	posMove = pos / nFrameRatio;
	m_PosMove = posMove;
	//����
	rotMove = rot / nFrameRatio;

	//���K��
	rot = NormalizationRot(rotMove);

	m_RotMove = rotMove;

	//�L�[�̃J�E���g��i�߂�
	m_nKey++;
}

//*****************************************************************************
//�S���̃p�[�c���̈ʒu�܂ł̌v�Z
//*****************************************************************************
void CMotionParts::AllNextMotionPosition(int nMotionNum)
{
	CMotionParts* pMotionParts = m_pMotionPartsTop;

	while (pMotionParts != nullptr)
	{
		if (pMotionParts->GetModelObjNum() == nMotionNum)
		{
			pMotionParts->KeyFrameReset();
			pMotionParts->NextMotionPosition();
		}

		pMotionParts = pMotionParts->GetNextMotionParts();
	}

}

//*****************************************************************************
// �����Ƃ̈�v�����邩�ǂ���
//*****************************************************************************
bool CMotionParts::GetMotionParts(int nMotionNum, int nPartsNum)
{
	if (m_nModelObjNum == nMotionNum
		&& m_nPartsNum == nPartsNum)
	{
		return true;
	}

	return false;
}

//*****************************************************************************
// �����Ƃ̈�v�����邩�ǂ���
//*****************************************************************************
bool CMotionParts::GetMotionParts(int nMotionNum)
{
	if (m_nModelObjNum == nMotionNum)
	{
		return true;
	}

	return false;
}

//*****************************************************************************
// ALL�I������
//*****************************************************************************
void CMotionParts::ALLUninit()
{
	CMotionParts* pMotionParts = m_pMotionPartsTop;

	while (pMotionParts != nullptr)
	{
		CMotionParts* pMotionPartsNext = pMotionParts->GetNextMotionParts();
		pMotionParts->Uninit();
		pMotionParts = pMotionPartsNext;
	}
}

//*****************************************************************************
// ALL�X�V����
//*****************************************************************************
void CMotionParts::ALLUpdate()
{
	CMotionParts* pMotionParts = m_pMotionPartsTop;

	while (pMotionParts != nullptr)
	{
		pMotionParts->Update();

		if (pMotionParts->IsUnused())
		{
			CMotionParts* pMotionPartsBreak = pMotionParts;
			pMotionParts = pMotionPartsBreak->GetNextMotionParts();
			pMotionPartsBreak->Uninit();
		}
		else
		{
			pMotionParts = pMotionParts->GetNextMotionParts();
		}
	}
}

//*****************************************************************************
// ALL�`�揈��
//*****************************************************************************
void CMotionParts::ALLDraw()
{
	CMotionParts* pMotionParts = m_pMotionPartsTop;

	while (pMotionParts != nullptr)
	{
		pMotionParts->Draw();
		pMotionParts = pMotionParts->GetNextMotionParts();
	}
}

//*****************************************************************************
// ����OBJ�̐���
//*****************************************************************************
int CMotionParts::CreateMotionObj(MotionData* pMotionData, int nPartsMax)
{
	for (int nCnt = 0; nCnt < nPartsMax; nCnt++)
	{
		CMotionParts* pMotionParts = new CMotionParts;

		if (FAILED(pMotionParts->Init()))
		{
			assert(false);
		}

		pMotionParts->SetPartsNum(nCnt);//��̃��[�V�����̒��̔ԍ�
		pMotionParts->SetModelPattnNum(pMotionData[nCnt].nModelPattern);//�g�p���郂�f���̃C���f�b�N�X
		pMotionParts->SetPos(pMotionData[nCnt].pos);//���f���̈ʒu
		pMotionParts->SetRot(pMotionData[nCnt].rot);//���f���̌���
		
		if (pMotionData[nCnt].nParentNum >= 0)
		{
			//�e�̐ݒ�
			pMotionParts->SetMotionRarent(GetMotionPartsPointer(m_nModelMax, pMotionData[nCnt].nParentNum));
		}
		
		pMotionParts->SetNormal();
	}

	int nModelNum = m_nModelMax;

	//�������̂̓o�^����
	SettingCompletion();

	return nModelNum;

}

//*****************************************************************************
// �����ɍ����|�C���^��Ԃ�
//*****************************************************************************
CMotionParts * CMotionParts::GetMotionPartsPointer(int nMotionNum, int nPartsNum)
{
	CMotionParts* pMotionParts = m_pMotionPartsTop;

	while (pMotionParts != nullptr)
	{
		if (pMotionParts->GetMotionParts(nMotionNum, nPartsNum))
		{
			return pMotionParts;
		}

		pMotionParts = pMotionParts->GetNextMotionParts();
	}

	return nullptr;
}

//*****************************************************************************
//���[�V�������f���̈ړ�
//*****************************************************************************
void CMotionParts::MoveMotionModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nModelNum, int nMotionNum)
{
	if (m_nMotionPlayMotonNum[nModelNum] != nMotionNum)
	{
		m_nMotionPlayMotonNum[nModelNum] = nMotionNum;
		//�S�̂̎��܂ł̈ʒu�̌v�Z
		AllNextMotionPosition(nModelNum);
	}

	GetMotionPartsPointer(nModelNum, 0)->SetParentPos(pos);
	GetMotionPartsPointer(nModelNum, 0)->SetParentRot(rot);
}

//*****************************************************************************
//���[�V�������f���̃��C�g�x�N�g��
//*****************************************************************************
void CMotionParts::SetLight(D3DXVECTOR3 vec, int nMotionNum)
{
	CMotionParts* pMotionParts = m_pMotionPartsTop;

	while (pMotionParts != nullptr)
	{
		if (pMotionParts->GetMotionParts(nMotionNum))
		{
			pMotionParts->SetLightVec(vec);
		}
		pMotionParts = pMotionParts->GetNextMotionParts();
	}
}

//*****************************************************************************
//�e�̐ݒ�
//*****************************************************************************
void CMotionParts::AllSetShadowPos(D3DXVECTOR3 pos, int nMotionNum)
{
	CMotionParts* pMotionParts = m_pMotionPartsTop;

	while (pMotionParts != nullptr)
	{
		if (pMotionParts->GetMotionParts(nMotionNum))
		{
			D3DXVECTOR3 ShadowPos = pMotionParts->GetWorldPos();
			ShadowPos.y = pos.y;
			pMotionParts->SetShadowPos(ShadowPos);
		}
		pMotionParts = pMotionParts->GetNextMotionParts();
	}
}

//*****************************************************************************
//���[�V�����̓o�^
//*****************************************************************************
void CMotionParts::SetMotionFileData(const MotionMoveData MotionMoveData, int nMotionNum)
{
	CMotionParts* pMotionParts = nullptr;//�ꎞ�ۊǏꏊ
	int nPartsNum = 0;//�J�E���^�[
	pMotionParts = GetMotionPartsPointer(nMotionNum, nPartsNum);//�����ɍ����|�C���^�̊l��
	while (pMotionParts != nullptr)
	{
		//�ꎞ�ۊǏꏊ
		KEY_SET KeySet;

		KeySet.pKey = new KEY[MotionMoveData.nKeyMax];

		for (int nCnt = 0; nCnt < MotionMoveData.nKeyMax; nCnt++)
		{
			//�t���[��
			KeySet.pKey[nCnt].nFrame = MotionMoveData.pMotionKeyData[nCnt].nFrame;
			//Pos
			KeySet.pKey[nCnt].pos = MotionMoveData.pMotionKeyData[nCnt].pMotionPartsData[nPartsNum].pos;
			//Rot
			KeySet.pKey[nCnt].rot = MotionMoveData.pMotionKeyData[nCnt].pMotionPartsData[nPartsNum].rot;
		}

		//���[�v�ݒ�
		KeySet.bLoop = MotionMoveData.bLoop;
		//�L�[�̍ő�
		KeySet.nKeyMax = MotionMoveData.nKeyMax;

		//�o�^
		pMotionParts->SetMotionData(KeySet);

		nPartsNum++;//�J�E���^�[��i�߂�
		pMotionParts = GetMotionPartsPointer(nMotionNum, nPartsNum);//�����ɍ����|�C���^�̊l��
	}
	
	//���[�V�����̓o�^������
	m_nMotionRegistrationNum[nMotionNum]++;
}

//*****************************************************************************
//�����蔻��i�����̃��f���ԍ��A�����̈ʒu�j
//*****************************************************************************
D3DXVECTOR3 CMotionParts::AllCollision(int nMotionNum, D3DXVECTOR3 pos, D3DXVECTOR3 oldpos)
{
	CMotionParts* pMotionParts = m_pMotionPartsTop;

	while (pMotionParts != nullptr)
	{
		if (!pMotionParts->GetMotionParts(nMotionNum))
		{
			D3DXVECTOR3 Add = pMotionParts->Collision(pos, oldpos);
			if (Add != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
			{
				return Add;
			}
			
		}
		pMotionParts = pMotionParts->GetNextMotionParts();
	}

	return pos;
}

D3DXVECTOR3 CMotionParts::AllCollision(int nMyselfMotionNum, int nTargetMotionNum, D3DXVECTOR3 pos, D3DXVECTOR3 oldpos)
{
	CMotionParts* pMotionParts = m_pMotionPartsTop;

	while (pMotionParts != nullptr)
	{
		if (!pMotionParts->GetMotionParts(nMyselfMotionNum) && pMotionParts->GetMotionParts(nTargetMotionNum))
		{
			D3DXVECTOR3 Add = pMotionParts->Collision(pos, oldpos);
			if (Add != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
			{
				return Add;
			}

		}
		pMotionParts = pMotionParts->GetNextMotionParts();
	}

	return pos;
}
