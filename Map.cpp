#include "Map.h"
#include "object.h"
#include "renderer.h"


//===========================
// �R���X�g���N�^
//===========================
CMap::CMap(int nPriority):CObject(nPriority)
{

}

//===========================
// �f�X�g���N�^
//===========================
CMap::~CMap()
{

}

//===========================
// ������
//===========================
HRESULT CMap::Init(D3DXVECTOR3 pos)
{
	//-----------------------
	// �f�o�C�X�̎擾
	//-----------------------
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//-----------------------
	// X�t�@�C���̓ǂݍ���
	//-----------------------
	D3DXLoadMeshFromX("data\\MODEL\\X_File\\Map_001.x",
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
void CMap::Uninit()
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
void CMap::Update()
{

}

//===========================
// �`��
//===========================
void CMap::Draw()
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

	//���݂̃}�e���A����ێ�
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�v���C���[�p�[�c�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	//�ێ����Ă���}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
// ����
//========================
CMap* CMap::Create( D3DXVECTOR3 pos)
{//���f���̃p�X , �e���f�����猩���ʒu , �e���f�����猩������
	CMap *pMap = nullptr;

	//----------------------------------
	// �����Ə�����
	//----------------------------------
	pMap = new CMap(0);	//����

	if (pMap != nullptr)
	{//NULL�`�F�b�N
	 //�����o�ϐ��ɑ��
		pMap->m_pos = pos;

		//������
		pMap->Init(pMap->m_pos);
	}

	return pMap;
}


//========================
// �ʒu�̐ݒ�
//========================
void CMap::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}


//========================
// �ʒu�̎擾
//========================
D3DXVECTOR3 CMap::GetPosition()
{
	return m_pos;
}


//========================
// �����̎擾
//========================
float CMap::GetWidth()
{
	return 0.0f;
}


//========================
// �c���̎擾
//========================
float CMap::GetHeight()
{
	return 0.0f;
}
