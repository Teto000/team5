//===================================================
//
// �u���b�N�̔���
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
#include "block.h"
#include "player.h"
#include "game.h"
#include "application.h"
#include "meshfield.h"
#include "object2D.h"
#include "fade.h"
#include "renderer.h"
#include "game.h"

#define PLAYER_SIZE (20.0f)
bool CBlock::m_bAbove = false;
bool CBlock::m_bHaveBlock = false;
//===========================
// �R���X�g���N�^
//===========================
CBlock::CBlock(int nPriority)
{
}

//===========================
// �f�X�g���N�^
//===========================
CBlock::~CBlock()
{
}

//===========================
// ������
//===========================
HRESULT CBlock::Init(D3DXVECTOR3 pos)
{
	//-----------------------
	// �f�o�C�X�̎擾
	//-----------------------
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//-----------------------
	// X�t�@�C���̓ǂݍ���
	//-----------------------
	D3DXLoadMeshFromX("data\\MODEL\\X_File\\Stained_Glass000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	//���_���̎擾
	int nNumVtx = m_pMesh->GetNumVertices();	//���_��
	DWORD sizeFVF;								//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE*pVtxBuff;								//���_�o�b�t�@�ւ̃|�C���^

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//���_���W�̑��
	//���ׂĂ̒��_��pos���擾����
	m_vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);	//�ő�l�̕ۑ��p
	m_vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);	//�ŏ��l�̕ۑ��p

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;
		//���_���W���r���ă��f���̍ŏ��l�ő�l���擾
		if (vtx.x > m_vtxMax.x)
		{//X�̍ő�l���擾
			m_vtxMax.x = vtx.x;
		}
		if (vtx.x < m_vtxMin.x)
		{//X�̍ŏ��l���擾
			m_vtxMin.x = vtx.x;
		}
		if (vtx.y > m_vtxMax.y)
		{//Y�̍ő�l���擾
			m_vtxMax.y = vtx.y;
		}
		if (vtx.y < m_vtxMin.y)
		{//Y�̍ŏ��l���擾
			m_vtxMin.y = vtx.y;
		}
		if (vtx.z > m_vtxMax.z)
		{//Z�̍ő�l���擾
			m_vtxMax.z = vtx.z;
		}
		if (vtx.z < m_vtxMin.z)
		{//Z�̍ŏ��l���擾
			m_vtxMin.z = vtx.z;
		}
		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}
	//���_�̍ő�l�ƍŏ��l��ۑ�
	m_vtxMax = m_vtxMax;
	m_vtxMin = m_vtxMin;

	//���_�o�b�t�@�̃A�����b�N
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//===========================
// �I��
//===========================
void CBlock::Uninit()
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
	//Release();
}

//===========================
// �X�V
//===========================
void CBlock::Update()
{
	// �����蔻��
	Collision();
}

//========================
// �`��
//========================
void CBlock::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//�f�o�C�X�̎擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p

	D3DXMATERIAL *pMat;		//�}�e���A���f�[�^�ւ̃|�C���^

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

	//---------------------------------
	// �L�����N�^�[�̕`��
	//---------------------------------
	//���݂̃}�e���A����ێ�
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &pTexture);

		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�v���C���[�p�[�c�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	//�ێ����Ă���}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//===========================
// �����蔻��
//===========================
bool CBlock::Collision()
{
	//�J�����̍ő吔���擾
	int nMaxCamera = CRenderer::GetMaxCamera();

	//----------------------------
	// �J�����̍ő吔�̐ݒ�
	//----------------------------
	if (CGame::GetEnumCamera() == CGame::NUMCAMERA_THREE)
	{//�J�����񋓌^��3�Ȃ�
	 //�J�����̍ő吔��1���炷
		nMaxCamera = 3;
	}

	for (int i = 0; i < nMaxCamera; i++)
	{//�v���C���[�̍ő�l������
		CPlayer*pPlayer = CGame::GetPlayer(i);

		//������Ԃ��������p
		////Y��
		//if (pPlayer->GetPosition().y < m_pos.y + m_vtxMax.y
		//	&&pPlayer->GetPosition().y  > m_pos.y - m_vtxMin.y)
		//{
		//}
		if (pPlayer != nullptr)
		{
			//X��
			if (pPlayer->GetPosition().z - PLAYER_SIZE < m_pos.z + m_vtxMax.z && pPlayer->GetPosition().z + PLAYER_SIZE  > m_pos.z + m_vtxMin.z)
			{
				//Z��
				if (pPlayer->GetPosition().x - PLAYER_SIZE < m_pos.x + m_vtxMax.x
					&& pPlayer->GetPosition().x + PLAYER_SIZE > m_pos.x + m_vtxMin.x)
				{// �������Ă���
					switch (m_state)
					{
					case FIELD_BLOCK:
						m_bHaveBlock = true;
						break;

					case PLAYER_BLOCK:
						m_bAbove = true;
						break;
					}
				}
				else
				{// X�œ������ĂȂ�
					m_bAbove = false;
				}
			}
			else
			{// Z�œ������ĂȂ�
				m_bAbove = false;
			}
		}
	}
	return m_bAbove;
}

//========================
// ����
//========================
CBlock* CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, BLOCK state)
{//���f���̃p�X , �e���f�����猩���ʒu , �e���f�����猩������

 //----------------------------------
 // �����Ə�����
 //----------------------------------
	CBlock *pCBlock = nullptr;
	//pGoal = new CGoal(CObject::OBJTYPE_GOAL);	//����
	pCBlock = new CBlock(CObject::OBJTYPE_BLOCK);	//����

	if (pCBlock != nullptr)
	{//NULL�`�F�b�N
	 //�����o�ϐ��ɑ��
		pCBlock->m_pos = pos;
		pCBlock->m_rot = rot;
		pCBlock->m_state = state;

		//������
		pCBlock->Init(pos);
	}

	return pCBlock;
}

//========================
// �ʒu�̐ݒ�
//========================
void CBlock::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

////===========================
//// �ʒu�̎擾
////===========================
//D3DXVECTOR3 CBlock::GetPosition()
//{
//	return m_pos;
//}