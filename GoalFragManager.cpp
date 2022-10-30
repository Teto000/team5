//===================================================
//
// �S�[���֌W�̃t���O
// Author : Arita Meigen
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
#include "GoalFragManager.h"
#include"player.h"
#include"game.h"
#include"application.h"
#include"meshfield.h"
#include"object2D.h"
#include"fade.h"
#include"renderer.h"

//===========================
// �R���X�g���N�^
//===========================
CGoalFragManager::CGoalFragManager()
{
}

//===========================
// �f�X�g���N�^
//===========================
CGoalFragManager::~CGoalFragManager()
{
}

//===========================
// ������
//===========================
HRESULT CGoalFragManager::Init()
{
	//�t���O���Z�b�g
	m_bGoal = false;

	//-----------------------
	// �f�o�C�X�̎擾
	//-----------------------
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//-----------------------
	// X�t�@�C���̓ǂݍ���
	//-----------------------
	D3DXLoadMeshFromX("data\\MODEL\\X_File\\legL.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	return S_OK;
}

//===========================
// �I��
//===========================
void CGoalFragManager::Uninit()
{
	//-----------------------
	// ���b�V���̔j��
	//-----------------------
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	//-----------------------
	// �}�e���A���̔j��
	//-----------------------
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}
}

//===========================
// �X�V
//===========================
void CGoalFragManager::Update()
{
	if (m_bGoal == true)
	{
		CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
	}
}

//========================
// �`��
//========================
void CGoalFragManager::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//�f�o�C�X�̎擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���f���̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//���f���̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//���f���̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//===========================
// �����蔻��
//===========================
bool CGoalFragManager::Collision()
{
	for (int i = 0; i < 2; i++)
	{
		CPlayer*pPlayer = CGame::GetPlayer(i);

		//������Ԃ��������p
		//Y��
		if (pPlayer->GetPosition().y < m_pos.y + m_vtxMax.y
			&&pPlayer->GetPosition().y  > m_pos.y - m_vtxMin.y)
		{
			//X��
			if (pPlayer->GetPosition().z < m_pos.z + m_vtxMax.z
				&&pPlayer->GetPosition().z  > m_pos.z + m_vtxMin.z)
			{
				m_bGoal = true;
			}
			//Z��
			else if (pPlayer->GetPosition().x < m_pos.x + m_vtxMax.x
				&&pPlayer->GetPosition().x > m_pos.x + m_vtxMin.x)
			{
				m_bGoal = true;
			}
		}
	}
	return m_bGoal;
}

//========================
// ����
//========================
CGoalFragManager* CGoalFragManager::Create( D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{//���f���̃p�X , �e���f�����猩���ʒu , �e���f�����猩������

	//----------------------------------
	// �����Ə�����
	//----------------------------------
	CGoalFragManager *pGoal = nullptr;
	pGoal = new CGoalFragManager;	//����

	if (pGoal != nullptr)
	{//NULL�`�F�b�N
	 //�����o�ϐ��ɑ��
		pGoal->m_pos = pos;
		pGoal->m_rot = rot;

		//������
		pGoal->Init();
	}

	return pGoal;
}


//===========================
// �S�[���t���O�̎擾
//===========================
bool CGoalFragManager::GetGoalFrag()
{
	return m_bGoal;
}