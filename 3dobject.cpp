//=============================================================================
//
// 3dobject.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "3dobject.h"
#include "application.h"
#include "renderer.h"

C3DObject::ModelPattern C3DObject::m_ModelPattern[MODEL_PATTERN_MAX] = {};
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
C3DObject::C3DObject()
{
	ZeroMemory(&m_Model, sizeof(m_Model));
	m_fSize = 1.0f;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
C3DObject::~C3DObject()
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT C3DObject::Init()
{
	ZeroMemory(&m_Model, sizeof(m_Model));
	m_fSize = 1.0f;

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void C3DObject::Uninit()
{
	if (m_Model.pTopPos != nullptr)
	{
		delete[] m_Model.pTopPos;
		m_Model.pTopPos = nullptr;
	}

	if(m_Model.pNormalPolygon != nullptr)
	{
		delete[] m_Model.pNormalPolygon;
		m_Model.pNormalPolygon = nullptr;
	}

}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void C3DObject::Update()
{

}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void C3DObject::Draw()
{
	CRenderer* pRenderer = CApplication::GetRenderer();

	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = pRenderer->GetDevice();

	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ�
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	//�e������
	if (m_LightVec != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		D3DXMATRIX mtxShadow;
		D3DXPLANE pIaneField;
		D3DXVECTOR4 vecLight;
		D3DXVECTOR3 pos, normal;

		pos = m_ShadowPos;
		normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		
		vecLight = D3DXVECTOR4(m_LightVec.x, m_LightVec.y, m_LightVec.z,0.0f) * -1.0f;

		D3DXPlaneFromPointNormal(&pIaneField, &pos, &normal);

		D3DXMatrixShadow(&mtxShadow, &vecLight, &pIaneField);

		D3DXMatrixMultiply(&mtxShadow, &m_Model.mtxWorld, &mtxShadow);

		//���[���h�}�g���b�N�X�̐ݒ�
		pD3DDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

		//���݂̃}�e���A����ۑ�
		pD3DDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)m_ModelPattern[m_Model.nPattn].pBuffMatModel->GetBufferPointer();

		//�e�N�X�`���̐ݒ�
		pD3DDevice->SetTexture(0, NULL);

		for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_Model.nPattn].nNumMatModel; nCntMat++)
		{

			pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			pMat[nCntMat].MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			//�}�e���A���̐ݒ�
			pD3DDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pD3DDevice->SetTexture(0, m_ModelPattern[m_Model.nPattn].pTexture[nCntMat]);

			//���f���p�[�c�̕`��
			m_ModelPattern[m_Model.nPattn].pMeshModel->DrawSubset(nCntMat);
		}

		//�ۑ����Ă����}�e���A����߂�
		pD3DDevice->SetMaterial(&matDef);

	}

	//���[���h�}�g���b�N�X�̐ݒ�
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_Model.mtxWorld);

	//���݂̃}�e���A����ۑ�
	pD3DDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_ModelPattern[m_Model.nPattn].pBuffMatModel->GetBufferPointer();

	//�e�N�X�`���̐ݒ�
	pD3DDevice->SetTexture(0, NULL);

	for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_Model.nPattn].nNumMatModel; nCntMat++)
	{
		pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�}�e���A���̐ݒ�
		pD3DDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pD3DDevice->SetTexture(0, m_ModelPattern[m_Model.nPattn].pTexture[nCntMat]);

		//���f���p�[�c�̕`��
		m_ModelPattern[m_Model.nPattn].pMeshModel->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pD3DDevice->SetMaterial(&matDef);


	if (m_Model.pNormalPolygon == nullptr)
	{
		//�@���̐ݒ�
		SetNormal();
	}
	else
	{
		//�@���̍X�V
		UpdateNormal();
	}
	

}

//*****************************************************************************
// �RD�I�u�W�F�N�g�̔z�u
//*****************************************************************************
void C3DObject::Set3DObject(int nPattn, D3DXVECTOR3 pos)
{
	int nNumVix;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	m_Model.pos = pos;
	m_Model.nPattn = nPattn;

	//���f���̃T�C�Y�̔�r�p�����l
	m_Model.vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	m_Model.vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

	//���_���̎擾
	nNumVix = m_ModelPattern[m_Model.nPattn].pMeshModel->GetNumVertices();

	//���_�t�H�[�}�b�g�擾
	sizeFVF = D3DXGetFVFVertexSize(m_ModelPattern[m_Model.nPattn].pMeshModel->GetFVF());

	//���_���b�N
	m_ModelPattern[m_Model.nPattn].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//���ׂĂ̒��_POS�̎擾
	for (int nCntVtx = 0; nCntVtx < nNumVix; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (m_Model.vtxMax.x < vtx.x)
		{//X
			m_Model.vtxMax.x = vtx.x;
		}
		if (m_Model.vtxMin.x > vtx.x)
		{
			m_Model.vtxMin.x = vtx.x;
		}

		if (m_Model.vtxMax.y < vtx.y)
		{//Y
			m_Model.vtxMax.y = vtx.y;
		}
		if (m_Model.vtxMin.y > vtx.y)
		{
			m_Model.vtxMin.y = vtx.y;
		}

		if (m_Model.vtxMax.z < vtx.z)
		{//Z
			m_Model.vtxMax.z = vtx.z;
		}
		if (m_Model.vtxMin.z > vtx.z)
		{
			m_Model.vtxMin.z = vtx.z;
		}

		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	//���_�A�����b�N
	m_ModelPattern[m_Model.nPattn].pMeshModel->UnlockVertexBuffer();

	//�@���̐ݒ�
	SetNormal();
}

//*****************************************************************************
// �}�g���b�N�X�̌v�Z
//*****************************************************************************
void C3DObject::CalculationMatrix()
{
	
	D3DXMATRIX mtxRoot;					//���̐e�̃��[���h�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScaling;		//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRotParent, mtxRotChild;

	//���[���h�}�g���b�N�X�̏������i���̐e�j
	D3DXMatrixIdentity(&mtxRoot);

	//�T�C�Y�̕ύX
	D3DXMatrixScaling(&mtxScaling, m_fSize, m_fSize, m_fSize);
	D3DXMatrixMultiply(&mtxRoot, &mtxRoot, &mtxScaling);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Model.rotParent.y, m_Model.rotParent.x, m_Model.rotParent.z);
	D3DXMatrixMultiply(&mtxRoot, &mtxRoot, &mtxRot);

	//���[���h�}�g���b�N�X���������̕ۑ�
	mtxRotParent = mtxRot;

	//�ʒu�̔��f
	D3DXMatrixTranslation(&mtxTrans, m_Model.posParent.x, m_Model.posParent.y, m_Model.posParent.z);
	D3DXMatrixMultiply(&mtxRoot, &mtxRoot, &mtxTrans);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_Model.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Model.rot.y, m_Model.rot.x, m_Model.rot.z);
	D3DXMatrixMultiply(&m_Model.mtxWorld, &m_Model.mtxWorld, &mtxRot);

	//���[���h�}�g���b�N�X���������̕ۑ�
	mtxRotChild = mtxRot;

	D3DXVECTOR3 pos = m_Model.pos + m_Model.posMove;
	//�ʒu�̔��f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_Model.mtxWorld, &m_Model.mtxWorld, &mtxTrans);

	//���f���̃}�g���b�N�X�@���@�e�̃��[���h�}�g���b�N�X
	D3DXMatrixMultiply(&m_Model.mtxWorld, &m_Model.mtxWorld, &mtxRoot);

	//���f���̃}�g���b�N�X�@���@�e�̃��[���h�}�g���b�N�X(��������)
	D3DXMatrixMultiply(&m_Model.mtxWorldRot, &mtxRotChild, &mtxRotParent);

}

//*****************************************************************************
// ROT�̐��K��
//*****************************************************************************
D3DXVECTOR3 C3DObject::NormalizationRot(D3DXVECTOR3 In)
{
	D3DXVECTOR3 rot = In;

	if (rot.x > D3DX_PI)
	{
		rot.x -= D3DX_PI * 2.0f;
	}
	else if (rot.x < -D3DX_PI)
	{
		rot.x += D3DX_PI * 2.0f;
	}

	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}
	else if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}

	if (rot.z > D3DX_PI)
	{
		rot.z -= D3DX_PI * 2.0f;
	}
	else if (rot.z < -D3DX_PI)
	{
		rot.z += D3DX_PI * 2.0f;
	}

	return rot;
}

//*****************************************************************************
//�@���ݒ�
//*****************************************************************************
void C3DObject::SetNormal()
{
	int nNumVix;		//���_��
	int nNumIndex;		//�C���f�b�N�X��
	int nNumPolygon;	//�|���S����

	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^
	BYTE *pIndexBuff;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	//���_���̎擾
	nNumVix = m_ModelPattern[m_Model.nPattn].pMeshModel->GetNumVertices();
	//�|���S�����̎擾
	nNumPolygon = m_ModelPattern[m_Model.nPattn].pMeshModel->GetNumFaces();
	//�C���f�N�X���̎擾
	nNumIndex = nNumPolygon + 2;
	
	//���_�̐ݒ�
	m_Model.pTopPos = new D3DXVECTOR3[nNumVix];

	//�@���̐ݒ�
	m_Model.pNormalPolygon = new D3DXVECTOR3[nNumPolygon];

	//���_�t�H�[�}�b�g�擾
	sizeFVF = D3DXGetFVFVertexSize(m_ModelPattern[m_Model.nPattn].pMeshModel->GetFVF());

	//���_���b�N
	m_ModelPattern[m_Model.nPattn].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);


	//���ׂĂ̒��_POS�̎擾
	for (int nCnt = 0; nCnt < nNumVix; nCnt++)
	{
		//���_���W�̎擾
		m_Model.pTopPos[nCnt] = *(D3DXVECTOR3*)pVtxBuff;
		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	//���_�A�����b�N
	m_ModelPattern[m_Model.nPattn].pMeshModel->UnlockVertexBuffer();

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	m_ModelPattern[m_Model.nPattn].pMeshModel->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndexBuff);

	int nIndex1, nIndex2, nIndex3;
	//�@���̌v�Z
	for (int nCnt = 0; nCnt < nNumPolygon; nCnt++)
	{
		
		nIndex1 = *(WORD*)pIndexBuff;
		nIndex2 = *((WORD*)pIndexBuff + 1);
		nIndex3 = *((WORD*)pIndexBuff + 2);

		//���_���W�̑��
		D3DXVECTOR3 vtx1 = m_Model.pTopPos[nIndex1];
		D3DXVECTOR3 vtx2 = m_Model.pTopPos[nIndex2];
		D3DXVECTOR3 vtx3 = m_Model.pTopPos[nIndex3];

		D3DXVECTOR3 vec1, vec2, vecResult;

		vec1 = vtx2 - vtx1;
		vec2 = vtx3 - vtx1;

		//�O��
		/*if (nCnt % 2 == 1)
		{
			D3DXVec3Cross(&vecResult, &vec2, &vec1);
		}
		else
		{*/
			D3DXVec3Cross(&vecResult, &vec1, &vec2);
		//}
		//���K��
		D3DXVec3Normalize(&vecResult, &vecResult);

		//�ʂ̖@���x�N�g���̕ۑ�
		m_Model.pNormalPolygon[nCnt] = vecResult;

		//�f�[�^��i�߂�
		pIndexBuff+=sizeof(WORD)*3;
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_ModelPattern[m_Model.nPattn].pMeshModel->UnlockIndexBuffer();

}

//*****************************************************************************
//�@���̌����̍X�V
//*****************************************************************************
void C3DObject::UpdateNormal()
{
//	int nNumVix;		//���_��
//	D3DXVECTOR3 vec;
//
//	//���_���̎擾
//	nNumVix = m_ModelPattern[m_Model.nPattn].pMeshModel->GetNumVertices();
//
//	//�@���̍X�V
//	for (int nCntVtx = 0; nCntVtx < nNumVix; nCntVtx++)
//	{
//		//�@���x�N�g���̈ꎞ�ۑ�
//		vec = m_Model.pNormalPolygon[nCntVtx];
//		//���[���h�}�g���b�N�X���g���������ϊ�
//		D3DXVec3TransformCoord(&vec,&vec, &m_Model.mtxWorld);
//		//�ʂ̖@���x�N�g���̕ۑ�
//		m_Model.pNormalPolygon[nCntVtx] = vec;
//	}
}

//*****************************************************************************
//�����蔻��
//*****************************************************************************
D3DXVECTOR3 C3DObject::Collision(D3DXVECTOR3 pos, D3DXVECTOR3 oldpos)
{
	int nNumVix;		//���_��
	int nNumIndex;		//�C���f�b�N�X��
	int nNumPolygon;	//�|���S����

	BYTE *pIndexBuff;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	//���_���̎擾
	nNumVix = m_ModelPattern[m_Model.nPattn].pMeshModel->GetNumVertices();
	//�|���S�����̎擾
	nNumPolygon = m_ModelPattern[m_Model.nPattn].pMeshModel->GetNumFaces();
	//�C���f�N�X���̎擾
	nNumIndex = nNumPolygon + 2;

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	m_ModelPattern[m_Model.nPattn].pMeshModel->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndexBuff);

	int nIndex1, nIndex2, nIndex3;

	//���ׂĂ̒��_POS�̎擾
	for (int nCnt = 0; nCnt < nNumPolygon; nCnt++)
	{
		//�@���̎擾
		D3DXVECTOR3 Normal = m_Model.pNormalPolygon[nCnt];
		//���[���h�}�g���b�N�X�Ƃ̊|���Z
		D3DXVec3TransformCoord(&Normal, &Normal, &m_Model.mtxWorldRot);
		//���K��
		D3DXVec3Normalize(&Normal, &Normal);

		//�C���f�b�N�X���璸�_�ԍ��̎擾
		nIndex1 = *(WORD*)pIndexBuff;
		nIndex2 = *((WORD*)pIndexBuff + 1);
		nIndex3 = *((WORD*)pIndexBuff + 2);

		D3DXVECTOR3 vtx0, vtx1, vtx2;
		//���_���W�̑��
		//���[���h�}�g���b�N�X�Ƃ̊|���Z
		D3DXVec3TransformCoord(&vtx0, &m_Model.pTopPos[nIndex1], &m_Model.mtxWorld);
		D3DXVec3TransformCoord(&vtx1, &m_Model.pTopPos[nIndex2], &m_Model.mtxWorld);
		D3DXVec3TransformCoord(&vtx2, &m_Model.pTopPos[nIndex3], &m_Model.mtxWorld);


		//���ʏ��
		D3DXPLANE Plane;
		D3DXPlaneFromPointNormal(&Plane, &vtx0, &Normal);

		//��_
		D3DXVECTOR3 IntersectionPos;

		//���ʂ�˂������钼���̊ђʓ_���擾
		D3DXPlaneIntersectLine(&IntersectionPos, &Plane, &pos, &oldpos);

		if (IntersectionPos == D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{
			continue;
		}


		D3DXVECTOR3 vec1 = pos - oldpos;
		D3DXVECTOR3 vec2 = IntersectionPos - oldpos;

		if (D3DXVec3Dot(&Normal, &vec1) > 0.0f)
		{
			continue;
		}

		float fSize1 = D3DXVec3Length(&vec1);
		float fSize2 = D3DXVec3Length(&vec2);

		if (fSize1 > fSize2)
		{
			//�O�p�`�̓��ɂ��邩�ǂ���
			if (TriangleInOut(pos, vtx0, vtx1, vtx2))
			{
				D3DXVECTOR3 IntersectionPosVec1 = IntersectionPos - pos;
				D3DXVECTOR3 IntersectionPosVec2 = IntersectionPos - oldpos;

				D3DXVec3Normalize(&IntersectionPosVec1, &IntersectionPosVec1);
				D3DXVec3Normalize(&IntersectionPosVec2, &IntersectionPosVec2);

				D3DXVECTOR3 Criteria = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				if (
					((IntersectionPosVec1 > Criteria && IntersectionPosVec2 > Criteria)
						|| (IntersectionPosVec1 < Criteria && IntersectionPosVec2 < Criteria))
					)
				{

					D3DXVECTOR3 WallSlide = (vec1 - D3DXVec3Dot(&vec1, &Normal) * Normal) * 1.0f;

					return IntersectionPos + WallSlide;
				}
			}
		}
		//�f�[�^��i�߂�
		pIndexBuff += sizeof(WORD) * 3;
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_ModelPattern[m_Model.nPattn].pMeshModel->UnlockIndexBuffer();

	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//*****************************************************************************
//�O�p�`�̓��ɂ��邩�ǂ���
//*****************************************************************************
bool C3DObject::TriangleInOut(D3DXVECTOR3 pos, D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2)
{
	D3DXVECTOR3 vec1, vec2;
	float fInnerProduct0[3], fInnerProduct1[3], fInnerProduct2[3];

	vec1 = vtx1 - vtx0;
	vec2 = pos - vtx0;

	fInnerProduct0[0] = vec1.x * vec2.z - vec1.z * vec2.x;
	fInnerProduct0[1] = vec1.y * vec2.x - vec1.x * vec2.y;
	fInnerProduct0[2] = vec1.z * vec2.y - vec1.y * vec2.z;

	vec1 = vtx2 - vtx1;
	vec2 = pos - vtx1;

	fInnerProduct1[0] = vec1.x * vec2.z - vec1.z * vec2.x;
	fInnerProduct1[1] = vec1.y * vec2.x - vec1.x * vec2.y;
	fInnerProduct1[2] = vec1.z * vec2.y - vec1.y * vec2.z;

	vec1 = vtx0 - vtx2;
	vec2 = pos - vtx2;

	fInnerProduct2[0] = vec1.x * vec2.z - vec1.z * vec2.x;
	fInnerProduct2[1] = vec1.y * vec2.x - vec1.x * vec2.y;
	fInnerProduct2[2] = vec1.z * vec2.y - vec1.y * vec2.z;


	if (
		(fInnerProduct0[0] > 0.0f && fInnerProduct1[0] > 0.0f && fInnerProduct2[0] > 0.0f)
		|| (fInnerProduct0[0] < 0.0f && fInnerProduct1[0] < 0.0f && fInnerProduct2[0] < 0.0f)
		)
	{
		return true;
	}

	if (
		(fInnerProduct0[1] > 0.0f && fInnerProduct1[1] > 0.0f && fInnerProduct2[1] > 0.0f)
		|| (fInnerProduct0[1] < 0.0f && fInnerProduct1[1] < 0.0f && fInnerProduct2[1] < 0.0f)
		)
	{
		return true;
	}

	if (
		(fInnerProduct0[2] > 0.0f && fInnerProduct1[2] > 0.0f && fInnerProduct2[2] > 0.0f)
		|| (fInnerProduct0[2] < 0.0f && fInnerProduct1[2] < 0.0f && fInnerProduct2[2] < 0.0f)
		)
	{
		return true;
	}


	return false;
}

//*****************************************************************************
// ���f���̃Z�b�g
//*****************************************************************************
int C3DObject::SetModel(ModelPattern *pModel)
{
	for (int nCnt = 0; nCnt < MODEL_PATTERN_MAX; nCnt++)
	{
		if (!m_ModelPattern[nCnt].bUse)
		{
			m_ModelPattern[nCnt] = *pModel;
			m_ModelPattern[nCnt].bUse = true;
			return nCnt;
		}
	}
	return -1;
}

//*****************************************************************************
// ���f���̑S�I������
//*****************************************************************************
void C3DObject::UninitAllModel()
{
	for (int nCnt = 0; nCnt < MODEL_PATTERN_MAX; nCnt++)
	{
		for (int i = 0; i < (int)m_ModelPattern[nCnt].nNumMatModel; i++)
		{
			if (m_ModelPattern[nCnt].pTexture[i] != nullptr)
			{// �e�N�X�`���̉��
				m_ModelPattern[nCnt].pTexture[i]->Release();
				m_ModelPattern[nCnt].pTexture[i] = nullptr;
			}
		}

		//���b�V���̔j��
		if (m_ModelPattern[nCnt].pMeshModel != nullptr)
		{
			m_ModelPattern[nCnt].pMeshModel->Release();
			m_ModelPattern[nCnt].pMeshModel = nullptr;
		}

		//�}�e���A���̔j��
		if (m_ModelPattern[nCnt].pBuffMatModel != nullptr)
		{
			m_ModelPattern[nCnt].pBuffMatModel->Release();
			m_ModelPattern[nCnt].pBuffMatModel = nullptr;
		}

	}

	ZeroMemory(&m_ModelPattern, sizeof(m_ModelPattern));
}

//*****************************************************************************
// ���̃��f���̃��[���h���W
//*****************************************************************************
D3DXVECTOR3 C3DObject::GetWorldPos()
{
	D3DXVECTOR3 pos;
	pos.x = m_Model.mtxWorld._41;
	pos.y = m_Model.mtxWorld._42;
	pos.z = m_Model.mtxWorld._43;

	return pos;
}

D3DXVECTOR3 C3DObject::GetWorlMatrixRot()
{
	D3DXVECTOR3 pos;
	pos.x = m_Model.mtxWorldRot._41;
	pos.y = m_Model.mtxWorldRot._42;
	pos.z = m_Model.mtxWorldRot._43;
	return pos;
}

//*****************************************************************************
// ���̃��f���̃��[���hRot
//*****************************************************************************
D3DXVECTOR3 C3DObject::GetWorldRot()
{
	D3DXVECTOR3 rot;
	rot.x = m_Model.mtxWorld._41;
	rot.y = m_Model.mtxWorld._42;
	rot.z = m_Model.mtxWorld._43;

	return rot;
}

//*****************************************************************************
// ���f���̃T�C�Y
//*****************************************************************************
D3DXVECTOR3 C3DObject::GetSize()
{
	D3DXVECTOR3 size;

	D3DXVECTOR3 minpos, maxpos;

	minpos = m_Model.pos + m_Model.vtxMin;

	maxpos = m_Model.pos + m_Model.vtxMax;

	size = maxpos - minpos;
	
	return size;
}